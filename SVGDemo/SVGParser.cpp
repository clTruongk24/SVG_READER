#include "stdafx.h"
#include "SVGParser.h"
#include "Factory.h"    // To call CreateShape from ShapeFactory
#include "Shape.h"      // To use Shape*
#include <fstream>
#include <sstream>
#include <iostream>
#include "Text.h"
#include "Group.h"

using namespace std;
using namespace rapidxml;

SVGParser::SVGParser() {}
SVGParser::~SVGParser() {}

vector<shape*> SVGParser::ParseFile(const string& filePath)
{
    vector<shape*> Shapes;

    ifstream file(filePath);
    if (!file.is_open())
    {
        return Shapes;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string content = buffer.str();
    file.close();

    if (content.empty())
        return Shapes;

    vector<char> xmlCopy(content.begin(), content.end());
    xmlCopy.push_back('\0');

    xml_document<> doc;
    doc.parse<0>(&xmlCopy[0]);

    xml_node<>* root = doc.first_node("svg");
    if (!root)
        return Shapes;

    for (xml_node<>* node = root->first_node(); node; node = node->next_sibling())
    {

        string tagName = node->name();

        if (tagName == "defs" || tagName == "style" || tagName == "metadata")
        {
            continue;
        }

        shape* s = ParseNode(node);

        if (s)
        {
            if (group* g = dynamic_cast<group*>(s))
            {
                g->applyAttribute();
			}
            Shapes.push_back(s);
        }

    }

    return Shapes;
}

shape* SVGParser::ParseNode(xml_node<>* node) {
    string tagName = node->name();

    map<string, string> attributes;
    for (xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
    {
        string attrName = attr->name();
        string attrValue = attr->value();

        attributes[attrName] = attrValue;
    }


    if (tagName == "g") {
        group* g = new group();

		g->setAttrs(attributes);

        if (attributes.count("transform")) {
            g->setTransform(attributes.at("transform"));
        }

        if (attributes.count("id")) {
            g->setID(attributes.at("id"));
		}

        for (xml_node<>* childNode = node->first_node(); childNode; childNode = childNode->next_sibling())
        {
            shape* childShape = ParseNode(childNode);
            if (childShape)
            {
                g->addChild(childShape);
            }
        }
		g->applyAttribute();
        
        return g;
    }


    if (node->value() && strlen(node->value()) > 0)
    {
        attributes["content"] = node->value();
    }

    if (tagName == "text")
    {
        string textContent = "";
        if (node->value())
        {
            textContent = node->value();
        }
        attributes["text-content"] = textContent;
    }

    shape* s = ParseElement(tagName, attributes);

    if (s != nullptr)
    {
        if (tagName == "text")
        {
            text* textShape = dynamic_cast<text*>(s);
            if (textShape && attributes.count("text-content"))
            {
                textShape->setText(attributes["text-content"]);
            }
        }
    }
    return s;
}

shape* SVGParser::ParseElement(const string& tag, const map<string, string>& attrs)
{
    if (tag.empty())
    {
        return nullptr;
    }

    return factory::createShape(tag, attrs);
}