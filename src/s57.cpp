#include <iostream>

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
    case 1:
        return S57::Type::AdministrationArea;
    case 4:
        return S57::Type::AnchorageArea;
    case 6:
        return S57::Type::BeaconIsolatedDanger;
    case 9:
        return S57::Type::Beacon;
    case 7:
        return S57::Type::BeaconLateral;
    case 11:
        return S57::Type::Bridge;
    case 13:
        return S57::Type::BuiltUpArea;
    case 17:
        return S57::Type::BuoyLateral;
    case 21:
        return S57::Type::CableOverhead;
    case 22:
        return S57::Type::Canal;
    case 27:
        return S57::Type::CautionArea;
    case 30:
        return S57::Type::CoastLine;
    case 33:
        return S57::Type::ControlPoint;
    case 42:
        return S57::Type::DepthArea;
    case 43:
        return S57::Type::DepthContour;
    case 50:
        return S57::Type::CartographicLine;
    case 69:
        return S57::Type::Lake;
    case 71:
        return S57::Type::LandArea;
    case 73:
        return S57::Type::LandRegion;
    case 74:
        return S57::Type::Landmark;
    case 75:
        return S57::Type::Light;
    case 85:
        return S57::Type::NavigationLine;
    case 86:
        return S57::Type::Obstruction;
    case 90:
        return S57::Type::Pile;
    case 91:
        return S57::Type::PilotBoardingPlace;
    case 94:
        return S57::Type::Pipeline;
    case 95:
        return S57::Type::Pontoon;
    case 106:
        return S57::Type::Railway;
    case 109:
        return S57::Type::RecommendedTrack;
    case 112:
        return S57::Type::RestrictedArea;
    case 114:
        return S57::Type::River;
    case 116:
        return S57::Type::Road;
    case 119:
        return S57::Type::SeaArea;
    case 121:
        return S57::Type::SeabedArea;
    case 122:
        return S57::Type::ShorelineConstruction;
    case 129:
        return S57::Type::Sounding;
    case 132:
        return S57::Type::StraightLineTerritorialSeaBaseline;
    case 135:
        return S57::Type::TerritorialSeaArea;
    case 153:
        return S57::Type::UnderwaterRock;
    case 154:
        return S57::Type::UnsurveyedArea;
    case 159:
        return S57::Type::Wreck;
    case 302:
        return S57::Type::Coverage;
    case 306:
        return S57::Type::NavigationalSystemOfMarks;
    case 308:
        return S57::Type::QualityOfData;
    default:
        return S57::Type::Unknown;
    }
}

void S57::setPointGeometry(const Position &pointGeometry)
{
    m_pointGeometry = pointGeometry;
}

std::optional<Position> S57::pointGeometry() const
{
    return m_pointGeometry;
}

void S57::setAttribute(Attribute attribute, std::variant<uint32_t, float, std::string> value)
{
    m_attributes[attribute] = value;
}

template <typename T>
std::optional<T> S57::attribute(Attribute attribute) const
{
    if (m_attributes.find(attribute) == m_attributes.end()) {
        return {};
    }

    const auto &value = m_attributes.at(attribute);

    if (!std::holds_alternative<T>(value)) {
        return {};
    }

    return std::get<T>(value);
}
template std::optional<float> S57::attribute<float>(Attribute) const;
template std::optional<uint32_t> S57::attribute<uint32_t>(Attribute) const;
template std::optional<std::string> S57::attribute<std::string>(Attribute) const;

