#include "oesenc/polygon.h"
#include <iostream>

using namespace oesenc;

Polygon::Polygon()
{
}

Polygon::Polygon(const std::vector<oesenc::Position> &positions)
    : m_positions(positions)
{
}

void Polygon::addPosition(const Position &position)
{
    m_positions.push_back(position);
}

std::vector<oesenc::Position> Polygon::positions() const
{
    return m_positions;
}
