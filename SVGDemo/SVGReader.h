#ifndef SVGREADER_H
#define SVGREADER_H

#include "Shape.h"
#include "SVGParser.h"
#include "Renderer.h"

class SVGReader {
private:
	string filePath;
	vector<shape*> m_shapes;
	Renderer m_render;
	SVGParser m_parser;
public:
	SVGReader(const string&);

	virtual ~SVGReader();

};

#endif // !SVGREADER_H
