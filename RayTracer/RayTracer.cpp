///Ray Tracing in One Weekend
///Peter Shirley
#include <Windows.h>
#include <iostream>
#include <sstream>

#include "Camera.h"
#include "Color.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Utility.h"

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
        Point3 target = record.p + record.normal + Random_inUnitSphere();
        return 0.5f * RayColor(Ray(record.p, target - record.p), world, depth - 1);
	}

	Vec3 unitDirection = UnitVector(ray.Direction());
	auto t = 0.5f * (unitDirection.y() + 1.0f);
	return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
}

int main() {

    // Image Size
    const auto aspectRatio = 16.0f / 9.0f;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;
    const double gamma = 2.4;

    // World
    HittableList world;
    world.Add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.Add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    // Camera
    Camera cam;

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
