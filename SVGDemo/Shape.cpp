#include "stdafx.h"
#include "Shape.h"

using namespace std;
using namespace Gdiplus;

shape::shape() {
	id = "";
	stroke_color = Color(0, 0, 0);
	fill_color = Color(0, 0, 0);
	stroke_opacity = fill_opacity = 1.0f;
	stroke_width = 0;
}