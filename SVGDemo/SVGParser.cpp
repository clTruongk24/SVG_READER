#include "stdafx.h"          
#include "SVGParser.h"
#include "ShapeFactory.h"    // To call CreateShape from ShapeFactory
#include "Shape.h"           // To use Shape*
#include "rapidxml.hpp"      // Parse XML library
#include <fstream>        
#include <sstream>         
#include <iostream>

using namespace std;
using namespace rapidxml;

SVGParser::SVGParser() {}
SVGParser::~SVGParser() {}

vector<Shape*> SVGParser::ParseFile(string path) {
    vector<Shape*> shapes;

    ifstream file(path);
    if (!file.is_open()) {
        return shapes; 
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    if (content.empty()) return shapes;

    vector<char> xmlCopy(content.begin(), content.end());
    xmlCopy.push_back('\0'); 

    try {

        xml_document<> doc;
        doc.parse<0>(&xmlCopy[0]);


        xml_node<>* root = doc.first_node("svg");
        if (!root) return shapes;

        for (xml_node<>* node = root->first_node(); node; node = node->next_sibling()) {

            string tagName = node->name();

           
            if (tagName == "defs" || tagName == "style" || tagName == "metadata") {
                continue;
            }

            map<string, string> attributes;
            for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
                string attrName = attr->name();
                string attrValue = attr->value();

                attributes[attrName] = attrValue;
            }

            if (node->value() && strlen(node->value()) > 0) {
                attributes["content"] = node->value();
            }

            Shape* shape = ParseElement(tagName, attributes);

            if (shape != nullptr) {
                shapes.push_back(shape);
            }
        }
    }
    return shapes;
}

Shape* SVGParser::ParseElement(string tag, map<string, string> attrs) {
    if (tag.empty()) {
        return nullptr;
    }

    return ShapeFactory::CreateShape(tag, attrs);
}