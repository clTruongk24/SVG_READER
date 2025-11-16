#include "stdafx.h"
#include "Shape.h"
#include "Color.h"

using namespace std;
using namespace Gdiplus;

shape::shape() {
	id = "";
	stroke_color = Color(0, 0, 0);
	fill_color = Color(0, 0, 0);
	stroke_opacity = fill_opacity = 1.0f;
	stroke_width = 0;
}

void shape::setID(const string& id) {
	this->id = id;
}
void shape::setStrokeColor(const string& strokeStr) {
	stroke_color = parseColor(strokeStr);
}
void shape::setFillColor(const string& fillStr) {
	fill_color = parseColor(fillStr);
}
void shape::setStrokeOpacity(const string& strokeStr) {
	stroke_opacity = stof(strokeStr);
}
void shape::setFillOpacity(const string& fillStr) {
	fill_opacity = stof(fillStr);
}
void shape::setStrokeWidth(const string& strokeValue) {
	stroke_width = stof(strokeValue);
}