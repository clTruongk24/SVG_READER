#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <vector>
#include <map>
#include "Shape.h"
#include "rapidxml.hpp"

using namespace rapidxml;

class SVGParser {
public:
	vector<shape*> parseFile(xml_node<>*);
	shape* parseNode(xml_node<>*);
};

#endif // !SVGREADER_H