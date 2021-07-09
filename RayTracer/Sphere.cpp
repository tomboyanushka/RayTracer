#include "Sphere.h"

bool Sphere::IsHit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const
{
	Vec3 oc = ray.Origin() - center;
	auto a = ray.Direction().length_squared();
	auto half_b = Dot(oc, ray.Direction());
	auto c = oc.length_squared() - radius * radius;
	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0)
	{
		return false;
	}

	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range
	auto root = (-half_b - sqrtd) / a;
	if (root < tMin || tMax < root)
	{
		root = (-half_b + sqrtd) / a;
		if (root < tMin || tMax < root)
		{
			return false;
		}
	}

	rec.t = root;
	rec.p = ray.PointAtParamter(rec.t);
	Vec3 outwardNormal = (rec.p - center) / radius;
	rec.SetFaceNormal(ray, outwardNormal);
	rec.material = material;

	return true;
}
