#ifndef COLOR_H
#define COLOR_H

#include "stdafx.h"
#include <gdiplus.h>
#include <string>

using namespace std;
using namespace Gdiplus;

Color parseColor(const string&);
Color ColorWithOpacity(const Color&, float);

#endif // !COLOR_H
