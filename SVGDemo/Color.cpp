#include "stdafx.h"

#include "Color.h"

#include <fstream>
#include <sstream>
#include <algorithm>

Color parseColor(const string& str) {
	int r = 0, g = 0, b = 0;

	if (str.find("rgb") != string::npos) {
		sscanf_s(str.c_str(), "rgb(%d,%d,%d)", &r, &g, &b);
	} 
	else if (str.find("#") != string::npos) {
		unsigned int rgb = 0;
		sscanf_s(str.c_str(), "#%x", &rgb);
		r = (rgb >> 16) & 0xFF;
		g = (rgb >> 8) & 0xFF;
		b = rgb & 0xFF;
	}
	else {
        string temp = str;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        ifstream file("rgb.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string buffer, name, rgb;
                getline(ss, buffer, '"');
                getline(ss, name, '"');
                if (name == temp) {
                    getline(ss, buffer, '(');
                    ss >> r;
                    ss >> buffer;
                    ss >> g;
                    ss >> buffer;
                    ss >> b;
                    break;
                }
                else {
                    getline(ss, buffer, '}');
                }
            }
        }
        file.close();
	}
    return Color(r, g, b);
}

Color ColorWithOpacity(const Color& fill_color, float fill_opacity) {
    BYTE r = fill_color.GetR();
    BYTE g = fill_color.GetG();
    BYTE b = fill_color.GetB();
    BYTE opacity = static_cast<BYTE>(fill_opacity * 255);
    return Color(opacity, r, g, b);
}
