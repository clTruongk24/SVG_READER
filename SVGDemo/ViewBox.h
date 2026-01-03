#ifndef VIEWBOX_H
#define VIEWBOX_H

#include <string>

struct viewBox {
	float minX;
	float minY;
	float width;
	float height;
	bool hasViewBox;
	float viewPortWidth;
	float viewPortHeight;
	std::string preserveAspectRatio;

	viewBox() {
		minX = minY = width = height = viewPortHeight = viewPortWidth = 0.0f;
		hasViewBox = false;
		preserveAspectRatio = "xMidYMid meet";
	}
};

void applyViewBoxTransform(Gdiplus::Graphics&, const viewBox&, const Gdiplus::Size&);


#endif // !VIEWBOX_H
