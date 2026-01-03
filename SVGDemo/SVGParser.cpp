#include "stdafx.h"
#include "SVGParser.h"
#include "Factory.h"    // To call CreateShape from ShapeFactory
#include "Shape.h"      // To use Shape*
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include "Text.h"
#include "Group.h"
#include "Color.h"

using namespace std;
using namespace rapidxml;

SVGParser::SVGParser() {}
SVGParser::~SVGParser() {}

vector<shape*> SVGParser::ParseFile(const string& filePath)
{
    vector<shape*> Shapes;

    Defs::getInstance().clear();

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

    parseViewBox(root);

    for (xml_node<>* node = root->first_node(); node; node = node->next_sibling())
    {
        if (string(node->name()) == "defs")
        {
            parseDefs(node);
        }
        else if (string(node->name()) == "style")
        {
            parseStyles(node);
        }
	}


    for (xml_node<>* node = root->first_node(); node; node = node->next_sibling())
    {

        string tagName = node->name();

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

    if (tagName == "style") {
        map<string, string> styleAttrs = Defs::parseStyleString(attributes["style"]);

        for (const auto& pair : styleAttrs) {
			attributes[pair.first] = pair.second;
        }
    }

    if (attributes.count("class")) {
        string className = attributes["class"];
		string selector = "." + className;

        if (Defs::getInstance().hasStyle(selector)) {
			string classStyle = Defs::getInstance().getStyle(selector);
			map<string, string> classAttrs = Defs::parseStyleString(classStyle);

            for (const auto& pair : classAttrs) {
                if (!attributes.count(pair.first)) {
					attributes[pair.first] = pair.second;
                }
            }
        }
    }

   

    if (tagName == "g") {
        group* g = new group();
        g->setAttrs(attributes);

        if (attributes.count("transform")) {
            Transform* t = Transform::parse(attributes.at("transform"));
            if (t) {
                g->setTransform(t);  
            }
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

void SVGParser::parseDefs(xml_node<>* defsNode)
{
    if (!defsNode)
		return;

    for (xml_node<>* node = defsNode->first_node(); node; node = node->next_sibling())
    {
        string tagName = node->name();

        if (tagName == "linearGradient") {
			GradientBase* lg = parseLinearGradient(node);
            if (lg) {
				Defs::getInstance().addGradient(lg);
            }
		}
        else if (tagName == "radialGradient") {
			GradientBase* rg = parseRadialGradient(node);
            if (rg) {
				Defs::getInstance().addGradient(rg);
            }
        }
        else if (tagName == "style") {
            parseStyles(node);
        }
        
	}
}

void SVGParser::parseStyles(xml_node<>* styleNode)
{
    if (!styleNode)
        return;

    string styleContent = styleNode->value();

	styleContent.erase(remove(styleContent.begin(), styleContent.end(), '\n'), styleContent.end());
	styleContent.erase(remove(styleContent.begin(), styleContent.end(), '\r'), styleContent.end());

    size_t pos = 0;

    while (pos < styleContent.length()) {

        size_t braceOpen = styleContent.find('{', pos);
        if (braceOpen == string::npos) break;

        string selector = styleContent.substr(pos, braceOpen - pos);

        selector.erase(0, selector.find_first_not_of(" \t"));
        selector.erase(selector.find_last_not_of(" \t") + 1);

        size_t braceClose = styleContent.find('}', braceOpen);
        if (braceClose == string::npos) break;

        string style = styleContent.substr(braceOpen + 1, braceClose - braceOpen - 1);

        style.erase(0, style.find_first_not_of(" \t"));
        style.erase(style.find_last_not_of(" \t") + 1);

        if (!selector.empty() && !style.empty()) {
            Defs::getInstance().addStyle(selector, style);
        }

        pos = braceClose + 1;
    }
}

GradientBase* SVGParser::parseLinearGradient(xml_node<>* lgNode)
{
    if (!lgNode)
		return nullptr;

	LinearGradient* lg = new LinearGradient();
    string href;

    for (xml_attribute<>* attr = lgNode->first_attribute(); attr; attr = attr->next_attribute())
    {
        string attrName = attr->name();
        string attrValue = attr->value();

        if (attrName == "id") {
            lg->setId(attrValue);
        }
        else if (attrName == "x1") {
            lg->setX1(parseCoordinate(attrValue));
        }
        else if (attrName == "y1") {
            lg->setY1(parseCoordinate(attrValue));
        }
        else if (attrName == "x2") {
            lg->setX2(parseCoordinate(attrValue));
        }
        else if (attrName == "y2") {
            lg->setY2(parseCoordinate(attrValue));
        }
        else if (attrName == "gradientUnits") {
            lg->setGradientUnits(attrValue);
        }
        else if (attrName == "spreadMethod") {
            lg->setSpreadMethod(attrValue);
        }
        else  if (attrName == "gradientTransform") {
            Transform* t = Transform::parse(attrValue);
            if (t) {
                lg->setTransform(*t);
                delete t;
            }
        }
        else if (attrName == "xlink:href" || attrName == "href") {
			href = attrValue;
        }
	}
    parseStops(lgNode, lg);
    
    if (!href.empty()) {
        lg->handleHref(href);

    }

    return lg;
}

GradientBase* SVGParser::parseRadialGradient(xml_node<>* rgNode)
{
    if (!rgNode)
        return nullptr;
    
	RadialGradient* rg = new RadialGradient();
    string href;

    for (xml_attribute<>* attr = rgNode->first_attribute(); attr; attr = attr->next_attribute())
    {
        string attrName = attr->name();
        string attrValue = attr->value();

        if (attrName == "id") {
            rg->setId(attrValue);
        }
        else if (attrName == "cx") {
            rg->setCX(parseCoordinate(attrValue));
        }
        else if (attrName == "cy") {
            rg->setCY(parseCoordinate(attrValue));
        }
        else if (attrName == "r") {
            rg->setR(parseCoordinate(attrValue));
        }
        else if (attrName == "fx") {
            rg->setFX(parseCoordinate(attrValue));
            rg->setHasFocal(true);
        }
        else if (attrName == "fy") {
            rg->setFY(parseCoordinate(attrValue));
            rg->setHasFocal(true);
        }
        else if (attrName == "gradientUnits") {
            rg->setGradientUnits(attrValue);
        }
        else if (attrName == "spreadMethod") {
            rg->setSpreadMethod(attrValue);
        }
        else if (attrName == "gradientTransform") {
            Transform* t = Transform::parse(attrValue);
            if (t) {
                rg->setTransform(*t);
                delete t;
            }
        }
        else if (attrName == "xlink:href" || attrName == "href") {
			href = attrValue;
        }
	}
    parseStops(rgNode, rg);

    if (!href.empty()) {
        rg->handleHref(href);
	}
    
	return rg;
}

void SVGParser::parseStops(xml_node<>* gradientNode, GradientBase* gradient)
{
    if (!gradientNode || !gradient)
        return;
    
    for (xml_node<>* stopNode = gradientNode->first_node("stop"); stopNode; stopNode = stopNode->next_sibling("stop"))
    {
        float offset = 0.0f;
        Color color = Color::Black;
        float opacity = 1.0f;

        for (xml_attribute<>* attr = stopNode->first_attribute(); attr; attr = attr->next_attribute())
        {
            string attrName = attr->name();
            string attrValue = attr->value();

            if (attrName == "offset") {
                string offsetStr = attrValue;

                if (offsetStr.find('%') != string::npos) {
                    offsetStr = offsetStr.substr(0, offsetStr.find('%'));
                    offset = stof(offsetStr) / 100.0f;
                }
                else {
                    offset = stof(offsetStr);
                }
            }
            else if (attrName == "stop-color") {
                color = parseColor(attrValue);
            } else if (attrName == "stop-opacity") {
                opacity = stof(attrValue);
            }
            else if (attrName == "style") {
				map<string, string> styleAttrs = Defs::parseStyleString(attrValue);
                if (styleAttrs.count("stop-color")) {
                    color = parseColor(styleAttrs["stop-color"]);
                }
                if (styleAttrs.count("stop-opacity")) {
                    opacity = stof(styleAttrs["stop-opacity"]);
                }
            }
        }
        gradient->addStop(offset, color, opacity);
	}
}

void SVGParser::parseViewBox(xml_node<>* svgNode)
{
    svgViewBox = viewBox();

    for (xml_attribute<>* attr = svgNode->first_attribute(); attr; attr = attr->next_attribute())
    {
        string attrName = attr->name();
        string attrValue = attr->value();

        if (attrName == "viewBox") {
            string s = attrValue;
            replace(s.begin(), s.end(), ',', ' ');
            stringstream ss(s);
            float minX, minY, width, height;
            if (ss >> minX >> minY >> width >> height) {
                svgViewBox.minX = minX;
                svgViewBox.minY = minY;
                svgViewBox.width = width;
                svgViewBox.height = height;
                svgViewBox.hasViewBox = true;
            }
        }
        else if (attrName == "width") {
            string widthStr = attrValue;
            size_t pos = widthStr.find_first_not_of("0123456789.+-");
            if (pos != string::npos) {
                widthStr = widthStr.substr(0, pos);
            }
            if (!widthStr.empty()) {
                svgViewBox.viewPortWidth = static_cast<float>(stof(widthStr));
            }
        }
        else if (attrName == "height") {
            string heightStr = attrValue;
            size_t pos = heightStr.find_first_not_of("0123456789.+-");
            if (pos != string::npos) {
                heightStr = heightStr.substr(0, pos);
            }
            if (!heightStr.empty()) {
                svgViewBox.viewPortHeight = static_cast<float>(stof(heightStr));
            }
        }
        else if (attrName == "preserveAspectRatio") {
			svgViewBox.preserveAspectRatio = attrValue;
        }
    }
}


float parseCoordinate(const std::string& val) {
    if (val.empty()) return 0.0f;

    if (val.back() != '%') {
        return stof(val);
    }
    
	string percentStr = val.substr(0, val.length() - 1);
	return stof(percentStr) / 100.0f;
}

const viewBox& SVGParser::getViewBox() const
{
    return svgViewBox;
}