#include "oesenc/polygon.h"
#include <iostream>

Polygon::Polygon()
{
}

Polygon::Polygon(const std::vector<Position> &positions)
    : m_positions(positions)
{
}

void Polygon::addPosition(const Position &position)
{
    m_positions.push_back(position);
}

std::vector<Position> Polygon::positions() const
{
    return m_positions;
}
