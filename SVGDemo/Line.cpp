#include "stdafx.h"
#include "Line.h"
#include "Color.h"

line::line() {
	x1 = y1 = x2 = y2 = 0;
}

void line::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

	if (transform)
	{
		transform->Apply(graphics);
	}

	RectF bounds = getBounds();
	Pen* pen = createStrokeGradientBrush(bounds);

	graphics.DrawLine(pen, x1, y1, x2, y2);

	delete pen;
	graphics.Restore(state);
}

void line::setX1(float x1) {
	this->x1 = x1;
}
void line::setY1(float y1) {
	this->y1 = y1;
}
void line::setX2(float x2) {
	this->x2 = x2;
}
void line::setY2(float y2) {
	this->y2 = y2;
}

RectF line::getBounds() const {
	float left = min(x1, x2);
	float top = min(y1, y2);
	float right = max(x1, x2);
	float bottom = max(y1, y2);
	return RectF(left, top, right - left, bottom - top);
}