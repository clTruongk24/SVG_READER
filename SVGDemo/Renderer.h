#ifndef RENDERER_H
#define RENDERER_H
#include <vector>
#include "Shape.h"


class Renderer
{
private:
	Graphics& graphics;
public:
	Renderer(Graphics&);

	void Draw(const std::vector<shape*>& shapes);

	void Draw(shape* shape);

	//void SetTransform(const Transform& transform, Graphics* g);
};

#endif //! RENDERER_H

