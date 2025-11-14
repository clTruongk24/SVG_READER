#ifndef SHAPE_H
#define SHAPE_H

#include "stdafx.h"
#include <string>
#include <gdiplus.h>

class Shape {
	std::string id;
	Gdiplus::Color stroke_color;
	Gdiplus::Color fill_color;
	float stroke_width;
	Matrix transform;
};

#endif //SHAPE_H