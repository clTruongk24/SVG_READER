#ifndef SHAPE_H
#define SHAPE_H

#include "stdafx.h"
#include "Gradient.h"
#include <string>
#include <map>
#include <gdiplus.h>


using namespace std;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

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

class shape {
protected:
    string id;
    Color stroke_color;
    Color fill_color;
    float stroke_opacity;
    float fill_opacity;
    float stroke_width;
    //Transform transform;
public:
    shape();
    virtual void draw(Graphics&) = 0;

    void setID(const string&);
    void setStrokeColor(const string&);
    void setFillColor(const string&);
    void setStrokeOpacity(const string&);
    void setFillOpacity(const string&);
    void setStrokeWidth(const string&);
};



#endif //SHAPE_H
