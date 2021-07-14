///Ray Tracing in One Weekend
///Peter Shirley
#include <Windows.h>
#include <iostream>
#include <sstream>

#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"

#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

Color RayColor(const Ray& ray, const Hittable& world, int depth)
{
	HitRecord record;
	// If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
    {
        return Color(0, 0, 0);
    }
	if (world.IsHit(ray, 0.001, infinity, record))
	{
        Ray scattered;
        Color attenuation;
        if (record.material->Scatter(ray, record, attenuation, scattered))
        {
            return attenuation * RayColor(scattered, world, depth - 1);
        }
        return Color(0, 0, 0);
	}

	Vec3 unitDirection = UnitVector(ray.Direction());
	auto t = 0.5f * (unitDirection.y() + 1.0f);
	return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
}

HittableList Scene()
{
    HittableList world;

    auto groundMat = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, groundMat));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            auto chooseMat = RandomDouble();
            Point3 center(a + 0.9 * RandomDouble(), 0.2, b + 0.9 * RandomDouble());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<Material> sphereMat;

                if (chooseMat < 0.8)
                {
                    // diffuse
                    auto albedo = Random() * Random();
					sphereMat = make_shared<Lambertian>(albedo);
					world.Add(make_shared<Sphere>(center, 0.2, sphereMat));
                }
                else if (chooseMat < 0.95)
                {
                    // Metal
                    auto albedo = Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphereMat = make_shared<Metal>(albedo, fuzz);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMat));
                }
                else
                {
                    // Glass
                    sphereMat = make_shared<Dielectric>(1.5);
                    world.Add(make_shared<Sphere>(center, 0.2, sphereMat));
                }
            }
        }
    }

	auto material1 = make_shared<Dielectric>(1.5);
	world.Add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.Add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.Add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

	return world;
}

int main() {

    // Image Size
    const auto aspectRatio = 3.0f / 2.0f;
    const int imageWidth = 1200;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 500;
    const int maxDepth = 50;
    const double gamma = 2.4;

    // World
    auto world = Scene();

    // Camera
    Point3 lookFrom(13, 2, 3);
    Point3 lookAt(0, 0, 0);
    Vec3 vUp(0, 1, 0);
    auto distToFocus = 10.0;
    auto aperture = 0.1;
    Camera cam(lookFrom, lookAt, vUp, 20, aspectRatio, aperture, distToFocus);

    // Render the image
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j)
    {
        DBOUT("\rScanlines remaining: " << j);
        for (int i = 0; i < imageWidth; ++i)
        {
            Color pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s)
            {
				auto u = (i + RandomDouble()) / (imageWidth - 1);
				auto v = (j + RandomDouble()) / (imageHeight - 1);
                Ray ray = cam.GetRay(u, v);
                pixelColor += RayColor(ray, world, maxDepth);
            }

            WriteColor(std::cout, pixelColor, samplesPerPixel, gamma);
        }
    }
    DBOUT("\nDone.\n");
}
