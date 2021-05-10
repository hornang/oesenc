#pragma once

#include "oesenc/position.h"

namespace OpenCPN {
Position fromSimpleMercator(double x,
                            double y,
                            const Position &reference);
}
