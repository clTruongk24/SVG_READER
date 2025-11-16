#ifndef POLYLINE_H
#define POLYLINE_H

#include "Shape.h"
#include <vector>

class polyline : public shape {
private:
	vector<Point> p;
public:
	void draw(Graphics&) override;
};
#endif // !POLYLINE_H
