#include "stdafx.h"
#include "Ellipse.h"
#include "Color.h"

ellipse::ellipse() {
	cx = cy = rx = ry = 0;
}

void ellipse::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

	if (transform)
	{
		transform->Apply(graphics);
	}

	//SolidBrush* brush = createFillBrush();
	//graphics.FillEllipse(brush, cx - rx, cy - ry, 2 * rx, 2 * ry);

	RectF bounds = getBounds();

	Brush* brush = createFillGradientBrush(getBounds());
	graphics.FillEllipse(brush, bounds.X, bounds.Y, bounds.Width, bounds.Height);

	if (stroke_width != 0) {
		Pen* pen = createStrokeGradientBrush(bounds);
		//graphics.DrawEllipse(pen, cx - rx, cy - ry, 2 * rx, 2 * ry);
		graphics.DrawEllipse(pen, bounds.X, bounds.Y, bounds.Width, bounds.Height);
		delete pen;
	}

	delete brush;

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

RectF ellipse::getBounds() const {
	return RectF(cx - rx, cy - ry, 2 * rx, 2 * ry);
}