#ifndef SVGPARSER_H
#define SVGPARSER_H

#include <vector>
#include <map>
#include "Shape.h"
#include "rapidxml.hpp"

using namespace rapidxml;

class SVGParser {
private:
	void parseNode(xml_node<>*, vector<shape*>&);
public:
	vector<shape*> parseFile(const string&);
	shape* parseElement(const string&, const map<string, string>&);
};

#endif // !SVGREADER_H