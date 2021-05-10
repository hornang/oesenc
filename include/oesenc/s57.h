#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
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
        std::vector<Position> &positions()
        {
            return m_points;
        }

    private:
        std::vector<Position> m_points;
    };

    class ConnectedNode
    {
    public:
        ConnectedNode() { }
        ConnectedNode(Position position)
            : m_position(position)
        {
        }
        Position position() const { return m_position; }

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
        Unknown = 0,
        LandArea,
        DepthArea,
        CoastLine,
        BuiltUpArea,
        Sounding,
    };

    enum class Attribute {
        Unknown = 0,
        DepthValue1 = 1,
    };

    S57(Type type);
    static Type fromTypeCode(int typeCode);
    static Attribute attributeFromTypeCode(int typeCode);
    void buildGeometry(const std::unordered_map<int, std::shared_ptr<S57::VectorEdge>> &vectorEdges,
                       const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    void setLineGeometry(LineElement *elements, int length);
    void setDepths(const std::vector<Depth> &depths);
    void setDepth(float depth);
    float depth() const;
    std::vector<Depth> depths() const;
    std::vector<Polygon> polygons() const;
    Area area() const;
    Line line() const;
    Type type() const;

private:
    void buildLine(const std::unordered_map<int, std::shared_ptr<S57::VectorEdge>> &vectorEdges,
                   const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    void buildArea(const std::unordered_map<int, std::shared_ptr<S57::VectorEdge>> &vectorEdges,
                   const std::unordered_map<int, S57::ConnectedNode> &connectedNodes);
    std::unordered_map<unsigned int, LineElement> m_lineElements;
    Area m_area;
    Line m_line;
    float m_depth = 0;
    Type m_type = Type::Unknown;
    std::vector<Polygon> m_polygons;
    std::vector<Depth> m_depths;
};

std::ostream &operator<<(std::ostream &os, const S57::LineElement &e);
