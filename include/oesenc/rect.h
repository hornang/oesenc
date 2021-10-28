#pragma once
#include "position.h"

#include "oesenc_export.h"

namespace oesenc {

class OESENC_EXPORT Rect
{
public:
    Rect();
    Rect(const Rect &rect);
    Rect(const oesenc::Position &upperLeft, const oesenc::Position &bottomRight);
    Rect &operator=(const Rect &rect);
    bool intersects(const Rect &rect) const;
    bool contains(const oesenc::Position &position) const;
    double longitudeSpan() const;
    double latitudeSpan() const;
    oesenc::Position topLeft() const;
    oesenc::Position bottomRight() const;
    oesenc::Position center() const;
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
    oesenc::Position m_topLeft;
    oesenc::Position m_bottomRight;
    bool m_valid = false;
};

};

std::ostream &operator<<(std::ostream &os, const oesenc::Rect &rect);
