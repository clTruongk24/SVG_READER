#ifndef GRADIENT_H
#define GRADIENT_H

#include "stdafx.h"
#include <gdiplus.h>
#include <vector>
#include <string>

class GradientStop {
private:
	float offset;
	Gdiplus::Color color;
	float opacity;
public:
	GradientStop();
	GradientStop(float, Gdiplus::Color, float);

};

#endif // !GRADIENT_H
