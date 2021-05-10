#include "oesenc/line.h"
#include <iostream>

std::vector<std::vector<Position>> Line::lines() const
{
    return m_lines;
}

void Line::addLine(const std::vector<Position> &line)
{
    if (line.empty()) {
        // std::cout << "Not addding empty polygon" << std::endl;
        return;
    }
    m_lines.push_back(line);
}
