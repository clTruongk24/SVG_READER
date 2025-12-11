#include "stdafx.h"
#include "Group.h"
#include "Factory.h"
#include "Color.h"

using namespace rapidxml;

group::group() {}

group::~group() {
	for (shape* child : children) {
		delete child;
	}
	children.clear();
}

void group::addChild(shape* child) {
	children.push_back(child);
}

void group::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

	if (transform) {
		transform->Apply(graphics);
	}

	for (shape* child : children) {
		child->draw(graphics);
	}

	graphics.Restore(state);
}

void group::ApplyAttributes(const map<string, string>& attrs) {
	if (children.empty()) {
		return;
	}

	for (shape* child : children) {
		ApplyCommonAttributes(child, attrs);
	}
}

void group::ApplyCommonAttributes(shape* s, const map<string, string>& attrs) {
	if (attrs.count("id") && attrs.at("id") != s->getID()) {
		s->setID(attrs.at("id"));
	}

	if (attrs.count("stroke")) {
		Color stroke_color = parseColor(attrs.at("stroke"));
		if (stroke_color.GetValue() != s->getStrokeColor().GetValue()) {
			s->setStrokeColor(attrs.at("stroke"));
		}
	}

	if (attrs.count("fill")) {
		Color fill_color = parseColor(attrs.at("fill"));
		if (fill_color.GetValue() != s->getFillColor().GetValue()) {
			s->setFillColor(attrs.at("fill"));
		}
	}

	if (attrs.count("stroke-opacity")) {
		float stroke_opacity = stof(attrs.at("stroke-opacity"));
		if (stroke_opacity != s->getStrokeOpacity()) {
			s->setStrokeOpacity(attrs.at("stroke-opacity"));
		}
	}

	if (attrs.count("fill-opacity")) {
		float fill_opacity = stof(attrs.at("fill-opacity"));
		if (fill_opacity != s->getFillOpacity()) {
			s->setFillOpacity(attrs.at("fill-opacity"));
		}
	}

	if (attrs.count("stroke-width")) {
		float stroke_width = stof(attrs.at("stroke-width"));
		if (stroke_width != s->getStrokeWidth()) {
			s->setStrokeWidth(attrs.at("stroke-width"));
		}
	}
	if (attrs.count("opacity")) {
		float opacity = stof(attrs.at("opacity"));
		if (opacity != s->getFillOpacity()) {
			s->setFillOpacity(attrs.at("opacity"));
		}
		if (opacity != s->getStrokeOpacity()) {
			s->setStrokeOpacity(attrs.at("opacity"));
		}
	}

}