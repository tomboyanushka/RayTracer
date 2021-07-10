#ifndef MATERIAL_H
#define MATERIAL_H

#include "Utility.h"

struct HitRecord;

class Material
{
public:
	virtual bool Scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	Lambertian(const Color& a) : albedo(a) {}

	virtual bool Scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		auto scatterDirection = record.normal + RandomUnitVector();
		// Catch degenerate scatter direction
		if (scatterDirection.NearZero())
		{
			scatterDirection = record.normal;
		}

		scattered = Ray(record.p, scatterDirection);
		attenuation = albedo;
		return true;
	}
private:
	Color albedo;
};

class Metal : public Material
{
public:
	Metal(const Color& a, double fuzz) : albedo(a), fuzziness(fuzz < 1 ? fuzz : 1) {}

	virtual bool Scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		Vec3 reflected = Reflect(UnitVector(rayIn.Direction()), record.normal);
		scattered = Ray(record.p, reflected + fuzziness * Random_inUnitSphere());
		attenuation = albedo;
		return Dot(scattered.Direction(), record.normal) > 0;
	}

private:
	Color albedo;
	double fuzziness;
};

class Dielectric : public Material
{
	// An interesting and easy trick with dielectric spheres is to note that if you use a negative radius, 
	// the geometry is unaffected, but the surface normal points inward.This can be used as a bubble to make a hollow glass sphere.
public:
	Dielectric(double indexOfRefr) : indexOfRefraction(indexOfRefr) {}

	virtual bool Scatter(const Ray& rayIn, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double refractionRatio = record.frontFace ? (1.0 / indexOfRefraction) : indexOfRefraction;
		
		Vec3 unitDirection = UnitVector(rayIn.Direction());
		double cosTheta = fmin(Dot(-unitDirection, record.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool cannotRefract = refractionRatio * sinTheta > 1.0;
		Vec3 direction;
		auto reflectance = Reflectance(cosTheta, indexOfRefraction);

		if (cannotRefract || reflectance > RandomDouble())
		{
			direction = Reflect(unitDirection, record.normal);
		}
		else
		{
			direction = Refract(unitDirection, record.normal, refractionRatio);
		}

		scattered = Ray(record.p, direction);
		return true;
	}

private:
	double indexOfRefraction;

	static double Reflectance(double cosine, double refrIndex)
	{
		// Using Schlick's approximation for reflectance
		auto r0 = (1 - refrIndex) / (1 + refrIndex);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};
#endif