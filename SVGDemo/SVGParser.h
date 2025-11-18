#pragma once
#include <string>
#include <vector>
#include <map>

class Shape; //Forward Declaration

class SVGParser {
public:

    SVGParser();
    ~SVGParser();

    // Read File and return list of shapes
    std::vector<Shape*> ParseFile(const std::string& filePath);

    // Process each tag and its attributes
    Shape* ParseElement(const std::string& tagName, const std::map<std::string, std::string>& attributes);
};