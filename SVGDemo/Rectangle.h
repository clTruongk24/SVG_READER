#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class rectangle : public shape {
private:
	float x;
	float y;
	float width;
	float height;
public:
	rectangle();
	void draw(Graphics&) override;
};

#endif // !RECTANGLE_H
