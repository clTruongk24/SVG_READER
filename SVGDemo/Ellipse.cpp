#include "stdafx.h"
#include "Ellipse.h"
#include "Color.h"

ellipse::ellipse() {
	cx = cy = rx = ry = 0;
}

void ellipse::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

	SolidBrush brush(ColorWithOpacity(fill_color, fill_opacity));
	Pen pen(ColorWithOpacity(stroke_color, stroke_opacity), stroke_width);

	graphics.FillEllipse(&brush, cx - rx, cy - ry, 2 * rx, 2 * ry);
	if (stroke_width != 0) {
		graphics.DrawEllipse(&pen, cx - rx, cy - ry, 2 * rx, 2 * ry);
	}

	graphics.Restore(state);
}

void ellipse::setCX(float cx) {
	this->cx = cx;
}

void ellipse::setCY(float cy){
	this->cy = cy;
}

void ellipse::setRX(float rx) {
	this->rx = rx;
}

void ellipse::setRY(float ry) {
	this->ry = ry;
}