#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable
{
public:
	Sphere() {}
	Sphere(Point3 cen, double radius, std::shared_ptr<Material> mat) : center(cen), radius(radius), material(mat) {};

	virtual bool IsHit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const override;

private:
	Point3 center;
	double radius;
	std::shared_ptr<Material> material;
};

#endif