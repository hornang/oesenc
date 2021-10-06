#pragma once

#include <vector>

#include "depth.h"

#include "oesenc_export.h"

class Sounding
{
public:
    OESENC_EXPORT Sounding(const std::vector<Depth> &depths);
    OESENC_EXPORT std::vector<Depth> depths() const;

private:
    std::vector<Depth> m_depths;
};
