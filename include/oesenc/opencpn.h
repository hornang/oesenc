#pragma once

#include "oesenc/position.h"

namespace OpenCPN {
oesenc::Position fromSimpleMercator(double x,
                                    double y,
                                    const oesenc::Position &reference);
}
