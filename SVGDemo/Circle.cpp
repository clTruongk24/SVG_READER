#include "stdafx.h"
#include "Circle.h"
#include "Color.h"


circle::circle() {
	cx = cy = r = 0;
}

void circle::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

	if (transform) {
		transform->Apply(graphics);
	}

	SolidBrush brush(ColorWithOpacity(fill_color, fill_opacity));
	Pen pen(ColorWithOpacity(stroke_color, stroke_opacity), stroke_width);

	graphics.FillEllipse(&brush, cx - r, cy - r, 2 * r, 2 * r);
	if (stroke_width != 0) {
		graphics.DrawEllipse(&pen, cx - r, cy - r, 2 * r, 2 * r);
	}

	graphics.Restore(state);
}

void circle::setCX(float cx) {
	this->cx = cx;
}

void circle::setCY(float cy) {
	this->cy = cy;
}

void circle::setRadius(float r) {
	this->r = r;
}