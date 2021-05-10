#include <cmath>

#include "oesenc/opencpn.h"

// https://github.com/OpenCPN/OpenCPN/blob/master/src/georef.cpp

static const double WGS84_semimajor_axis_meters = 6378137.0;
static const double mercator_k0 = 0.9996;

#define DEGREE (M_PI / 180.0)

Position OpenCPN::fromSimpleMercator(double x,
                                     double y,
                                     const Position &reference)
{
    const double z = WGS84_semimajor_axis_meters * mercator_k0;

    // lat = arcsin((e^2(y+y0) - 1)/(e^2(y+y0) + 1))
    /*
      double s0 = sin(lat0 * DEGREE);
      double y0 = (.5 * log((1 + s0) / (1 - s0))) * z;

    double e = exp(2 * (y0 + y) / z);
    double e11 = (e - 1)/(e + 1);
    double lat2 =(atan2(e11, sqrt(1 - e11 * e11))) / DEGREE;
    */
    //    which is the same as....

    double lat0 = reference.latitude();
    double lon0 = reference.longitude();

    const double s0 = sin(lat0 * DEGREE);
    const double y0 = (.5 * log((1 + s0) / (1 - s0))) * z;

    double lat = (2.0 * atan(exp((y0 + y) / z)) - M_PI / 2.) / DEGREE;

    // lon = x + lon0
    double lon = lon0 + (x / (DEGREE * z));

    return Position(lat, lon);
}
