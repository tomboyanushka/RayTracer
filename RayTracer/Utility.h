#pragma once
#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <cstdlib>
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

inline double RandomDouble()
{
	// Returns a random real in [0,1).
	auto rand_double = rand() / (RAND_MAX + 1.0);
	return rand_double;
}

inline double RandomDouble(double min, double max)
{
	// Returns a random real in [min,max).
	return min + (max - min) * RandomDouble();
}

inline Vec3 Random()
{
	return Vec3(RandomDouble(), RandomDouble(), RandomDouble());
}

inline static Vec3 Random(double min, double max)
{
	return Vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
}

Vec3 Random_inUnitSphere()
{
	while (true)
	{
		auto p = Random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

inline double Clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

#endif