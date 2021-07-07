#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "Utility.h"

#include <iostream>

void WriteColor(std::ostream& out, Color pixelColor, int samplesPerPixel, double gamma)
{
	auto r = pixelColor.x();
	auto g = pixelColor.y();
	auto b = pixelColor.z();

	// Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
	if (r != r) r = 0.0;
	if (g != g) g = 0.0;
	if (b != b) b = 0.0;

	// Divide the color by the number of samples and gamma-correct for a gamma value of 2.0
	auto scale = 1.0 / samplesPerPixel;
	r = pow((scale * r), (1 / gamma));
	g = pow((scale * g), (1 / gamma));
	b = pow((scale * b), (1 / gamma));

	// Write out the translated [0,255] value of each color component.
	out << static_cast<int>(256 * Clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(b, 0.0, 0.999)) << '\n';
}

#endif