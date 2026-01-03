#include "stdafx.h"
#include "Polygon.h"
#include "Color.h"
#include <sstream>
#include <algorithm>

void polygon::draw(Graphics& graphics) {
	if (p.size() < 3) return;

	int size = p.size();
	Point* points = new Point[size];
	for (int i = 0; i < size; i++) {
		points[i] = p[i];
	}

	GraphicsState state = graphics.Save();

	if (transform)
	{
		transform->Apply(graphics);
	}

	//SolidBrush* brush = createFillBrush();
	//graphics.FillPolygon(brush, points, size, FillModeWinding);

	RectF bounds = getBounds();

	Brush* brush = createFillGradientBrush(bounds);

	graphics.FillPolygon(brush, points, size, FillModeWinding);

	if (stroke_width != 0) {
		Pen* pen = createStrokeGradientBrush(bounds);
		graphics.DrawPolygon(pen, points, size);
		delete pen;
	}

	delete brush;
	delete[] points;
	graphics.Restore(state);
}

void polygon::setPoints(const string& pointStr) {

	string temp = pointStr;
	replace(temp.begin(), temp.end(), ',', ' ');

	istringstream iss(temp);
	float x, y;
	while (iss >> x >> y) {
		p.push_back(Point(static_cast<INT>(x), static_cast<INT>(y)));
	}
}

RectF polygon::getBounds() const {
	if (p.size() == 0) return RectF();
	INT minX = p[0].X;
	INT minY = p[0].Y;
	INT maxX = p[0].X;
	INT maxY = p[0].Y;
	for (size_t i = 1; i < p.size(); i++) {
		if (p[i].X < minX) minX = p[i].X;
		if (p[i].Y < minY) minY = p[i].Y;
		if (p[i].X > maxX) maxX = p[i].X;
		if (p[i].Y > maxY) maxY = p[i].Y;
	}
	return RectF(static_cast<REAL>(minX),
		static_cast<REAL>(minY),
		static_cast<REAL>(maxX - minX),
		static_cast<REAL>(maxY - minY));
}