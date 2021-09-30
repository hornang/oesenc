#pragma once
#include "position.h"

class Rect
{
public:
    Rect();
    Rect(const Rect &rect);
    Rect(const Position &upperLeft, const Position &bottomRight);
    Rect &operator=(const Rect &rect);
    bool intersects(const Rect &rect) const;
    bool contains(const Position &position) const;
    double longitudeSpan() const;
    double latitudeSpan() const;
    Position topLeft() const;
    Position bottomRight() const;
    Position center() const;
    void setRight(float longitude);
    void setLeft(float longitude);
    void setTop(float latitude);
    void setBottom(float latitude);
    double right() const;
    double left() const;
    double top() const;
    double bottom() const;
    bool isValid() const { return m_valid; }

private:
    Position m_topLeft;
    Position m_bottomRight;
    bool m_valid = false;
};
