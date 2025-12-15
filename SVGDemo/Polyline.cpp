#include "stdafx.h"
#include "Polyline.h"
#include "Color.h"
#include <sstream>
#include <algorithm>


void polyline::draw(Graphics& graphics) {
	if (p.size() < 2) return;

	int size = p.size();
	Point* points = new Point[size];
	for (int i = 0; i < size; i++) {
		points[i] = p[i];
	}

	GraphicsState state = graphics.Save();

	if (transform) {
		transform->Apply(graphics);
	}

	SolidBrush brush(ColorWithOpacity(fill_color, fill_opacity));
	Pen pen(ColorWithOpacity(stroke_color, stroke_opacity), stroke_width);

	graphics.FillPolygon(&brush, points, size, FillModeWinding);
	if (stroke_width != 0) {
		graphics.DrawLines(&pen, points, size);
	}
	
	
	delete[] points;
	graphics.Restore(state);
}

void polyline::setPoints(const string& pointStr) {
	string temp = pointStr;
	replace(temp.begin(), temp.end(), ',', ' ');

	istringstream iss(temp);
	float x, y;
	while (iss >> x >> y) {
		p.push_back(Point(static_cast<INT>(x), static_cast<INT>(y)));
	}

}