#pragma once
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable
{
public:
	HittableList() {}
	HittableList(shared_ptr<Hittable> object)
	{
		Add(object); 
	}

	void Clear()
	{
		objects.clear();
	}
	void Add(shared_ptr<Hittable> object)
	{
		objects.push_back(object);
	}

	virtual bool IsHit(const Ray& ray, double tMin, double tMax, HitRecord& record) const override;

private:
	std::vector<shared_ptr<Hittable>> objects;
};
#endif