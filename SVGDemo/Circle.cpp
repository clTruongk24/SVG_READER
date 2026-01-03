#include "stdafx.h"
#include "Circle.h"
#include "Color.h"
#include "Defs.h"
#include "Gradient.h"

circle::circle()
{
	cx = cy = r = 0;
}

void circle::draw(Graphics &graphics)
{
	GraphicsState state = graphics.Save();

	if (transform)
	{
		transform->Apply(graphics);
	}

	RectF bounds = getBounds();

	Brush *brush = createFillGradientBrush(bounds);
	graphics.FillEllipse(brush, bounds.X, bounds.Y, bounds.Width, bounds.Height);

	// graphics.FillEllipse(brush, cx - r, cy - r, 2 * r, 2 * r);

	if (stroke_width != 0)
	{
		Pen *pen = createStrokeGradientBrush(bounds);
		graphics.DrawEllipse(pen, bounds.X, bounds.Y, bounds.Width, bounds.Height);
		// graphics.DrawEllipse(pen, cx - r, cy - r, 2 * r, 2 * r);
		delete pen;
	}

	delete brush;
	graphics.Restore(state);
}

void circle::setCX(float cx)
{
	this->cx = cx;
}

void circle::setCY(float cy)
{
	this->cy = cy;
}

void circle::setRadius(float r)
{
	this->r = r;
}

RectF circle::getBounds() const
{
	return RectF(cx - r, cy - r, 2 * r, 2 * r);
}