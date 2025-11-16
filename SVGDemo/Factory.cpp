#include "stdafx.h"
#include "Factory.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Line.h"
#include "Polygon.h"
#include "Polyline.h"

shape* factory::createShape(const string& tag, const map<string, string>& attrs) {
	shape* s = nullptr;

	if (tag == "rect") {
		rectangle* temp = new rectangle();
		temp->setX(stof(attrs.at("x")));
		temp->setY(stof(attrs.at("y")));
		temp->setWidth(stof(attrs.at("width")));
		temp->setHeight(stof(attrs.at("height")));
		s = temp;
	} 
	else if (tag == "circle") {
		circle* temp = new circle();
		temp->setCX(stof(attrs.at("cx")));
		temp->setCY(stof(attrs.at("cy")));
		temp->setRadius(stof(attrs.at("r")));
		s = temp;	
	}
	else if (tag == "ellipse") {
		ellipse* temp = new ellipse();
		temp->setCX(stof(attrs.at("cx")));
		temp->setCY(stof(attrs.at("cy")));
		temp->setRX(stof(attrs.at("rx")));
		temp->setRY(stof(attrs.at("ry")));
		s = temp;
	}
	else if (tag == "line") {
		line* temp = new line();
		temp->setX1(stof(attrs.at("x1")));
		temp->setY1(stof(attrs.at("y1")));
		temp->setX2(stof(attrs.at("x2")));
		temp->setY2(stof(attrs.at("y2")));
		s = temp;

	}
	else if (tag == "polygon") {
		polygon* temp = new polygon();
		temp->setPoints(attrs.at("points"));
		s = temp;
		
	}
	else if (tag == "polyline") {
		polyline* temp = new polyline();
		temp->setPoints(attrs.at("points"));
		s = temp;
	}

	if (s) {
		ApplyCommonAttributes(s, attrs);
	}

	return s;
}

static void ApplyCommonAttributes(shape* s, const map<string, string>& attrs) {
	if (attrs.count("id")) {
		s->setID(attrs.at("id"));
	}

	if (attrs.count("stroke")) {
		s->setStrokeColor(attrs.at("stroke"));
	}

	if (attrs.count("fill")) {
		s->setFillColor(attrs.at("fill"));
	}

	if (attrs.count("stroke-opacity")) {
		s->setStrokeOpacity(attrs.at("stroke-opacity");
	}

	if (attrs.count("fill-opacity")) {
		s->setFillOpacity(attrs.at("fill-opacity"));
	}

	if (attrs.count("stroke-width")) {
		s->setStrokeWidth(attrs.at("stroke-width"));
	}
}