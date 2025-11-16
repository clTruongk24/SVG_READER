#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"
#include <vector>

class polygon : public shape {
private:
	vector<Point> p;
public:
	void draw(Graphics&) override;
};
#endif // !POLYGON_H
