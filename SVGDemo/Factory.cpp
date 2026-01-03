#include "stdafx.h"
#include "Factory.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Ellipse.h"
#include "Line.h"
#include "Polygon.h"
#include "Polyline.h"
#include "Text.h"
#include "Group.h"
#include "Path.h"
#include <functional>

//shape* factory::createShape(const string& tag, const map<string, string>& attrs) {
//	shape* s = nullptr;
//
//	if (tag == "rect") {
//		rectangle* temp = new rectangle();
//		if (attrs.count("x")) temp->setX(stof(attrs.at("x")));
//		if (attrs.count("y")) temp->setY(stof(attrs.at("y")));
//		if (attrs.count("width")) temp->setWidth(stof(attrs.at("width")));
//		if (attrs.count("height")) temp->setHeight(stof(attrs.at("height")));
//		s = temp;
//	} 
//	else if (tag == "circle") {
//		circle* temp = new circle();
//		if (attrs.count("cx")) temp->setCX(stof(attrs.at("cx")));
//		if (attrs.count("cy")) temp->setCY(stof(attrs.at("cy")));
//		if (attrs.count("r")) temp->setRadius(stof(attrs.at("r")));
//		s = temp;	
//	}
//	else if (tag == "ellipse") {
//		ellipse* temp = new ellipse();
//		if (attrs.count("cx")) temp->setCX(stof(attrs.at("cx")));
//		if (attrs.count("cy")) temp->setCY(stof(attrs.at("cy")));
//		if (attrs.count("rx")) temp->setRX(stof(attrs.at("rx")));
//		if (attrs.count("ry")) temp->setRY(stof(attrs.at("ry")));
//		s = temp;
//	}
//	else if (tag == "line") {
//		line* temp = new line();
//		if (attrs.count("x1")) temp->setX1(stof(attrs.at("x1")));
//		if (attrs.count("y1")) temp->setY1(stof(attrs.at("y1")));
//		if (attrs.count("x2")) temp->setX2(stof(attrs.at("x2")));
//		if (attrs.count("y2")) temp->setY2(stof(attrs.at("y2")));
//		s = temp;
//
//	}
//	else if (tag == "polygon") {
//		polygon* temp = new polygon();
//		if (attrs.count("points")) temp->setPoints(attrs.at("points"));
//		s = temp;
//		
//	}
//	else if (tag == "polyline") {
//		polyline* temp = new polyline();
//		if (attrs.count("points")) temp->setPoints(attrs.at("points"));
//		s = temp;
//	}
//	else if (tag == "text") {
//		text* temp = new text();
//		if (attrs.count("x")) temp->setX(stof(attrs.at("x")));
//		if (attrs.count("y")) temp->setY(stof(attrs.at("y")));
//		if (attrs.count("font-size")) temp->setFontSize(stof(attrs.at("font-size")));
//		if (attrs.count("font-family")) {
//			string family = attrs.at("font-family");
//			size_t commaPos = family.find(',');
//			if (commaPos != string::npos) {
//				family = family.substr(0, commaPos);
//			}
//			temp->setFontFamily(family);
//		}
//		if (attrs.count("text-anchor")) temp->setTextAnchor(attrs.at("text-anchor"));
//		if (attrs.count("font-style") && attrs.at("font-style") == "italic") {
//			temp->setItalic(true);
//		}
//		if (attrs.count("dx")) temp->setDX(stof(attrs.at("dx")));
//		if (attrs.count("dy")) temp->setDY(stof(attrs.at("dy")));
//		//temp->setStrokeWidth("1.0f");
//		s = temp;
//	}
//	/*else if (tag == "g") {
//		group* temp = new group();
//
//		temp->ApplyAttributes(attrs);
//
//		s = temp;
//	}*/
//	else if (tag == "path") {   
//		path* temp = new path();
//
//		if (attrs.count("d")) {
//			temp->setD(attrs.at("d"));
//		}
//
//		s = temp; 
//	}
//	if (s) {
//		ApplyCommonAttributes(s, attrs);
//		s->setAttrs(attrs);
//	}
//
//	return s;
//}

using shapeCreator = std::function<shape* (const map<string, string>&)>;

shape* createRectangle(const map<string, string>& attrs) {
	rectangle* rect = new rectangle();
	if (attrs.count("x")) rect->setX(stof(attrs.at("x")));
	if (attrs.count("y")) rect->setY(stof(attrs.at("y")));
	if (attrs.count("width")) rect->setWidth(stof(attrs.at("width")));
	if (attrs.count("height")) rect->setHeight(stof(attrs.at("height")));
	return rect;
}

shape* createCircle(const map<string, string>& attrs) {
	circle* circ = new circle();
	if (attrs.count("cx")) circ->setCX(stof(attrs.at("cx")));
	if (attrs.count("cy")) circ->setCY(stof(attrs.at("cy")));
	if (attrs.count("r")) circ->setRadius(stof(attrs.at("r")));
	return circ;
}

