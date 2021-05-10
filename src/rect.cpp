#include "oesenc/rect.h"

Rect::Rect()
{
}

Rect::Rect(const Rect &rect)
    : m_topLeft(rect.topLeft())
    , m_bottomRight(rect.bottomRight())
    , m_valid(true)
{
}

Rect::Rect(const Position &topLeft, const Position &bottomRight)
    : m_topLeft(topLeft)
    , m_bottomRight(bottomRight)
    , m_valid(true)
{
}

Rect &Rect::operator=(const Rect &rect)
{
    m_topLeft = rect.m_topLeft;
    m_bottomRight = rect.m_bottomRight;
    m_valid = rect.m_valid;
    return *this;
}

bool Rect::intersects(const Rect &rect) const
{
    if (left() < rect.right() && right() > rect.left() && top() > rect.bottom() && bottom() < rect.top()) {
        return true;
    }

    return false;
}

bool Rect::contains(const Position &position) const
{
    if (position.longitude() >= left() && position.longitude() <= right() && position.latitude() < top() && position.latitude() > bottom()) {
        return true;
    }

    return false;
}

double Rect::right() const
{
    return m_bottomRight.longitude();
}

double Rect::left() const
{
    return m_topLeft.longitude();
}

double Rect::top() const
{
    return m_topLeft.latitude();
}

double Rect::bottom() const
{
    return m_bottomRight.latitude();
}

void Rect::setLeft(float longitude)
{
    m_topLeft.setLongitude(longitude);
}

void Rect::setRight(float longitude)
{
    m_bottomRight.setLongitude(longitude);
}

void Rect::setTop(float latitude)
{
    m_topLeft.setLatitude(latitude);
}
void Rect::setBottom(float latitude)
{
    m_bottomRight.setLatitude(latitude);
}

Position Rect::topLeft() const
{
    return m_topLeft;
}

Position Rect::bottomRight() const
{
    return m_bottomRight;
}

Position Rect::center() const
{
    return Position((m_topLeft.latitude() + m_bottomRight.latitude()) / 2.,
                    (m_topLeft.longitude() + m_bottomRight.longitude()) / 2.);
}
