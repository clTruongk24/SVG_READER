#ifndef POLYLINE_H
#define POLYLINE_H

#include "Shape.h"
#include <vector>

class polyline : public shape {
private:
	vector<Point> p;
public:
	void draw(Graphics&) override;
	
	RectF getBounds() const override;

	void setPoints(const string&);
};
#endif // !POLYLINE_H
