#pragma once

#include "polygon.h"
#include <vector>

class DepthArea
{
public:
    DepthArea(float depth, const std::vector<Polygon> &polygons);
    std::vector<Polygon> polygons() const;
    float depth() const;

private:
    float m_depth = 0;
    std::vector<Polygon> m_polygons;
};
