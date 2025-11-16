#include "SVGParser.h"
#include "Factory.h"
#include <fstream>

vector<shape*> SVGParser::parseFile(const string&) {
	vector<shape*> Shapes;

	xml_document<> doc;
	xml_node<>* rootNode;
	// Read the xml file into a vector
	ifstream file("sample.svg");
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

	rootNode = doc.first_node();

	if (rootNode) {
		parseNode(rootNode, Shapes);
	}

	return Shapes;
}
shape* SVGParser::parseElement(const string& tag, const map<string, string>& attrs) {
	return factory::createShape(tag, attrs);
}

void SVGParser::parseNode(xml_node<>* node, vector<shape*>& Shapes) {
	
	for (auto child = node->first_node(); child; child = child->next_sibling()) {
		string tag = child->name();

		map<string, string> attrs;

		for (auto attr = child->first_attribute(); attr; attr = attr->next_attribute()) {
			attrs[attr->name()] = attr->value();
		}

		shape* s = parseElement(tag, attrs);
		if (s) {
			Shapes.push_back(s);
		}

	}
}