shape* createEllipse(const map<string, string>& attrs) {
	ellipse* ellip = new ellipse();
	if (attrs.count("cx")) ellip->setCX(stof(attrs.at("cx")));
	if (attrs.count("cy")) ellip->setCY(stof(attrs.at("cy")));
	if (attrs.count("rx")) ellip->setRX(stof(attrs.at("rx")));
	if (attrs.count("ry")) ellip->setRY(stof(attrs.at("ry")));
	return ellip;
}

shape* createLine(const map<string, string>& attrs) {
	line* ln = new line();
	if (attrs.count("x1")) ln->setX1(stof(attrs.at("x1")));
	if (attrs.count("y1")) ln->setY1(stof(attrs.at("y1")));
	if (attrs.count("x2")) ln->setX2(stof(attrs.at("x2")));
	if (attrs.count("y2")) ln->setY2(stof(attrs.at("y2")));
	return ln;
}

shape* createPolygon(const map<string, string>& attrs) {
	polygon* poly = new polygon();
	if (attrs.count("points")) poly->setPoints(attrs.at("points"));
	return poly;
}

shape* createPolyline(const map<string, string>& attrs) {
	polyline* polylineShape = new polyline();
	if (attrs.count("points")) polylineShape->setPoints(attrs.at("points"));
	return polylineShape;
}

shape* createText(const map<string, string>& attrs) {
	text* txt = new text();
	if (attrs.count("x")) txt->setX(stof(attrs.at("x")));
	if (attrs.count("y")) txt->setY(stof(attrs.at("y")));
	if (attrs.count("font-size")) txt->setFontSize(stof(attrs.at("font-size")));
	if (attrs.count("font-family")) {
		string family = attrs.at("font-family");
		size_t commaPos = family.find(',');
		if (commaPos != string::npos) {
			family = family.substr(0, commaPos);
		}
		txt->setFontFamily(family);
	}
	if (attrs.count("text-anchor")) txt->setTextAnchor(attrs.at("text-anchor"));
	if (attrs.count("font-style") && attrs.at("font-style") == "italic") {
		txt->setItalic(true);
	}
	if (attrs.count("dx")) txt->setDX(stof(attrs.at("dx")));
	if (attrs.count("dy")) txt->setDY(stof(attrs.at("dy")));
	return txt;
}

shape* createPath(const map<string, string>& attrs) {
	path* pth = new path();
	if (attrs.count("d")) {
		pth->setD(attrs.at("d"));
	}
	return pth;
}

static const map<string, shapeCreator> shapeFactoryMap = {
	{"rect", createRectangle},
	{"circle", createCircle},
	{"ellipse", createEllipse},
	{"line", createLine},
	{"polygon", createPolygon},
	{"polyline", createPolyline},
	{"text", createText},
	{"path", createPath}
};

shape* factory::createShape(const string& tag, const map<string, string>& attrs) {
	
	auto it = shapeFactoryMap.find(tag);

	if (it == shapeFactoryMap.end()) {
		return nullptr;
	}

	shape* s = it->second(attrs);
	
	if (s) {
		ApplyCommonAttributes(s, attrs);
		s->setAttrs(attrs);
	}

	return s;
}


void factory::ApplyCommonAttributes(shape* s, const map<string, string>& attrs) {
	if (!s) return;

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
		s->setStrokeOpacity(attrs.at("stroke-opacity"));
	}

	if (attrs.count("fill-opacity")) {
		s->setFillOpacity(attrs.at("fill-opacity"));
	}

	if (attrs.count("stroke-width")) {
		s->setStrokeWidth(attrs.at("stroke-width"));
	}

	if (attrs.count("opacity")) {
		string opacity = attrs.at("opacity");
		s->setFillOpacity(opacity);
		s->setStrokeOpacity(opacity);
	}

	if (attrs.count("transform")) {
		s->setTransform(attrs.at("transform"));
	}

	if (attrs.count("style")) {
		string styleStr = attrs.at("style");
		map<string, string> styleAttrs = Defs::getInstance().parseStyleString(styleStr);
		for (const auto& pair : styleAttrs) {
			const string& key = pair.first;
			const string& value = pair.second;
			if (key == "stroke" && !attrs.count("stroke")) {
				s->setStrokeColor(value);
			}
			else if (key == "fill" && !attrs.count("fill")) {
				s->setFillColor(value);
			}
			else if (key == "stroke-opacity" && !attrs.count("stroke-opacity")) {
				s->setStrokeOpacity(value);
			}
			else if (key == "fill-opacity" && !attrs.count("fill-opacity")) {
				s->setFillOpacity(value);
			}
			else if (key == "stroke-width" && !attrs.count("stroke-width")) {
				s->setStrokeWidth(value);
			}
		}
	}
}