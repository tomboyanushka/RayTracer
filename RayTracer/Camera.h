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
		double aspectRatio,
		double aperture,
		double focusDist)
	{
		auto theta = DegreesToRadians(vfov);
		auto height = tan(theta / 2);
		auto viewportHeight = 2.0 * height;
		auto viewportWidth = aspectRatio * viewportHeight;

		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(Cross(vUp, w));
		v = Cross(w, u);

		origin = lookFrom;
		horizontal = focusDist * viewportWidth * u;
		vertical = focusDist * viewportHeight * v;
		lowerLeftCorner = origin - horizontal/2 - vertical/2 - focusDist*w;
		lensRadius = aperture / 2;
	}

	Ray GetRay(double s, double t) const
	{
		Vec3 rd = lensRadius * Random_inUnitDisk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin + offset, lowerLeftCorner + s*horizontal + t*vertical - origin - offset);
	}

private:
	Point3 origin;
	Point3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	double lensRadius;
};

#endif