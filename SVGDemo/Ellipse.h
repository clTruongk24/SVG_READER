#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Shape.h"

class ellipse : public shape {
private:
	float cx;
	float cy;
	float rx;
	float ry;
public:
	ellipse();
	void draw(Graphics&) override;
};

#endif // !ELLIPSE_H
