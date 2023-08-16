#include <array>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <oesenc/cryptreader.h>

using namespace oesenc;
using namespace std;

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
