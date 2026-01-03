#ifndef PATH_H
#define PATH_H

#include "Shape.h"
#include <vector>
#include <gdiplus.h>

class path : public shape {
private:
    Gdiplus::GraphicsPath* gPath;
    float currentX;
    float currentY;

	float lastControlX;
	float lastControlY;
	bool hadControlPoint;

	float lastQuadControlX;
	float lastQuadControlY;
	bool hadQuadControlPoint;

	float subPathStartX;
	float subPathStartY;

    void parseD(const string& d);

	void handleMoveTo(istream& , bool );
	void handleLineTo(istream& , bool );
	void handleHorizontalLineTo(istream& , bool );
	void handleVerticalLineTo(istream& , bool );
	void handleCurveTo(istream& , bool );
	void handleSmoothCurveTo(istream& , bool );
	void handleQuadraticCurveTo(istream& , bool );
	void handleSmoothQuadraticCurveTo(istream& , bool );
	void handleEllipticalArc(istream& , bool );
	void handleClosePath();

	void addQuadraticBezier(float, float, float, float);
	void addArc(float, float, float, bool, bool, float, float);


public:
    path();
    ~path();

	RectF getBounds() const override;

    void setD(const string& d);

    void draw(Graphics& graphics) override;

};

#endif // PATH_H