#pragma once

#include "position.h"
#include <vector>

class Line
{
public:
    OESENC_EXPORT Line() { }
    OESENC_EXPORT void addLine(const std::vector<Position> &line);
    OESENC_EXPORT std::vector<std::vector<Position>> lines() const;

private:
    std::vector<std::vector<Position>> m_lines;
};
