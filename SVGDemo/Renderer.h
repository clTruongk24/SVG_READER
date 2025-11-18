#pragma 
#include <vector>
#include "Shape.h"


class Renderer
{
public:
	void Draw(const std::vector<Shape*>& shapes, Graphics* g);

	void Draw(const Shape& shape, Graphics* g);

	void SetTransform(const Transform& transform, Graphics* g);
};

