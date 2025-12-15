#ifndef COLOR_H
#define COLOR_H

#include "stdafx.h"
#include <gdiplus.h>
#include <string>

using namespace std;
using namespace Gdiplus;

Color parseColor(const string&);
Color ColorWithOpacity(const Color&, float);

Color parseRGBColor(const string&);
Color parseHexColor(const string&);
Color parseNamedColor(const string&);

#endif // !COLOR_H
