#pragma once

#include "position.h"
#include <vector>

class Polygon
{
public:
    Polygon();
    Polygon(const std::vector<Position> &positions);
    std::vector<Position> positions() const;
    void addPosition(const Position &position);

private:
    std::vector<Position> m_positions;
};
