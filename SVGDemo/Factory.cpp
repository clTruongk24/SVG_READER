#include "stdafx.h"
#include "Factory.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Line.h"
#include "Polygon.h"
#include "Polyline.h"

shape* factory::createShape(const string& tag, const map<string, string>& attrs) {
	shape* s;

	if (tag == "rect") {
		s = new rectangle(attrs);
	} 
	else if (tag == "circle") {
		s = new circle(attrs);
	}
	else if (tag == "ellipse") {
		s = new ellipse(attrs);
	}
	else if (tag == "line") {
		s = new line(attrs);
	}
	else if (tag == "polygon") {
		s = new polygon(attrs);
	}
	else if (tag == "polyline") {
		s = new polyline(attrs);
	}
}