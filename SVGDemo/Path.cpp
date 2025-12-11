#include "stdafx.h"
#include "Path.h"
#include "Color.h"
#include <sstream>
#include <algorithm> 

path::path() {
    gPath = new Gdiplus::GraphicsPath();
    currentX = 0.0f;
    currentY = 0.0f;
}

path::~path() {
    if (gPath) {
        delete gPath;
        gPath = nullptr;
    }
}

void path::setD(const string& d) {
    parseD(d);
}

void path::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

    Pen pen(stroke_color, stroke_width);

    if (stroke_opacity < 1.0f) {
        Color c = stroke_color;
        pen.SetColor(Color(static_cast<BYTE>(stroke_opacity * 255), c.GetR(), c.GetG(), c.GetB()));
    }

    graphics.DrawPath(&pen, gPath);

    if (fill_opacity > 0.0f) {
        SolidBrush brush(fill_color);
        Color c = fill_color;
        brush.SetColor(Color(static_cast<BYTE>(fill_opacity * 255), c.GetR(), c.GetG(), c.GetB()));
        graphics.FillPath(&brush, gPath);
    }

	graphics.Restore(state);
}

// -------------------------------------------------------------------------
// Parses the 'd' attribute of the <path> element.
// Implements SVG Path Data specification:
// Reference: http://www.w3.org/TR/SVG/paths.html#PathData
// Supported commands: M (MoveTo), L (LineTo), H (Horizontal), V (Vertical), 
//                     C (Cubic Bezier), Z (ClosePath).
// -------------------------------------------------------------------------

void path::parseD(const string& raw_d) {
    string d = raw_d;
    replace(d.begin(), d.end(), ',', ' ');

    stringstream ss(d);
    char cmd;

    gPath->Reset();

    while (ss >> cmd) {

        switch (cmd) {
        case 'M': case 'm': {
            float x, y;
            if (ss >> x >> y) {
                gPath->StartFigure();
                currentX = x;
                currentY = y;
            }
            break;
        }
        case 'L': case 'l': {
            float x, y;
            if (ss >> x >> y) {
                gPath->AddLine(currentX, currentY, x, y);
                currentX = x;
                currentY = y;
            }
            break;
        }
        case 'H': case 'h': { 
            float x;
            if (ss >> x) {
                gPath->AddLine(currentX, currentY, x, currentY);
                currentX = x;
            }
            break;
        }
        case 'V': case 'v': { 
            float y;
            if (ss >> y) {
                gPath->AddLine(currentX, currentY, currentX, y);
                currentY = y;
            }
            break;
        }
        case 'C': case 'c': { 
            float x1, y1, x2, y2, x3, y3;
            if (ss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3) {
                gPath->AddBezier(currentX, currentY, x1, y1, x2, y2, x3, y3);
                currentX = x3;
                currentY = y3;
            }
            break;
        }
        case 'Z': case 'z': { 
            gPath->CloseFigure();
            break;
        }
        default:
            break;
        }
    }
}