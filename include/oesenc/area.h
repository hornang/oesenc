#pragma once

#include "polygon.h"
#include <vector>

#include "oesenc_export.h"

class Area
{
public:
    OESENC_EXPORT Area();
    OESENC_EXPORT Area(const std::vector<Polygon> &polygons);
    OESENC_EXPORT void addPolygon(const Polygon &polygon);
    OESENC_EXPORT bool isEmpty() const;
    OESENC_EXPORT void clear();
    OESENC_EXPORT std::vector<Polygon> polygons() const;
    OESENC_EXPORT Area &operator+=(const Area &area);

private:
    std::vector<Polygon> m_polygons;
};
