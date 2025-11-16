#include "Polyline.h"
#include "Color.h"

void polyline::draw(Graphics& graphics) {
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