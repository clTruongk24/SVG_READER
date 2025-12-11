#include "stdafx.h"
#include "Shape.h"
#include "Color.h"

using namespace std;
using namespace Gdiplus;

shape::shape() {
	id = "";
	stroke_color = Color(0, 0, 0);
	fill_color = Color(0, 0, 0);
	stroke_opacity = fill_opacity = 1.0f;
	stroke_width = 0;
	transform = nullptr;
}

shape::~shape() {
	if (transform) {
		delete transform;
		transform = nullptr;
	}
}

void shape::setID(const string& id) {
	this->id = id;
}
void shape::setStrokeColor(const string& strokeStr) {
	stroke_color = parseColor(strokeStr);
}
void shape::setFillColor(const string& fillStr) {
	fill_color = parseColor(fillStr);
}
void shape::setStrokeOpacity(const string& strokeStr) {
	stroke_opacity = stof(strokeStr);
}
void shape::setFillOpacity(const string& fillStr) {
	fill_opacity = stof(fillStr);
}
void shape::setStrokeWidth(const string& strokeValue) {
	stroke_width = stof(strokeValue);
}

void shape::setTransform(Transform* transform) {
	delete this->transform;
	this->transform = transform;
}

void shape::setTransform(const string& transformStr) {
	delete this->transform;
	this->transform = Transform::parse(transformStr);
}

string shape::getID() const {
	return id;
}

Color shape::getStrokeColor() const {
	return stroke_color;
}

Color shape::getFillColor() const {
	return fill_color;
}

float shape::getStrokeOpacity() const {
	return stroke_opacity;
}

float shape::getFillOpacity() const {
	return fill_opacity;
}

float shape::getStrokeWidth() const {
	return stroke_width;
}