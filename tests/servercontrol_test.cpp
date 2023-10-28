#include <array>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <oesenc/chartfile.h>
#include <oesenc/servercontrol.h>
#include <oesenc/serverreader.h>

using namespace oesenc;
using namespace std;

namespace {
uint16_t readUint16(istream &input)
{
    array<std::byte, 2> data;
    input.read(reinterpret_cast<char *>(data.data()), 2);
    if (!input) {
        throw std::runtime_error("Unable to read all bytes");
    }
    uint16_t value = 0;
    return static_cast<uint16_t>(data[0]) | static_cast<uint16_t>(data[1]) << 8;
}

uint16_t readUint32(istream &input)
{
    array<std::byte, 4> data;
    input.read(reinterpret_cast<char *>(data.data()), 4);
    if (!input) {
        throw std::runtime_error("Unable to read all bytes");
    }
    uint16_t value = 0;
    return static_cast<uint16_t>(data[0])
        | static_cast<uint16_t>(data[1]) << 8
        | static_cast<uint16_t>(data[2]) << 16
        | static_cast<uint16_t>(data[3]) << 24;
}

struct ServerRecord
{
    uint16_t serverStatus = 0;
    uint16_t decryptStatus = 0;
    uint16_t expireStatus = 0;
    uint16_t expireDaysRemaining = 0;
    uint16_t graceDaysAllowed = 0;
    uint16_t graceDaysRemaining = 0;
};

void verifyDefaultServerRecord(istream &input)
{
    uint16_t type = readUint16(input);
    uint32_t length = readUint32(input);

    ASSERT_EQ(type, 200);
    ASSERT_EQ(length, 18);

    ServerRecord record;

    record.serverStatus = readUint16(input);
    record.decryptStatus = readUint16(input);
    record.expireStatus = readUint16(input);
    record.expireDaysRemaining = readUint16(input);
    record.graceDaysAllowed = readUint16(input);
    record.graceDaysRemaining = readUint16(input);
}
}

TEST(ServerControl, TestServerConnection)
{
    // Write 1024 zeros to a dummy chart file
    string dummyChartFile = "test.oesenc";
    std::array<std::byte, 1024> data;
    std::fill(data.begin(), data.end(), std::byte(0));

    std::ofstream file(dummyChartFile);
    file.write((char *)data.data(), data.size());
    file.close();

    ServerControl serverControl;
    ASSERT_TRUE(serverControl.waitUntilReady());
    unique_ptr<istream> inputStream = ServerReader::openOesu(serverControl.pipeName(), dummyChartFile, {});
    ASSERT_TRUE(inputStream);
    verifyDefaultServerRecord(*inputStream);
}

TEST(ServerControl, ReusesServer)
{
    string pipeName;

    {
        ServerControl serverControl(ServerControl::Flags::LeaveServerRunning);
        ASSERT_TRUE(serverControl.waitUntilReady());
        pipeName = serverControl.pipeName();
    }

    ServerControl serverControl(ServerControl::Flags::DontStartServer, pipeName);
    ASSERT_TRUE(serverControl.waitUntilReady());
    ASSERT_EQ(pipeName, serverControl.pipeName());
}

#ifdef OESENC_LINUX
TEST(ServerControl, DontBlockOnDeadPipe)
{
    string pipeName;
    {
        ServerControl serverControl(ServerControl::Flags::DontStartServer
                                    | ServerControl::Flags::LeaveServerRunning);
        pipeName = serverControl.pipeName();
        mkfifo(pipeName.c_str(), 0666);
    }

    ServerControl serverControl(ServerControl::Flags::DontStartServer);

    ASSERT_FALSE(serverControl.isReady());
    remove(pipeName.c_str());
}
#endif

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
