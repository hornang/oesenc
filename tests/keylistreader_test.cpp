#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <oesenc/keylistreader.h>
#include <sstream>

using namespace oesenc;
using namespace std;

TEST(testKeyListReader, ReadOesuKeyList)
{
    string keyListXMLData = R"(<?xml version="1.0" ?>
<keyList>
    <Chart>
        <FileName>file1</FileName>
        <RInstallKey>key1</RInstallKey>
    </Chart>
    <Chart>
        <FileName>file2</FileName>
        <RInstallKey>key2</RInstallKey>
    </Chart>
</keyList>)";

    filesystem::path dataDir("data");
    filesystem::create_directory(dataDir);
    std::ofstream keyListFile(dataDir / "keyList.xml");
    keyListFile << keyListXMLData;
    keyListFile.close();

    auto keys = KeyListReader::readOesuKeys(dataDir.string());

    ASSERT_EQ(keys.size(), 2);
    ASSERT_EQ(keys["file1"], "key1");
    ASSERT_EQ(keys["file2"], "key2");
}

TEST(testKeyListReader, ReadOesencKey)
{
    const string chartInfoData = R"(UserKey:keyvalue

otherKey1:otherValue1

otherKey2:otherValue2

)";

    filesystem::path dataDir("data");
    filesystem::create_directory(dataDir);
    std::ofstream keyListFile(dataDir / "Chartinfo.txt");
    keyListFile << chartInfoData;
    keyListFile.close();

    auto key = KeyListReader::readOesencKey(dataDir.string());
    ASSERT_EQ(key, "keyvalue");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
