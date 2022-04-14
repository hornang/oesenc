#pragma once

#include <cmath>

#include <iostream>

#include "oesenc_export.h"

namespace oesenc {

class OESENC_EXPORT Position
{
public:
    Position() { }
    Position(double latitude, double longitude);
    double latitude() const;
    double longitude() const;
    void setLatitude(double latitude);
    void setLongitude(double longitude);
    bool operator==(const Position &position) const;
    bool operator!=(const Position &position) const;

private:
    /// North/South coordinate in degrees
    double m_latitude = 0;

    /// East/West coordinate in degrees
    double m_longitude = 0;
};

};

std::ostream &operator<<(std::ostream &os, const oesenc::Position &position);
