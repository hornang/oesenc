#include "oesenc/position.h"

Position::Position(double latitude, double longitude)
    : m_latitude(latitude)
    , m_longitude(longitude)
    , m_valid(true)
{
}

double Position::latitude() const
{
    return m_latitude;
}

double Position::longitude() const
{
    return m_longitude;
}

void Position::setLatitude(double latitude)
{
    m_latitude = latitude;
}

void Position::setLongitude(double longitude)
{
    m_longitude = longitude;
}

bool Position::operator!=(const Position &position) const
{
    return (m_latitude != position.latitude() || m_longitude != position.longitude());
}

std::ostream &operator<<(std::ostream &os, const Position &position)
{
    os << position.longitude() << "N," << position.latitude() << "E";
    return os;
}
