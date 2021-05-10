#include "oesenc/deptharea.h"
#include <iostream>

DepthArea::DepthArea(float depth, const std::vector<Polygon> &polygons)
    : m_depth(depth)
    , m_polygons(polygons)
{
}

std::vector<Polygon> DepthArea::polygons() const
{
    return m_polygons;
}

float DepthArea::depth() const
{
    return m_depth;
}
