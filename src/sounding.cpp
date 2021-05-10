#include "oesenc/sounding.h"

Sounding::Sounding(const std::vector<Depth> &depths)
    : m_depths(depths)
{
}

std::vector<Depth> Sounding::depths() const
{
    return m_depths;
}
