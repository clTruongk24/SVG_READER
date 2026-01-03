#include "stdafx.h"
#include "ViewBox.h"
#include <algorithm>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;


void applyViewBoxTransform(Graphics& graphics, const viewBox& vb, const Size& clientSize) {
    if (!vb.hasViewBox) return;

    float viewPortW = (vb.viewPortWidth > 0) ? vb.viewPortWidth : static_cast<float>(clientSize.Width);
    float viewPortH = (vb.viewPortHeight > 0) ? vb.viewPortHeight : static_cast<float>(clientSize.Height);

    if (viewPortW <= 0 || viewPortH <= 0 || vb.width <= 0 || vb.height <= 0) return;

    float scaleX = viewPortW / vb.width;
    float scaleY = viewPortH / vb.height;
    float scale = scaleX, translateX = 0.0f, translateY = 0.0f;

    string par = vb.preserveAspectRatio;
    par.erase(remove(par.begin(), par.end(), '\n'), par.end());
    par.erase(remove(par.begin(), par.end(), '\r'), par.end());

    bool none = (par.find("none") != string::npos);
    bool slice = (par.find("slice") != string::npos);
    bool meet(!none && !slice);

    if (none) {
        graphics.TranslateTransform(-vb.minX, -vb.minY);
        graphics.ScaleTransform(scaleX, scaleY);
        return;
    }

    scale = (slice) ? max(scaleX, scaleY) : min(scaleX, scaleY);

    string align = par;
    size_t spacePos = align.find(' ');
    if (spacePos != string::npos) {
        align = par.substr(0, spacePos);
    }
    if (align.empty()) {
        align = "xMidYMid";
    }

    float viewPortX = 0.0f;
    float viewPortY = 0.0f;

    float scaledWidth = vb.width * scale;
    float scaledHeight = vb.height * scale;

    if (align.find("xMin") != string::npos) {
        viewPortX = 0.0f;
    }
    else if (align.find("xMid") != string::npos) {
        viewPortX = (viewPortW - scaledWidth) / 2.0f;
    }
    else if (align.find("xMax") != string::npos) {
        viewPortX = viewPortW - scaledWidth;
    }

    if (align.find("YMin") != string::npos || align.find("yMin") != string::npos) {
        viewPortY = 0.0f;
    }
    else if (align.find("YMid") != string::npos || align.find("yMid") != string::npos) {
        viewPortY = (viewPortH - scaledHeight) / 2.0f;
    }
    else if (align.find("YMax") != string::npos || align.find("yMax") != string::npos) {
        viewPortY = viewPortH - scaledHeight;
    }

    graphics.TranslateTransform(viewPortX, viewPortY);
    graphics.ScaleTransform(scale, scale);
    graphics.TranslateTransform(-vb.minX, -vb.minY);
}