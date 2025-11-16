#ifndef RENDERER_H
#define RENDERER_H

#include "Shape.h"

class Renderer {
protected:
	Graphics& graphics;
public:
	//Renderer();
	Renderer(Graphics&);


	virtual void render(const vector<shape*>&);
	virtual void render(shape*);
};

#endif // !RENDERER_H
