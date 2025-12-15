#include "stdafx.h"

#include "Color.h"

#include <fstream>
#include <sstream>
#include <algorithm>

Color parseColor(const string& str) {

    Color rgb;

    if (str.find("rgb") != string::npos) {
        rgb = parseRGBColor(str);
    }
    else if (str.find("#") != string::npos) {
        rgb = parseHexColor(str);
    }
    else {
		rgb = parseNamedColor(str);
    }
    return rgb;
	
}

Color ColorWithOpacity(const Color& fill_color, float fill_opacity) {
    BYTE r = fill_color.GetR();
    BYTE g = fill_color.GetG();
    BYTE b = fill_color.GetB();
    BYTE opacity = static_cast<BYTE>(fill_opacity * 255);
    return Color(opacity, r, g, b);
}

Color parseRGBColor(const string& str) {
    size_t start = str.find("(");
    size_t end = str.find(")");
    string content = str.substr(start + 1, end - start - 1);
    replace(content.begin(), content.end(), ',', ' ');
    istringstream iss(content);
    int r, g, b;
    iss >> r >> g >> b;
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
    return Color(static_cast<BYTE>(r), static_cast<BYTE>(g), static_cast<BYTE>(b));
}

Color parseHexColor(const string& str) {
    string hex = str.substr(1); // Remove '#'
    unsigned int rgb = 0;
    if (hex.length() == 3) {
        // Short form (#RGB)
        rgb = (stoi(hex.substr(0, 1), nullptr, 16) << 20) |
              (stoi(hex.substr(0, 1), nullptr, 16) << 16) |
              (stoi(hex.substr(1, 1), nullptr, 16) << 12) |
              (stoi(hex.substr(1, 1), nullptr, 16) << 8) |
              (stoi(hex.substr(2, 1), nullptr, 16) << 4) |
              (stoi(hex.substr(2, 1), nullptr, 16));
    }
    else if (hex.length() == 6) {
        // Full form (#RRGGBB)
        rgb = stoi(hex, nullptr, 16);
    }
    BYTE r = (rgb >> 16) & 0xFF;
    BYTE g = (rgb >> 8) & 0xFF;
    BYTE b = rgb & 0xFF;
    return Color(r, g, b);
}

Color parseNamedColor(const string& str) {
    ifstream in("rgb.txt");
    string colorName = str;
	transform(colorName.begin(), colorName.end(), colorName.begin(), ::tolower);

    string line;
	//Parse the rgb.txt file format "name rgb(r g b)"
    while (getline(in, line)) {
		stringstream ss(line);
        string name;
        
        ss >> name;
        ss.ignore(5);


        if (name == colorName) {
            int r, g, b;
			ss >> r >> g >> b;
            ss.ignore(1);
            return Color(r, g, b);
        }
	}
    in.close();
	return Color(0, 0, 0);
}

