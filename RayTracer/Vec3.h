#pragma once
#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3
{
public:
	Vec3() : e{ 0, 0, 0 } {}
	Vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	Vec3& operator+=(const Vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	Vec3& operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	Vec3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}

	double length() const
	{
		return sqrt(length_squared());
	}

	double length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	bool NearZero() const
	{
		// Return true if the vector is near zero in all dimensions
		const auto s = 1e-8;
		return (fabs(e[0] < s) && fabs(e[1] < s) && fabs(e[2] < s));
	}

public:
	double e[3];
};

// Type aliases for vec3
using Point3 = Vec3;   // 3D point
using Color = Vec3;    // RGB color

// Utility functions
inline std::ostream& operator<<(std::ostream &out, const Vec3 &v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3 &u, const Vec3 &v)
{
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v)
{
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t)
{
	return t * v;
}

inline Vec3 operator/(const Vec3& v, double t)
{
	return (1/t) * v;
}

inline double Dot(const Vec3& u, const Vec3& v)
{
	return	u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vec3 Cross(const Vec3& u, const Vec3& v)
{
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 UnitVector(Vec3 v)
{
	return v / v.length();
}

inline Vec3 Reflect(const Vec3& ray, const Vec3& normal)
{
	return ray - 2 * Dot(ray, normal) * normal;
}

inline Vec3 Refract(const Vec3& uv, const Vec3& normal, double etaOverEtaPrime)
{
	auto cosTheta = fmin(Dot(-uv, normal), 1.0);
	Vec3 rayOutPerpendicular = etaOverEtaPrime * (uv + cosTheta * normal);
	Vec3 rayOutParallel = -sqrt(fabs(1.0 - rayOutPerpendicular.length_squared())) * normal;
	return rayOutPerpendicular + rayOutParallel;
}

#endif