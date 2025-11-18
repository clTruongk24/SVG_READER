#include "stdafx.h"
#include "Polyline.h"
#include "Color.h"
#include <sstream>


void polyline::draw(Graphics& graphics) {
	if (p.size() < 2) return;

	int size = p.size();
	Point* points = new Point[size];
	for (int i = 0; i < size; i++) {
		points[i] = p[i];
	}

	GraphicsState state = graphics.Save();

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
	stringstream ss(pointStr);
	string token;

	while (ss >> token) {
		stringstream pair(token);
		string xStr, yStr;

		if (getline(pair, xStr, ',') && getline(pair, yStr)) {
			INT x = static_cast<INT>(stof(xStr));
			INT y = static_cast<INT>(stof(yStr));
			Point temp(x, y);
			p.push_back(temp);
		}
	}
}