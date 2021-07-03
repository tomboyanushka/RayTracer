#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <limits>
#include <memory>

// Common Headers
#include "Ray.h"
#include "Vec3.h"

// Using
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions
inline double DegreesToRadians(double degrees)
{
	return degrees * pi / 180.0;
}

#endif