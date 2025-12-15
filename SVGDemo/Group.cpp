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
    if (!s || this->attrs.empty()) return;

    
	map<string, string> groupAttributes = this->attrs;
	map<string, string> childAttrs = s->getAttrs();

    if (!childAttrs.count("fill") && groupAttributes.count("fill")) {
        s->setFillColor(groupAttributes.at("fill"));
	}

    if (!childAttrs.count("stroke") && groupAttributes.count("stroke")) {
        s->setStrokeColor(groupAttributes.at("stroke"));

        if (s->getStrokeWidth() == 0.0f) {
            s->setStrokeWidth("1.0");
		}
    }

    if (!childAttrs.count("stroke-width") && groupAttributes.count("stroke-width")) {
        s->setStrokeWidth(groupAttributes.at("stroke-width"));
	}

    if (!childAttrs.count("fill-opacity") && groupAttributes.count("fill-opacity")) {
        s->setFillOpacity(groupAttributes.at("fill-opacity"));
    }

    if (!childAttrs.count("stroke-opacity") && groupAttributes.count("stroke-opacity")) {
        s->setStrokeOpacity(groupAttributes.at("stroke-opacity"));
	}

    if (groupAttributes.count("opacity")) {
        float groupOpacity = stof(groupAttributes.at("opacity"));

        if (!childAttrs.count("fill-opacity") && !childAttrs.count("opacity")) {
            float currentFillOpacity = s->getFillOpacity();
            s->setFillOpacity(to_string(currentFillOpacity * groupOpacity));
        }

        if (!childAttrs.count("stroke-opacity") && !childAttrs.count("opacity")) {
            float currentStrokeOpacity = s->getStrokeOpacity();
            s->setStrokeOpacity(to_string(currentStrokeOpacity * groupOpacity));
        }
    }

    if (!childAttrs.count("fill-opacity") && !childAttrs.count("opacity") && groupAttributes.count("fill-opacity")) {
        float groupFillOpacity = stof(groupAttributes.at("fill-opacity"));
        float currentFillOpacity = s->getFillOpacity();
        s->setFillOpacity(to_string(currentFillOpacity * groupFillOpacity));
    }

    if (!childAttrs.count("stroke-opacity") && !childAttrs.count("opacity") && groupAttributes.count("stroke-opacity")) {
        float groupStrokeOpacity = stof(groupAttributes.at("stroke-opacity"));
        float currentStrokeOpacity = s->getStrokeOpacity();
        s->setStrokeOpacity(to_string(currentStrokeOpacity * groupStrokeOpacity));
    }

    group* childGroup = dynamic_cast<group*>(s);
    if (childGroup) {
        const vector<string> inheritable = {
            "fill", "stroke", "stroke-width",
            "fill-opacity", "stroke-opacity", "opacity"
        };

        map<string, string> childGroupAttrs = childGroup->getAttrs();
        bool needUpdate = false;

        for (const auto& key : inheritable) {
            if (groupAttributes.count(key) && !childGroupAttrs.count(key)) {
                childGroupAttrs[key] = groupAttributes.at(key);
                needUpdate = true;
            }
        }

        if (needUpdate) {
            childGroup->setAttrs(childGroupAttrs);
        }

        childGroup->applyAttribute();
    }
}

void group::applyAttribute() {
    for (shape* child : children) {
        ApplyInheritAttribute(child);
    }
}