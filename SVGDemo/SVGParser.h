#ifndef SVGPARSER_H
#define SVGPARSER_H

#include "Shape.h"
#include <string>
#include <vector>
#include <map>

class SVGParser {
public:

    SVGParser();
    ~SVGParser();

    // Read File and return list of shapes
    std::vector<shape*> ParseFile(const std::string& filePath);

    // Process each tag and its attributes
    shape* ParseElement(const std::string& tagName, const std::map<std::string, std::string>& attributes);
};

#endif // SVGPARSER_H


