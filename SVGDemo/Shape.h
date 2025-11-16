#ifndef SHAPE_H
#define SHAPE_H

#include "stdafx.h"
#include "Gradient.h"
#include <string>
#include <map>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


class shape {
protected:
	string id;
	Color stroke_color;
	Color fill_color;
	float stroke_opacity;
	float fill_opacity;
	float stroke_width;
public:
	shape();
	virtual void draw(Graphics&) = 0;
};

#endif //SHAPE_H