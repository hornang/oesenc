#include <algorithm>
#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#ifdef OESENC_LINUX
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef OESENC_WINDOWS
#include <windows.h>
#endif

#include <oesenc/serverreader.h>

#include "random.h"

using namespace oesenc;
using namespace std;

namespace {

const chrono::duration serverExitTimeout = 5000ms;

auto createMessage(ServerReader::Command command,
                   const string_view &pipeName,
                   const string_view &chartFile,
                   const string_view &userKey)
{
    array<std::byte, 1 + 256 + 256 + 512> message {};
    fill(message.begin(), message.end(), std::byte(0));
    message[0] = static_cast<std::byte>(command);

    transform(pipeName.cbegin(),
              pipeName.cend(),
              message.begin() + 1,
              [](unsigned char c) { return std::byte(c); });

    transform(chartFile.cbegin(),
              chartFile.cend(),
              message.begin() + 1 + 256,
              [](unsigned char c) { return std::byte(c); });

    transform(userKey.cbegin(),
              userKey.cend(),
              message.begin() + 1 + 256 + 256,
              [](unsigned char c) { return std::byte(c); });

    return message;
}

class InputStream : public ifstream
{
public:
    InputStream(const string &pipeName)
        : ifstream(pipeName, ios::binary)
        , m_pipeName(pipeName)
    {
    }

    ~InputStream()
    {
        filesystem::remove(m_pipeName);
    }

private:
    string m_pipeName;
};

}

#ifdef OESENC_LINUX
string createReturnPipe()
{
    const int numTries = 10;

    for (int i = 0; i < numTries; i++) {
        string returnPipe = "/tmp/oesenc_" + randomAlphaNumericString(10);
        if (filesystem::exists(returnPipe)) {
            continue;
        }
        if (mkfifo(returnPipe.data(), 0660) != 0) {
            continue;
        }
        return returnPipe;
    }

    return {};
}

unique_ptr<istream> ServerReader::request(string_view pipeName,
                                          Command command,
                                          string_view chartFile,
                                          string_view userKey)
{
    if (!filesystem::exists(pipeName) || !filesystem::is_fifo(pipeName)) {
        return {};
    }

    auto returnPipe = createReturnPipe();

    if (returnPipe.empty()) {
        return {};
    }

    FILE *fifo = fopen(pipeName.data(), "wb");

    if (fifo == nullptr) {
        return {};
    }

    auto message = createMessage(command, returnPipe, chartFile, userKey);

    if (fwrite(message.data(), 1, message.size(), fifo) == -1) {
        return {};
    }

    fclose(fifo);
    return make_unique<InputStream>(returnPipe);
}
#endif

#ifdef OESENC_WINDOWS

class PipeBuffer : public streambuf
{
public:
    explicit PipeBuffer(HANDLE pipeHandle)
        : m_handle(pipeHandle)
    {
    }

    ~PipeBuffer()
    {
        if (m_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(m_handle);
        }
    }

    int_type underflow()
    {
        DWORD bytesRead = 0;
        BOOL success = ReadFile(m_handle, m_buffer.data(), m_buffer.size(), &bytesRead, NULL);
        DWORD lastError = GetLastError();

        if (!success && lastError != ERROR_MORE_DATA) {
            return traits_type::eof();
        }

        char *charBuffer = reinterpret_cast<char *>(m_buffer.data());
        setg(charBuffer, charBuffer, charBuffer + bytesRead);

        return traits_type::to_int_type(static_cast<char>(m_buffer[0]));
    }

private:
    array<std::byte, 1024 * 1024> m_buffer;
    HANDLE m_handle = INVALID_HANDLE_VALUE;
};

class PipeStream : public istream
{
public:
    PipeStream(HANDLE handle)
        : istream(new PipeBuffer(handle))
    {
    }
    ~PipeStream()
    {
        delete rdbuf();
    }
};

unique_ptr<istream> ServerReader::request(string_view pipeName,
                                          Command command,
                                          string_view chartFile,
                                          string_view userKey)
{
    if (pipeName.empty()) {
        return {};
    }

    auto message = createMessage(command, {}, chartFile, userKey);

    string serverPipe = string(R"(\\.\pipe\)") + string(pipeName);

    HANDLE pipeHandle = CreateFile(serverPipe.c_str(), GENERIC_READ | GENERIC_WRITE,
                                   0, NULL, OPEN_EXISTING, 0, NULL);

    if (pipeHandle == INVALID_HANDLE_VALUE) {
        return {};
    }

    DWORD bytesWritten = 0;

    bool success = WriteFile(pipeHandle, message.data(), message.size(), &bytesWritten, NULL);

    if (!success) {
        return {};
    }

    return make_unique<PipeStream>(pipeHandle);
}
#endif

unique_ptr<istream> ServerReader::openOesu(string_view pipeName, string_view chartFile, string_view key)
{
    return request(pipeName, Command::ReadOesu, chartFile, key);
}

unique_ptr<istream> ServerReader::openOesenc(string_view pipeName, string_view chartFile, string_view key)
{
    return request(pipeName, Command::ReadEsenc, chartFile, key);
}
