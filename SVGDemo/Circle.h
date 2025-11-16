#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"

class circle : public shape {
private:
	float cx;
	float cy;
	float r;
public:
	circle();
	circle(const map<string, string>&);
	void draw(Graphics&) override;
};

#endif // !CIRCLE_H
