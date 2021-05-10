#pragma once

#include <vector>

#include "depth.h"

class Sounding
{
public:
    Sounding(const std::vector<Depth> &depths);
    std::vector<Depth> depths() const;

private:
    std::vector<Depth> m_depths;
};
