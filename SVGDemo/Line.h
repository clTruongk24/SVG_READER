#ifndef LINE_H
#define LINE_H

#include "Shape.h"

class line : public shape {
private:
	float x1;
	float y1;
	float x2;
	float y2;
public:
	line();
	void draw(Graphics&) override;
};
#endif // !LINE_H
