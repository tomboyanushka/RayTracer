#pragma once
#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct HitRecord
{
	Point3 p;
	Vec3 normal;
	double t;
	bool frontFace;
	inline void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
	{
		frontFace = Dot(ray.Direction(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual bool IsHit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const = 0;
};

#endif