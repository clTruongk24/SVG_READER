#include "Rectangle.h"
#include "Color.h"

rectangle::rectangle() {
	x = y = width = height = 0;
}

void rectangle::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

	SolidBrush brush(ColorWithOpacity(fill_color, fill_opacity));
	Pen pen(ColorWithOpacity(stroke_color, stroke_opacity), stroke_width);

	graphics.FillRectangle(&brush, x, y, width, height);
	if (stroke_width != 0) {
		graphics.DrawRectangle(&pen, x, y, width, height);
	}

	graphics.Restore(state);
}