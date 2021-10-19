#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "area.h"
#include "depth.h"
#include "line.h"
#include "position.h"

class S57
{
public:
    class VectorEdge
    {
    public:
        VectorEdge() = default;
        VectorEdge(const VectorEdge &other) = delete;
        void setPositions(const std::vector<Position> &positions)
        {
            m_points = positions;
        }

        void setPoints(float *points, int nPoints)
        {
            for (int i = 0; i < nPoints; i++) {
                Position position(points[i * 2 + 1], points[i * 2]);
                m_points.push_back(position);
            }
        }
        const std::vector<Position> &positions() const
        {
            return m_points;
        }

    private:
        std::vector<Position> m_points;
    };

    class ConnectedNode
    {
    public:
        ConnectedNode() = default;
        ConnectedNode(const ConnectedNode &other) = default;
        ConnectedNode(Position position)
            : m_position(position)
        {
        }
        const Position &position() const { return m_position; }

    private:
        Position m_position;
    };

    enum class Direction {
        Forward = 0,
        Reverse
    };

    struct LineElement
    {
        int startConnectedNode = 0;
        int edgeVector = 0;
        int endConnectedNode = 0;
        Direction direction = Direction::Forward;
    };

    enum class Type {
        Unknown,
        AdministrationArea,
        AnchorageArea,
        BeaconIsolatedDanger,
        Beacon,
        BeaconLateral,
        Bridge,
        BuiltUpArea,
        BuoyLateral,
        CableOverhead,
        Canal,
        CautionArea,
        CoastLine,
        ControlPoint,
        DepthArea,
        DepthContour,
        CartographicLine,
        Lake,
        LandArea,
        LandRegion,
        Landmark,
        Light,
        NavigationLine,
        Obstruction,
        Pile,
        PilotBoardingPlace,
        Pipeline,
        Pontoon,
        Railway,
        RecommendedTrack,
        RestrictedArea,
        River,
        Road,
        SeaArea,
        SeabedArea,
        ShorelineConstruction,
        Sounding,
        StraightLineTerritorialSeaBaseline,
        TerritorialSeaArea,
        UnderwaterRock,
        UnsurveyedArea,
        Wreck,
        Coverage,
        NavigationalSystemOfMarks,
        QualityOfData,
    };

    enum class Attribute {
        BeaconShape,
        BuoyShape,
        CategoryOfCoverage,
        CategoryOfLateralMark,
        CategoryOfRoad,
        CategoryOfSpecialPurposeMark,
        Colour,
        DepthValue1,
        Height,
        LightCharacteristic,
        MarkNavigationalSystem,
        NatureOfSurface,
        ObjectName,
        ScaleMin,
        SignalGroup,
        SignalPeriod,
        SourceDate,
        SourceIndication,
        Status,
        ValueOfNominalRange,
        ValueOfSounding,
        WaterLevelEffect,
        Unknown,
    };

    struct PointGeometry
    {
        Position position;
        double value = 0;
    };

    S57(Type type);
    static Type fromTypeCode(int typeCode);
    static Attribute attributeFromTypeCode(int typeCode);
    void setAttribute(Attribute attribute, std::variant<uint32_t, float, std::string> value);

    template <typename T>
    std::optional<T> attribute(Attribute attribute) const;

    void buildGeometry(const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                       const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    void setLineGeometry(LineElement *elements, int length);
    void setPointGeometry(const Position &position);
    void setMultiPointGeometry(std::vector<PointGeometry> points);
    std::optional<Position> pointGeometry() const;
    std::vector<PointGeometry> multiPointGeometry() const { return m_multiPointGeometry; }
    std::vector<Polygon> polygons() const;
    const std::vector<std::vector<Position>> &lines() const { return m_lines; }
    S57::Type type() const;

private:
    void buildLine(const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                   const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    void buildArea(const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                   const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    std::unordered_map<unsigned int, LineElement> m_lineElements;
    std::vector<std::vector<Position>> m_lines;
    Type m_type = Type::Unknown;
    std::vector<Polygon> m_polygons;
    std::vector<PointGeometry> m_multiPointGeometry;
    Position m_pointGeometry;
    std::unordered_map<Attribute, std::variant<uint32_t, float, std::string>> m_attributes;
};

std::ostream &operator<<(std::ostream &os, const S57::LineElement &e);
