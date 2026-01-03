#ifndef SVGPARSER_H
#define SVGPARSER_H

#include "Shape.h"
#include "Gradient.h"
#include "ViewBox.h"
#include "rapidxml.hpp"
#include "Defs.h"

#include <string>
#include <vector>
#include <map>


class SVGParser {
private:
	viewBox svgViewBox;

	void parseViewBox(rapidxml::xml_node<>*);
public:

    SVGParser();
    ~SVGParser();

    std::vector<shape*> ParseFile(const std::string& filePath);

    shape* ParseNode(rapidxml::xml_node<>*);
    shape* ParseElement(const std::string& tagName, const std::map<std::string, std::string>& attributes);

	void parseDefs(rapidxml::xml_node<>*);
	void parseStyles(rapidxml::xml_node<>*);
    
	GradientBase* parseLinearGradient(rapidxml::xml_node<>*);
	GradientBase* parseRadialGradient(rapidxml::xml_node<>*);
	void parseStops(rapidxml::xml_node<>*, GradientBase*);

	const viewBox& getViewBox() const;

	//Defs& getDefs();
};

float parseCoordinate(const std::string&);

#endif // SVGPARSER_H


