#include "oesenc/position.h"

using namespace oesenc;

Position::Position(double latitude, double longitude)
    : m_latitude(latitude)
    , m_longitude(longitude)
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

bool Position::operator==(const Position &position) const
{
    return (m_latitude == position.latitude() && m_longitude == position.longitude());
}

bool Position::operator!=(const Position &position) const
{
    return (m_latitude != position.latitude() || m_longitude != position.longitude());
}

std::ostream &operator<<(std::ostream &os, const Position &position)
{
    os << position.latitude() << " " << position.longitude();
    return os;
}
