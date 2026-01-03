#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"
#include <vector>

class polygon : public shape {
private:
	vector<Point> p;
public:
	void draw(Graphics&) override;

	RectF getBounds() const override;

	void setPoints(const string&);
};
#endif // !POLYGON_H
