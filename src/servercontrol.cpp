#include <chrono>
#include <filesystem>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <thread>

#ifdef OESENC_LINUX
#include <cassert>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

#ifdef OESENC_WINDOWS
#include <windows.h>
#endif

#include <oesenc/servercontrol.h>
#include <oesenc/serverreader.h>

#include "random.h"

using namespace oesenc;
using namespace std;

namespace {

// Linux version of oexserverd does not support the -p parameter and will
// always listen on /tmp/OCPN_PIPEX
constexpr string_view serverPipeLinux = "/tmp/OCPN_PIPEX";
constexpr chrono::duration serverExitTimeout = 5000ms;

#ifdef OESENC_WINDOWS

optional<PROCESS_INFORMATION> startOexserverd(const string &pipeName)
{
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInfo;
    memset(&startupInfo, 0, sizeof(STARTUPINFO));
    startupInfo.cb = sizeof(STARTUPINFO);
    memset(&processInfo, 0, sizeof(PROCESS_INFORMATION));

    const string commandLine = string("oexserverd.exe") + string(" -p " + pipeName);

    BOOL result = CreateProcess(nullptr,
                                const_cast<char *>(commandLine.c_str()),
                                NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL,
                                NULL, &startupInfo, &processInfo);

    if (result == FALSE) {
        return {};
    }

    CloseHandle(processInfo.hThread);

    return processInfo;
}

#endif

#ifdef OESENC_LINUX
bool isPipeOpenedByReader(string_view pipeName)
{
    int fd = open(pipeName.data(), O_WRONLY | O_NONBLOCK);

    if (fd == -1) {
        return false;
    }

    close(fd);
    return true;
}
#endif

string readUntilEof(unique_ptr<istream> &stream)
{
    return string((istreambuf_iterator<char>(*stream.get())),
                  istreambuf_iterator<char>());
}

bool requestServerToExit(string_view pipeName)
{

#ifdef OESENC_LINUX
    if (!isPipeOpenedByReader(pipeName)) {
        return false;
    }
#endif

    auto stream = ServerReader::request(pipeName, ServerReader::Command::Exit, {}, {});
    if (!stream) {
        return false;
    }
    auto reply = readUntilEof(stream);
    return true;
}

}
#ifdef OESENC_LINUX

ServerControl::ServerControl(Flags flags, string_view pipeName)
    : m_leaveServerRunning(flags & Flags(Flags::LeaveServerRunning))
    , m_pipeName(serverPipeLinux)
{
    assert(pipeName.empty() || pipeName == serverPipeLinux);

    if (flags & Flags(Flags::DontStartServer)) {
        return;
    }

    bool success = isReady();

    if (success) {
        return;
    }

    m_oexserverdPid = fork();

    if (m_oexserverdPid == -1) {
        // Failed to fork
    }

    if (m_oexserverdPid == 0) {
        execlp("oexserverd", "oexserverd", NULL);
        exit(1);
    }
}
#endif

#ifdef OESENC_WINDOWS

ServerControl::ServerControl(Flags flags, string_view pipeName)
    : m_leaveServerRunning(flags & Flags(Flags::LeaveServerRunning))
    , m_pipeName(pipeName)
{
    if (flags & Flags(Flags::DontStartServer)) {
        return;
    }
#include <sys/stat.h>

    if (m_pipeName.empty()) {
        m_pipeName = randomAlphaNumericString(10);
    }

    bool success = isReady();

    if (success) {
        return;
    }

    auto processInformation = startOexserverd(m_pipeName);
    if (processInformation.has_value()) {
        m_processInfo.reset(new PROCESS_INFORMATION());
        std::memcpy(m_processInfo.get(), &processInformation.value(), sizeof(PROCESS_INFORMATION));
    }
}
#endif

bool ServerControl::isReady()
{
    if (m_pipeName.empty()) {
        return false;
    }

#ifdef OESENC_LINUX
    if (!isPipeOpenedByReader(m_pipeName)) {
        return false;
    }
#endif

    auto stream = ServerReader::request(m_pipeName, ServerReader::Command::TestAvailability, {}, {});

    if (!stream) {
        return false;
    }

    auto reply = readUntilEof(stream);

    if (reply != "OK" && reply != "OK?") {
        return false;
    }

    return true;
}

ServerControl::~ServerControl()
{
    if (!m_leaveServerRunning) {
        requestServerToExit(m_pipeName);

#ifdef OESENC_LINUX
        if (m_oexserverdPid > 0) {
            // On Linux oexserverd spawns its own child so our fork have already
            // exited. This wait call is probably useless.
            wait(nullptr);
        }
#endif
#ifdef OESENC_WINDOWS
        if (m_processInfo) {
            WaitForSingleObject(m_processInfo->hProcess, serverExitTimeout.count());
            CloseHandle(m_processInfo->hProcess);
        }
#endif
    }
}

bool ServerControl::waitUntilReady(chrono::seconds timeout)
{
    chrono::duration checkInterval = 200ms;
    chrono::duration passed = 0ms;

    while (passed < timeout) {
        if (isReady()) {
            return true;
        }
        passed += chrono::duration_cast<chrono::milliseconds>(checkInterval);
        this_thread::sleep_for(checkInterval);
    }

    return false;
}

string_view ServerControl::pipeName() const
{
    return m_pipeName;
}
