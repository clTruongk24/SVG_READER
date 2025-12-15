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
	if (!child) {
		return;
	}
	children.push_back(child);
	ApplyInheritAttribute(child);
}

void group::setGroupAttributes(const map<string, string>& attrs) {
	groupAttributes = attrs;

	factory::ApplyCommonAttributes(this, attrs);
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


void group::ApplyInheritAttribute(shape* s) {
	if (!s || groupAttributes.empty()) return;

	if (groupAttributes.count("fill")) {
		Color childFill = s->getFillColor();
		float childFillOpacity = s->getFillOpacity();

		if (childFill.GetR() == 0 && childFill.GetG() == 0 && childFill.GetB() == 0 && childFillOpacity == 1.0f) {
			s->setFillColor(groupAttributes.at("fill"));
		}
	}

	if (groupAttributes.count("stroke")) {
		Color childStroke = s->getStrokeColor();
		float childStrokeWidth = s->getStrokeWidth();

		if (childStrokeWidth == 0) {
			s->setStrokeColor(groupAttributes.at("stroke"));
			s->setStrokeWidth("1");
		}
	}

	if (groupAttributes.count("stroke-width")) {
		float childStrokeWidth = s->getStrokeWidth();

		if (childStrokeWidth == 0) {
			s->setStrokeWidth(groupAttributes.at("stroke-width"));
		}
	}

	if (groupAttributes.count("fill-opacity")) {
		float childFillOpacity = s->getFillOpacity();

		if (s->getFillOpacity() == 1.0f) {
			s->setFillOpacity(groupAttributes.at("fill-opacity"));
		}
	}

	if (groupAttributes.count("stroke-opacity")) {
		float childStrokeOpacity = s->getStrokeOpacity();

		if (s->getStrokeOpacity() == 1.0f) {
			s->setStrokeOpacity(groupAttributes.at("stroke-opacity"));
		}
	}

	if (groupAttributes.count("opacity")) {
		float childFillOpacity = s->getFillOpacity();
		float childStrokeOpacity = s->getStrokeOpacity();

		if (s->getFillOpacity() == 1.0f) {
			s->setFillOpacity(groupAttributes.at("opacity"));
		}
			
		if (s->getStrokeOpacity() == 1.0f) {
			s->setStrokeOpacity(groupAttributes.at("opacity"));
		}
	}

	group* childGroup = dynamic_cast<group*>(s);
	if (childGroup) {
		const std::vector<std::string> inheritable = {
			"fill", "stroke", "stroke-width", "fill-opacity", "stroke-opacity", "opacity"
		};

		for (const auto& key : inheritable) {
			if (groupAttributes.count(key) && !childGroup->getGroupAtrributes().count(key)) {
				map<string, string> inheritedAttr;
				inheritedAttr[key] = groupAttributes.at(key);
				factory::ApplyCommonAttributes(childGroup, inheritedAttr);
			}
		}
	}
}

map<string, string> group::getGroupAtrributes() const {
	return groupAttributes;
}