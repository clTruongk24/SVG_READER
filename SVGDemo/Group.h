#ifndef GROUP_H
#define GROUP_H

#include "Shape.h"
#include "rapidxml.hpp"
#include <map>
#include <vector>

class group : public shape {
private:
	std::vector<shape*> children;


	void ApplyInheritAttribute(shape*);
public:
	group();
	virtual ~group();

	RectF getBounds() const override;

	void draw(Graphics&) override;
	void addChild(shape*);
	void applyAttribute();

};

#endif // !GROUP_H