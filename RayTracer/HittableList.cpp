#include "HittableList.h"

bool HittableList::IsHit(const Ray& ray, double tMin, double tMax, HitRecord& record) const
{
	HitRecord tempRecord;
	bool hitAnything = false;
	auto closestSoFar = tMax;

	for (const auto& object : objects)
	{
		if (object->IsHit(ray, tMin, closestSoFar, tempRecord))
		{
			hitAnything = true;
			closestSoFar = tempRecord.t;
			record = tempRecord;
		}
	}

	return hitAnything;
}
