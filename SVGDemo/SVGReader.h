#ifndef SVGREADER_H
#define SVGREADER_H

#include "Shape.h"
#include "SVGParser.h"
#include "Renderer.h"

class SVGReader {
private:
	string filePath;
	vector<shape*> m_shapes;
	Renderer* m_render;
	SVGParser m_parser;
public:
	SVGReader(const string&);
	SVGReader(const string&, Graphics&);

	virtual ~SVGReader();
	
	void Parse();
	void Render();

	void setGraphics(Graphics&);
	std::vector<shape*> GetShapes();
};

#endif // !SVGREADER_H


