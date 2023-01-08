#include <gtest/gtest.h>
#include <oesenc/keylistreader.h>

using namespace oesenc;

TEST(testKeyListReader, test)
{
    // Compilation check
    KeyListReader keyListReader;
    keyListReader.load("");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
