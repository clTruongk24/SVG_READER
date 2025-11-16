#include "SVGParser.h"

vector<shape*> SVGParser::parseFile(xml_node<>* node) {
	vector<shape*> Shapes;

	for (xml_node<>* p = node; node; node = node->next_sibling()) {
		shape* s = parseNode(p);
		if (s) Shapes.push_back(s);
	}

	return Shapes;
}

shape* SVGParser::parseNode(xml_node<>* node) {
	string tag = node->name();

	map<string, string> attrs;
	for (auto* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
		attrs[attr->name()] = attr->value();
	}

	return FactoryShape(tag, attrs);
}