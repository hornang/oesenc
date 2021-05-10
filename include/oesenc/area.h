#pragma once

#include "polygon.h"
#include <vector>

class Area
{
public:
    Area();
    Area(const std::vector<Polygon> &polygons);
    void addPolygon(const Polygon &polygon);
    bool isEmpty() const;
    void clear();
    std::vector<Polygon> polygons() const;
    Area &operator+=(const Area &area);

private:
    std::vector<Polygon> m_polygons;
};
