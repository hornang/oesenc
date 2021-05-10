#pragma once

#include <cmath>

#include <iostream>

class Position
{
public:
    Position() { }
    Position(double latitude, double longitude);
    double latitude() const;
    double longitude() const;
    void setLatitude(double latitude);
    void setLongitude(double longitude);
    bool isValid() const { return m_valid; }
    bool operator!=(const Position &position) const;

private:
    /// North/South coordinate in degrees
    double m_latitude = 0;

    /// East/West coordinate in degrees
    double m_longitude = 0;

    bool m_valid = false;
};

std::ostream &operator<<(std::ostream &os, const Position &position);
