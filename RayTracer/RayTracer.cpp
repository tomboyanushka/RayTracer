///Ray Tracing in One Weekend
///Peter Shirley
#include <Windows.h>
#include <iostream>
#include <sstream>
#include "Vec3.h"
#include "Color.h"
#include "Ray.h"

#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

Color RayColor(const Ray& r)
{
    Vec3 unitDirection = UnitVector(r.Direction());
    auto t = 0.5f * (unitDirection.y() + 1.0f);
    return (1.0f - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
}

int main() {

    // Image Size
    const auto aspectRatio = 16.0f / 9.0f;
    const int imageWidth = 400;
    const int imageHeight = static_cast<int>(imageWidth / aspectRatio);

    // Camera
    auto viewportHeight = 2.0;
    auto viewportWidth = aspectRatio * viewportHeight;
    auto focalLength = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vec3(viewportWidth, 0, 0);
    auto vertical = Vec3(0, viewportHeight, 0);
    auto lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focalLength);


    // Render the image
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = imageHeight - 1; j >= 0; --j)
    {
        DBOUT("\rScanlines remaining: " << j);
        for (int i = 0; i < imageWidth; ++i)
        {
			auto u = double(i) / (imageWidth - 1);
			auto v = double(j) / (imageHeight - 1);
            Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
			Color pixelColor = RayColor(r);
            WriteColor(std::cout, pixelColor);
        }
    }
    DBOUT("\nDone.\n");
}
