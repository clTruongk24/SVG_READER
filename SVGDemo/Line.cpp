#include "Line.h"
#include "Color.h"

line::line() {
	x1 = y1 = x2 = y2 = 0;
}

void line::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

	Pen pen(ColorWithOpacity(stroke_color, stroke_opacity), stroke_width);

	graphics.DrawLine(&pen, x1, y1, x2, y2);

	graphics.Restore(state);
}