#pragma once

#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "position.h"

#include "oesenc_export.h"

namespace oesenc {

class S57
{
public:
    class VectorEdge
    {
    public:
        VectorEdge() = default;
        VectorEdge(const VectorEdge &other) = delete;
        void setPositions(const std::vector<oesenc::Position> &positions)
        {
            m_points = positions;
        }

        void setPoints(float *points, int nPoints)
        {
            for (int i = 0; i < nPoints; i++) {
                oesenc::Position position(points[i * 2 + 1], points[i * 2]);
                m_points.push_back(position);
            }
        }
        const std::vector<oesenc::Position> &positions() const
        {
            return m_points;
        }

    private:
        std::vector<oesenc::Position> m_points;
    };

    class ConnectedNode
    {
    public:
        ConnectedNode() = default;
        ConnectedNode(const ConnectedNode &other) = default;
        ConnectedNode(oesenc::Position position)
            : m_position(position)
        {
        }
        const oesenc::Position &position() const { return m_position; }

    private:
        oesenc::Position m_position;
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
        ColourPattern,
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
        oesenc::Position position;
        double value = 0;
    };

    using MultiGeometry = std::vector<oesenc::Position>;

    S57(Type type);
    static Type fromTypeCode(int typeCode);
    static Attribute attributeFromTypeCode(int typeCode);
    void setAttribute(Attribute attribute, std::variant<uint32_t, float, std::string> value);

    template <typename T>
    std::optional<T> attribute(Attribute attribute) const;

    void buildGeometry(const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                       const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    void setLineGeometry(LineElement *elements, int length);
    void setPointGeometry(const oesenc::Position &position);
    void setMultiPointGeometry(std::vector<PointGeometry> points);

    OESENC_EXPORT std::optional<oesenc::Position> pointGeometry() const;
    OESENC_EXPORT std::vector<PointGeometry> multiPointGeometry() const { return m_multiPointGeometry; }
    OESENC_EXPORT std::vector<MultiGeometry> polygons() const { return m_polygons; }
    OESENC_EXPORT const std::vector<MultiGeometry> &lines() const { return m_lines; }
    OESENC_EXPORT S57::Type type() const;

    template <typename T>
    static std::vector<T> buildGeometries(const std::vector<LineElement> &lineElements,
                                          const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                                          const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);

private:
    void buildLine(const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                   const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    void buildArea(const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                   const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    std::vector<LineElement> m_lineElements;
    std::vector<MultiGeometry> m_lines;
    Type m_type = Type::Unknown;
    std::vector<MultiGeometry> m_polygons;
    std::vector<PointGeometry> m_multiPointGeometry;
    std::optional<oesenc::Position> m_pointGeometry;
    std::unordered_map<Attribute, std::variant<uint32_t, float, std::string>> m_attributes;
};

};
std::ostream &operator<<(std::ostream &os, const oesenc::S57::LineElement &e);
