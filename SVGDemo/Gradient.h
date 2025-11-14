#ifndef GRADIENT_H
#define GRADIENT_H

#include "stdafx.h"
#include <gdiplus.h>
#include <vector>
#include <string>

class GradientStop {
private:
	float offset;
	Gdiplus::Color Color;
	float opacity;
public:
	GradientStop();
	GradientStop(float, Gdiplus::Color, float);
};

class LinearGradient {
private:
	std::string id;
	std::vector<GradientStop> stops;
	float x1;
	float y1;
	float x2;
	float y2;
public:
	LinearGradient();
	LinearGradient(std::string, float, float, float, float);

	void addStop(const GradientStop&);
};

class RadialGradient {
private:
	std::string id;
	std::vector<GradientStop> stops;

	float cx;
	float cy;
	float r;
	float fx;
	float fy;

public:
	RadialGradient();
	RadialGradient(std::string, float, float, float, float, float);
	
	void addStop(const GradientStop&);
};
#endif // !GRADIENT_H