S57::Attribute S57::attributeFromTypeCode(int typeCode)
{
    switch (typeCode) {
    case 2:
        return S57::Attribute::BeaconShape;
    case 4:
        return S57::Attribute::BuoyShape;
    case 18:
        return S57::Attribute::CategoryOfCoverage;
    case 36:
        return S57::Attribute::CategoryOfLateralMark;
    case 57:
        return S57::Attribute::CategoryOfRoad;
    case 66:
        return S57::Attribute::CategoryOfSpecialPurposeMark;
    case 75:
        return S57::Attribute::Colour;
    case 87:
        return S57::Attribute::DepthValue1;
    case 95:
        return S57::Attribute::Height;
    case 107:
        return S57::Attribute::LightCharacteristic;
    case 109:
        return S57::Attribute::MarkNavigationalSystem;
    case 113:
        return S57::Attribute::NatureOfSurface;
    case 116:
        return S57::Attribute::ObjectName;
    case 133:
        return S57::Attribute::ScaleMin;
    case 141:
        return S57::Attribute::SignalGroup;
    case 142:
        return S57::Attribute::SignalPeriod;
    case 147:
        return S57::Attribute::SourceDate;
    case 148:
        return S57::Attribute::SourceIndication;
    case 149:
        return S57::Attribute::Status;
    case 178:
        return S57::Attribute::ValueOfNominalRange;
    case 179:
        return S57::Attribute::ValueOfSounding;
    case 187:
        return S57::Attribute::WaterLevelEffect;
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

void S57::setMultiPointGeometry(std::vector<PointGeometry> points)
{
    m_multiPointGeometry = points;
}

void S57::buildGeometry(const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                        const std::unordered_map<int, S57::ConnectedNode> &connectedNodes)
{
    switch (m_type) {
    case S57::Type::LandArea:
    case S57::Type::DepthArea:
    case S57::Type::BuiltUpArea:
    case S57::Type::Coverage:
        buildArea(vectorEdges, connectedNodes);
        break;
    case S57::Type::CoastLine:
    case S57::Type::Road:
        buildLine(vectorEdges, connectedNodes);
        break;
    default:
        break;
    }
}

void S57::buildLine(const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                    const std::unordered_map<int, S57::ConnectedNode> &connectedNodes)
{
    if (m_lineElements.empty()) {
        return;
    }

    auto it = m_lineElements.begin();
    std::vector<Position> line;

    while (it != m_lineElements.end()) {
        const LineElement lineElement = it->second;

        auto connectedNodeIt = connectedNodes.find(lineElement.startConnectedNode);
        if (connectedNodeIt != connectedNodes.end()) {
            line.push_back(connectedNodeIt->second.position());
        } else {
            std::cerr << "Connected node index " << lineElement.startConnectedNode << " not found" << std::endl;
        }

        if (lineElement.edgeVector != 0) {
            auto vectorEdgeIt = vectorEdges.find(lineElement.edgeVector);
            if (vectorEdgeIt != vectorEdges.end()) {
                const S57::VectorEdge &edge = vectorEdgeIt->second;
                const std::vector<Position> &positions = edge.positions();
                if (line.size() == 0) {
                    std::cerr << "No first point?" << std::endl;
                }

                if (lineElement.direction == S57::Direction::Reverse) {
                    line.insert(line.end(), positions.rbegin(), positions.rend());
                } else {
                    line.insert(line.end(), positions.begin(), positions.end());
                }
            } else {
                std::cerr << "Vector edge " << lineElement.edgeVector << " not found" << std::endl;
            }
        }

        m_lineElements.erase(it);
        it = m_lineElements.find(lineElement.endConnectedNode);

        if (it == m_lineElements.end()) {
            it = m_lineElements.begin();

            // Since the line is finished we willl add the end connected node as
            // last point. Consider doing this for the polygon variant also to
            // avoid closing the polygon when painting. Could also generalize the
            // code for polygon and line into one function.
            auto connectedNodeIt = connectedNodes.find(lineElement.endConnectedNode);
            if (connectedNodeIt != connectedNodes.end()) {
                line.push_back(connectedNodeIt->second.position());
            } else {
                std::cerr << "Connected node index " << lineElement.endConnectedNode << " not found" << std::endl;
            }
            m_lines.push_back(line);
            line.clear();
        }
    }

    if (!line.empty()) {
        std::cerr << "Left over positions?" << std::endl;
        m_lines.push_back(line);
    }
}

std::vector<Polygon> S57::polygons() const
{
    return m_polygons;
}

void S57::buildArea(const std::unordered_map<int, S57::VectorEdge> &vectorEdges,
                    const std::unordered_map<int, S57::ConnectedNode> &connectedNodes)
{
    if (m_lineElements.empty()) {
        return;
    }

    auto it = m_lineElements.begin();
    std::vector<Position> polygonPositions;
    m_polygons.clear();

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
                const S57::VectorEdge &edge = vectorEdgeIt->second;
                const std::vector<Position> &positions = edge.positions();

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
            m_polygons.push_back(Polygon(polygonPositions));
            polygonPositions.clear();
        }
    }

    if (!polygonPositions.empty()) {
        std::cerr << "Left over positions?" << std::endl;
        m_polygons.push_back(polygonPositions);
    }
}
