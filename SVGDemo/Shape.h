#ifndef SHAPE_H
#define SHAPE_H

#include "stdafx.h"
#include "Gradient.h"
#include "Transform.h"
#include <string>
#include <map>
#include <gdiplus.h>


using namespace std;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class shape {
protected:
    string id;
    Color stroke_color;
    Color fill_color;
    float stroke_opacity;
    float fill_opacity;
    float stroke_width;
    Transform* transform;

    map<string, string> attrs;
public:
    shape();
    virtual ~shape();
    virtual void draw(Graphics&) = 0;

    void setID(const string&);
    void setStrokeColor(const string&);
    void setFillColor(const string&);
    void setStrokeOpacity(const string&);
    void setFillOpacity(const string&);
    void setStrokeWidth(const string&);

    void setTransform(Transform*);
	void setTransform(const string&);

    void setAttrs(const map<string, string>&);

    string getID() const;
	Color getStrokeColor() const;
	Color getFillColor() const;
	float getStrokeOpacity() const;
	float getFillOpacity() const;
	float getStrokeWidth() const;

	map<string, string> getAttrs() const;

};



#endif //SHAPE_H
