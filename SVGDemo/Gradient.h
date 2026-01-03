#ifndef GRADIENT_H
#define GRADIENT_H

#include <vector>
#include <string>
#include <gdiplus.h>
#include "Transform.h"

class GradientStop
{
private:
	float offset;
	Gdiplus::Color color;
	float opacity;
public:
	GradientStop();
	GradientStop(float, const Gdiplus::Color&, float);

	void setOffset(float);
	float getOffset() const;

	void setColor(const Gdiplus::Color&);
	Gdiplus::Color getColor() const;

	void setOpacity(float);
	float getOpacity() const;
};

class GradientBase {
protected:
	std::string id;
	std::vector<GradientStop> stops;
	std::string spreadMethod; // pad, reflect, repeat
	std::string gradientUnits; // userSpaceOnUse, objectBoundingBox
	Transform* transform;

public:
	GradientBase();
	virtual ~GradientBase();
	
	void setId(const std::string&);
	std::string getId() const;

	void addStop(const GradientStop&);
	void addStop(float, const Gdiplus::Color&, float);

	void setSpreadMethod(const std::string&);
	std::string getSpreadMethod() const;

	void setGradientUnits(const std::string&);
	std::string getGradientUnits() const;

	void setTransform(const Transform&);
	Transform* getTransform() const;

	void setStops(const std::vector<GradientStop>&);
	std::vector<GradientStop> getStops() const;

	void handleHref(const std::string&);

	virtual Gdiplus::Brush* createBrush(const Gdiplus::RectF& )  = 0;
};

class LinearGradient : public GradientBase {
private:
	float x1;
	float y1;
	float x2;
	float y2;
public:
	LinearGradient();
	
	void setX1(float);
	float getX1() const;

	void setY1(float);
	float getY1() const;

	void setX2(float);
	float getX2() const;

	void setY2(float);
	float getY2() const;

	virtual Gdiplus::Brush* createBrush(const Gdiplus::RectF&)  override;
};

class RadialGradient : public GradientBase {
private:
    float cx, cy, r, fx, fy;
    bool hasFocal;

    Gdiplus::Bitmap* cachedBitmap;
    std::string cacheKey;

	std::string generateCacheKey(const Gdiplus::RectF&, float, float, float, float, float) const;
	Gdiplus::Brush* createApproximateRadialGradient(const Gdiplus::RectF&, float, float, float, float, float);

	Gdiplus::Color getColorAtOffset(float) const;

	void getTransformedCoordinates(float&, float&, float&, float&, float&, const Gdiplus::RectF&) const;
public:
    RadialGradient();
    ~RadialGradient(); 

    void setCX(float);
    float getCX() const;

    void setCY(float);
    float getCY() const;

    void setR(float);
    float getR() const;

    void setFX(float);
    float getFX() const;

    void setFY(float);
    float getFY() const;

    void setHasFocal(bool);
    bool getHasFocal() const;

    void clearCache();

    virtual Gdiplus::Brush* createBrush(const Gdiplus::RectF&) override;
};

#endif // !GRADIENT_H
