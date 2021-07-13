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

int main() {

    // Image Size
    const auto aspectRatio = 16.0f / 9.0f;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;
    const double gamma = 2.4;

    // World
    auto R = cos(pi / 4);
    HittableList world;

	auto materialGround = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto materialCenter = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	auto materialLeft = make_shared<Dielectric>(1.5);
	auto materialRight = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

	world.Add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, materialGround));
	world.Add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, materialCenter));
	world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, materialLeft));
	world.Add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), -0.45, materialLeft));
	world.Add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, materialRight));

    // Camera
    Point3 lookFrom(3, 3, 2);
    Point3 lookAt(0, 0, -1);
    Vec3 vUp(0, 1, 0);
    auto distToFocus = (lookFrom - lookAt).length();
    auto aperture = 2.0;
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
