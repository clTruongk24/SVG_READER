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

        if (childFillOpacity == 0.0f) {
            s->setFillColor(groupAttributes.at("fill"));
        }
    }

    if (groupAttributes.count("stroke")) {
        float childStrokeWidth = s->getStrokeWidth();

        if (childStrokeWidth == 0) {
            s->setStrokeColor(groupAttributes.at("stroke"));

            if (!groupAttributes.count("stroke-width")) {
                s->setStrokeWidth("1");
            }
        }
    }

    if (groupAttributes.count("stroke-width")) {
        float childStrokeWidth = s->getStrokeWidth();

        if (childStrokeWidth == 0) {
            s->setStrokeWidth(groupAttributes.at("stroke-width"));
        }
    }

    if (groupAttributes.count("fill-opacity")) {
        if (s->getFillOpacity() == 1.0f) {
            s->setFillOpacity(groupAttributes.at("fill-opacity"));
        }
    }

    if (groupAttributes.count("stroke-opacity")) {
        if (s->getStrokeOpacity() == 1.0f) {
            s->setStrokeOpacity(groupAttributes.at("stroke-opacity"));
        }
    }

    if (groupAttributes.count("opacity")) {
        if (s->getFillOpacity() == 1.0f) {
            s->setFillOpacity(groupAttributes.at("opacity"));
        }

        if (s->getStrokeOpacity() == 1.0f) {
            s->setStrokeOpacity(groupAttributes.at("opacity"));
        }
    }

    group* childGroup = dynamic_cast<group*>(s);
    if (childGroup) {
        const vector<string> inheritable = {
            "fill", "stroke", "stroke-width",
            "fill-opacity", "stroke-opacity", "opacity"
        };

        map<string, string> childAttrs = childGroup->getGroupAttributes();
        bool needUpdate = false;

        for (const auto& key : inheritable) {
            if (groupAttributes.count(key) && !childAttrs.count(key)) {
                childAttrs[key] = groupAttributes.at(key);
                needUpdate = true;
            }
        }

        if (needUpdate) {
            childGroup->setGroupAttributes(childAttrs);
        }
    }
}

map<string, string> group::getGroupAttributes() const {
    return groupAttributes;
}