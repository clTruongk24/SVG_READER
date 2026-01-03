#ifndef RENDERER_H
#define RENDERER_H
#include <vector>
#include "Shape.h"
#include "Defs.h"

class Renderer
{
private:
	Graphics& graphics;
	//Defs* defs;
public:
	Renderer(Graphics&);
	//Renderer(Graphics&, Defs*);

	void Draw(const std::vector<shape*>& shapes);
	void Draw(shape* shape);

	/*void setDefs(Defs* defs);
	Defs* getDefs() const;*/

	//void SetTransform(const Transform& transform, Graphics* g);
};

#endif //! RENDERER_H

