#pragma once

#include <vector>

#include "polygon.h"

#include "oesenc_export.h"

class DepthArea
{
public:
    OESENC_EXPORT DepthArea(float depth, const std::vector<Polygon> &polygons);
    OESENC_EXPORT std::vector<Polygon> polygons() const;
    OESENC_EXPORT float depth() const;

private:
    float m_depth = 0;
    std::vector<Polygon> m_polygons;
};
