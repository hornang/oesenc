#pragma once

#include "position.h"

#include "oesenc_export.h"

class OESENC_EXPORT Depth
{
public:
    Depth(double latitude, double longitude, double depth);
    Depth(const Position &position, double depth);
    Position position() const { return m_position; }
    double depth() const { return m_depth; }

private:
    Position m_position;
    double m_depth = 0;
};
