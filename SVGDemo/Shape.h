#ifndef SHAPE_H
#define SHAPE_H

#include "stdafx.h"
#include <string>
#include <gdiplus.h>

class Transform { //thêm class transform
private:
    float scaleX = 1.0f;
    float scaleY = 1.0f;
    float rotation = 0.0f;
    float translateX = 0.0f;
    float translateY = 0.0f;

public:
    void initTransform(float sX, float sY, float r, float tX, float tY) {
        scaleX = sX;
        scaleY = sY;
        rotation = r;
        translateX = tX;
        translateY = tY;
    }

    void apply(Matrix& mtx) const {
        mtx.Reset();
        mtx.Scale(scaleX, scaleY);
        mtx.Rotate(rotation);
        mtx.Translate(translateX, translateY);
    }
};

class Shape {
	std::string id;
	Gdiplus::Color stroke_color;
	Gdiplus::Color fill_color;
	float stroke_width;
	Matrix transform;
};

#endif //SHAPE_H
