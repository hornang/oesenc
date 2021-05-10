#include "oesenc/area.h"
#include <iostream>

Area::Area()
{
}

Area::Area(const std::vector<Polygon> &polygons)
    : m_polygons(polygons)
{
}

void Area::clear()
{
    m_polygons.clear();
}

std::vector<Polygon> Area::polygons() const
{
    return m_polygons;
}

Area &Area::operator+=(const Area &area)
{
    auto newPolygons = area.polygons();
    m_polygons.insert(m_polygons.end(), newPolygons.begin(), newPolygons.end());
    return *this;
}

void Area::addPolygon(const Polygon &polygon)
{
    m_polygons.push_back(polygon);
}

bool Area::isEmpty() const
{
    return m_polygons.empty();
}
