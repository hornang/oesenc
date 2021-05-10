#pragma once

#include "position.h"
#include <vector>

class Line
{
public:
    Line() { }
    void addLine(const std::vector<Position> &line);
    std::vector<std::vector<Position>> lines() const;

private:
    std::vector<std::vector<Position>> m_lines;
};
