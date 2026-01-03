#include "stdafx.h"
#include "Shape.h"
#include "Color.h"
#include "Defs.h"
#include <memory>

using namespace std;
using namespace Gdiplus;

shape::shape() {
	id = "";
	stroke_color = Color(0, 0, 0);
	fill_color = Color(0, 0, 0, 0);
	stroke_opacity = fill_opacity = 1.0f;
	stroke_width = 0;
	transform = nullptr;

	hasFillGradient = false;
	hasStrokeGradient = false;

	//defs = nullptr;
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
	if (strokeStr == "none") {
		setStrokeColor("white");
		setStrokeOpacity("0");
		hasStrokeGradient = false;
		return;
	}

	if (strokeStr.find("url(") != string::npos) {
		string graID = Defs::extractIDFromURL(strokeStr);
		setStrokeGradientID(graID);
	}
	else {
		stroke_color = parseColor(strokeStr);
		hasStrokeGradient = false;
	}
}
void shape::setFillColor(const string& fillStr) {
	if (fillStr == "none") {
		setFillColor("white");
		setFillOpacity("0");
		hasFillGradient = false;
		return;
	}

	if (fillStr.find("url(") != string::npos) {
		string graID = Defs::extractIDFromURL(fillStr);
		setFillGradientID(graID);
	}
	else {
		fill_color = parseColor(fillStr);

		hasFillGradient = false;
	}
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
	if (this->transform) {
		delete this->transform;
		this->transform = nullptr;
	}
	this->transform = transform;
}

void shape::setTransform(const string& transformStr) {
	if (transform) {
				delete transform;
				this->transform = nullptr;
	}
	this->transform = Transform::parse(transformStr);
}

void shape::setAttrs(const map<string, string>& attrs) {
	this->attrs = attrs;
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

map<string, string> shape::getAttrs() const {
	return attrs;
}

SolidBrush* shape::createFillBrush() const {
	SolidBrush* brush = new SolidBrush(Color(ColorWithOpacity(fill_color, fill_opacity)));

	return brush;
}

Pen* shape::createStrokePen() const {
	Pen* pen = new Pen(Color(ColorWithOpacity(stroke_color, stroke_opacity)), stroke_width);
	return pen;
}


void shape::setFillGradientID(const string& graID) {
	fillGradientID = graID;
	hasFillGradient = !graID.empty();
}

void shape::setStrokeGradientID(const string& graID) {
	strokeGradientID = graID;
	hasStrokeGradient = !graID.empty();
}

bool shape::hasFillGradientFunc() const {
	return hasFillGradient;
}

bool shape::hasStrokeGradientFunc() const {
	return hasStrokeGradient;
}

string shape::getFillGradientID() const {
	return fillGradientID;
}

string shape::getStrokeGradientID() const {
	return strokeGradientID;
}

//Defs* shape::getDefs() const {
//	return defs;
//}
//
//void shape::setDefs(Defs* d) {
//	defs = d;
//}

Pen* shape::createStrokeGradientBrush(const RectF& bounds) const {
	if (hasStrokeGradient && !strokeGradientID.empty()) {
		GradientBase* gradient = Defs::getInstance().getGradient(strokeGradientID);
		if (gradient) {
			Brush* brush = gradient->createBrush(bounds);
			if (brush) {
				Pen* pen = new Pen(brush, stroke_width);
				delete brush;
				return pen;
			}

		}
	}
	return createStrokePen();
}

Brush* shape::createFillGradientBrush(const RectF& bounds) const {
	if (hasFillGradient && !fillGradientID.empty()) {
		GradientBase* gradient = Defs::getInstance().getGradient(fillGradientID);
		if (gradient) {
			return gradient->createBrush(bounds);
		}
	}
	return createFillBrush();
}