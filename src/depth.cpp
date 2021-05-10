#include "oesenc/depth.h"

Depth::Depth(const Position &position, double depth)
    : m_position(position)
    , m_depth(depth)
{
}

Depth::Depth(double latitude, double longitude, double depth)
    : m_position(latitude, longitude)
    , m_depth(depth)
{
}
