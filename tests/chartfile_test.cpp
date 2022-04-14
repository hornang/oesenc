#include <gtest/gtest.h>
#include <oesenc/s57.h>

using namespace oesenc;

TEST(testS57, buildGeometryBasic)
{
    S57 s57(S57::Type::LandArea);

    std::vector<S57::LineElement> lineElements;
    lineElements.push_back({ 0, 0, 1, S57::Direction::Forward });
    lineElements.push_back({ 1, 0, 2, S57::Direction::Forward });
    lineElements.push_back({ 2, 0, 3, S57::Direction::Forward });
    lineElements.push_back({ 3, 0, 0, S57::Direction::Forward });
    s57.setLineGeometry(lineElements.data(), lineElements.size());

    std::unordered_map<int, S57::ConnectedNode> connectedNodes = {
        { 0, { { 0, 0 } } },
        { 1, { { 1, 0 } } },
        { 2, { { 1, 1 } } },
        { 3, { { 0, 1 } } },
    };

    auto result = s57.buildGeometries<S57::MultiGeometry>(lineElements, {}, connectedNodes);
    std::vector<S57::MultiGeometry> correct = { { { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { 0, 0 } } };
    ASSERT_EQ(result, correct);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
