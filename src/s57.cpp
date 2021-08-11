#include <iostream>

#include "oesenc/line.h"
#include "oesenc/s57.h"

std::ostream &operator<<(std::ostream &os, const S57::LineElement &e)
{
    if (e.direction == S57::Direction::Forward) {
        os << " forward: " << e.startConnectedNode << " " << e.edgeVector << " " << e.endConnectedNode << std::endl;
    } else {
        os << " reverse: " << e.startConnectedNode << " " << e.edgeVector << " " << e.endConnectedNode << std::endl;
    }

    return os;
}

S57::S57(Type type)
    : m_type(type)
{
}

S57::Type S57::fromTypeCode(int typeCode)
{
    switch (typeCode) {
    case 42:
        return S57::Type::DepthArea;
    case 71:
        return S57::Type::LandArea;
    case 30:
        return S57::Type::CoastLine;
    case 13:
        return S57::Type::BuiltUpArea;
    case 129:
        return S57::Type::Sounding;
    default:
        return S57::Type::Unknown;
    }
}

S57::Attribute S57::attributeFromTypeCode(int typeCode)
{
    switch (typeCode) {
    case 87:
        return S57::Attribute::DepthValue1;
    default:
        return S57::Attribute::Unknown;
    }
}

S57::Type S57::type() const
{
    return m_type;
}

void S57::setLineGeometry(LineElement *elements, int length)
{
    for (int i = 0; i < length; i++) {
        LineElement e = elements[i];
        m_lineElements[e.startConnectedNode] = elements[i];
    }
}

void S57::setDepths(const std::vector<Depth> &depths)
{
    m_depths = depths;
}

std::vector<Depth> S57::depths() const
{
    return m_depths;
}

void S57::buildGeometry(const std::unordered_map<int, std::shared_ptr<S57::VectorEdge>> &vectorEdges,
                        const std::unordered_map<int, S57::ConnectedNode> &connectedNodes)
{
    if (m_type == S57::Type::DepthArea || m_type == S57::Type::LandArea) {
        buildArea(vectorEdges, connectedNodes);
    } else if (m_type == S57::Type::CoastLine) {
        buildLine(vectorEdges, connectedNodes);
    } else if (m_type == S57::Type::BuiltUpArea) {
        buildArea(vectorEdges, connectedNodes);
    }
}

void S57::buildLine(const std::unordered_map<int, std::shared_ptr<S57::VectorEdge>> &vectorEdges,
                    const std::unordered_map<int, S57::ConnectedNode> &connectedNodes)
{
    if (m_lineElements.empty()) {
        return;
    }

    auto it = m_lineElements.begin();
    std::vector<Position> pos;
    Line line;

    while (it != m_lineElements.end()) {
        const LineElement lineElement = it->second;

        auto connectedNodeIt = connectedNodes.find(lineElement.startConnectedNode);
        if (connectedNodeIt != connectedNodes.end()) {
            pos.push_back(connectedNodeIt->second.position());
        } else {
            std::cerr << "Connected node index " << lineElement.startConnectedNode << " not found" << std::endl;
        }

        if (lineElement.edgeVector != 0) {
            auto vectorEdgeIt = vectorEdges.find(lineElement.edgeVector);
            if (vectorEdgeIt != vectorEdges.end()) {
                std::shared_ptr<S57::VectorEdge> edge = vectorEdgeIt->second;
                const std::vector<Position> &positions = edge->positions();

                if (lineElement.direction == S57::Direction::Reverse) {
                    pos.insert(pos.end(), positions.rbegin(), positions.rend());
                } else {
                    pos.insert(pos.end(), positions.begin(), positions.end());
                }
            } else {
                std::cerr << "Vector edge " << lineElement.edgeVector << " not found" << std::endl;
            }
        }

        m_lineElements.erase(it);
        it = m_lineElements.find(lineElement.endConnectedNode);

        if (it == m_lineElements.end()) {
            it = m_lineElements.begin();
            if (!pos.empty()) {
                line.addLine(pos);
                pos.clear();
            }
        } else {
            std::cerr << "Connected node index " << lineElement.endConnectedNode << " not found" << std::endl;
        }
    }

    line.addLine(pos);
    m_line = line;
}

std::vector<Polygon> S57::polygons() const
{
    return m_polygons;
}

void S57::buildArea(const std::unordered_map<int, std::shared_ptr<S57::VectorEdge>> &vectorEdges,
                    const std::unordered_map<int, S57::ConnectedNode> &connectedNodes)
{
    if (m_lineElements.empty()) {
        return;
    }

    auto it = m_lineElements.begin();
    std::vector<Position> polygonPositions;
    m_polygons.clear();
    Area area;

    while (it != m_lineElements.end()) {
        const LineElement lineElement = it->second;

        auto connectedNodeIt = connectedNodes.find(lineElement.startConnectedNode);
        if (connectedNodeIt != connectedNodes.end()) {
            polygonPositions.push_back(connectedNodeIt->second.position());
        } else {
            std::cerr << "Connected node " << lineElement.startConnectedNode << " not found" << std::endl;
        }

        if (lineElement.edgeVector != 0) {
            auto vectorEdgeIt = vectorEdges.find(lineElement.edgeVector);
            if (vectorEdgeIt != vectorEdges.end()) {
                const std::shared_ptr<S57::VectorEdge> edge = vectorEdgeIt->second;
                const std::vector<Position> &positions = edge->positions();

                if (lineElement.direction == S57::Direction::Reverse) {
                    polygonPositions.insert(polygonPositions.end(), positions.rbegin(), positions.rend());
                } else {
                    polygonPositions.insert(polygonPositions.end(), positions.begin(), positions.end());
                }
            } else {
                std::cerr << "Vector edge " << lineElement.edgeVector << " not found" << std::endl;
            }
        }

        m_lineElements.erase(it);
        it = m_lineElements.find(lineElement.endConnectedNode);

        if (it == m_lineElements.end()) {
            it = m_lineElements.begin();
            area.addPolygon(polygonPositions);
            m_polygons.push_back(Polygon(polygonPositions));
            polygonPositions.clear();
        }
    }

    if (!polygonPositions.empty()) {
        std::cerr << "Left over positions?" << std::endl;
        area.addPolygon(polygonPositions);
    }

    m_area = area;
}

float S57::depth() const
{
    return m_depth;
}

void S57::setDepth(float depth)
{
    m_depth = depth;
}

Area S57::area() const
{
    return m_area;
}

Line S57::line() const
{
    return m_line;
}
