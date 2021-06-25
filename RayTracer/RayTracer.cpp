///Ray Tracing in One Weekend
///Peter Shirley
#include <Windows.h>
#include <iostream>
#include <sstream>
#include "Vec3.h"
#include "Color.h"

#define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

int main() {

    // Image Size
    const int image_width = 256;
    const int image_height = 256;

    // Render the image
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        DBOUT("\rScanlines remaining: " << j);
        for (int i = 0; i < image_width; ++i)
        {
			auto r = double(i) / (image_width - 1);
			auto g = double(j) / (image_height - 1);
			auto b = 0.25;
			Color pixelColor(r, g, b);
			WriteColor(std::cout, pixelColor);
        }
    }
    DBOUT("\nDone.\n");
}
