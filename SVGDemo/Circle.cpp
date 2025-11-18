#include "stdafx.h"
#include "Circle.h"
#include "Color.h"


circle::circle() {
	cx = cy = r = 0;
}

//circle::circle(const map<string, string>& attrs) {
//	cx = stof(attrs.at("cx"));
//	cy = stof(attrs.at("cy"));
//	r = stof(attrs.at("r"));
//
//	if (attrs.find("fill") != attrs.end()) {
//		fill_color = parseColor(attrs.at("fill"));
//	}
//
//	if (attrs.find("stroke") != attrs.end()) {
//		stroke_color = parseColor(attrs.at("stroke"));
//		if (stroke_width == 0) {
//			stroke_width = 1;
//		}
//	}
//
//	if (attrs.find("stroke-width") != attrs.end()) {
//		stroke_width = stof(attrs.at("stroke-witdh"));
//	}
//
//	if (attrs.find("fill-opacity") != attrs.end()) {
//		fill_opacity = stof(attrs.at("fill-opacity"));
//	}
//
//	if (attrs.find("stroke-opacity") != attrs.end()) {
//		stroke_opacity = stof(attrs.at("stroke-opacity"));
//	}
//}

void circle::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

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