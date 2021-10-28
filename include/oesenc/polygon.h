#pragma once

#include "position.h"
#include <vector>

#include "oesenc_export.h"

namespace oesenc {

class Polygon
{
public:
    OESENC_EXPORT Polygon();
    OESENC_EXPORT Polygon(const std::vector<Position> &positions);
    OESENC_EXPORT std::vector<Position> positions() const;
    OESENC_EXPORT void addPosition(const Position &position);

private:
    std::vector<Position> m_positions;
};
};
