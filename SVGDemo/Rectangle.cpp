#include "stdafx.h"
#include "Rectangle.h"
#include "Color.h"

rectangle::rectangle() {
	x = y = width = height = 0;
}

void rectangle::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

	if (transform)
	{
		transform->Apply(graphics);
	}

	//SolidBrush* brush = createFillBrush();
	//graphics.FillRectangle(brush, x, y, width, height);

	Brush* brush = createFillGradientBrush(getBounds());
	graphics.FillRectangle(brush, x, y, width, height);

	if (stroke_width != 0) {
		Pen* pen = createStrokeGradientBrush(getBounds());
		//graphics.DrawRectangle(pen, x, y, width, height);
		graphics.DrawRectangle(pen, x, y, width, height);
		delete pen;
	}

	delete brush;
	graphics.Restore(state);
}

void rectangle::setX(float x) {
	this->x = x;
}

void rectangle::setY(float y) {
	this->y = y;
}

void rectangle::setWidth(float width) {
	this->width = width;
}

void rectangle::setHeight(float height) {
	this->height = height;
}

RectF rectangle::getBounds() const {
	return RectF(x, y, width, height);
}