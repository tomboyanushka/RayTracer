#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Utility.h"

class Camera
{
public:
	Camera(
		Point3 lookFrom,
		Point3 lookAt,
		Vec3 vUp, // up vector
		double vfov, // Vertical field-of-view in degrees
		double aspectRatio)
	{
		auto theta = DegreesToRadians(vfov);
		auto height = tan(theta / 2);
		auto viewportHeight = 2.0 * height;
		auto viewportWidth = aspectRatio * viewportHeight;

		auto w = UnitVector(lookFrom - lookAt);
		auto u = UnitVector(Cross(vUp, w));
		auto v = Cross(w, u);

		origin = lookFrom;
		horizontal = viewportWidth * u;
		vertical = viewportHeight * v;
		lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - w;
	}

	Ray GetRay(double s, double t) const
	{
		return Ray(origin, lowerLeftCorner + s * horizontal + t * vertical - origin);
	}

private:
	Point3 origin;
	Point3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
};

#endif