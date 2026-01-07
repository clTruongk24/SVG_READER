#ifndef GRADIENT_H
#define GRADIENT_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "Transform.h"

class GradientStop {
public:
    GradientStop();
    GradientStop(float off, const Gdiplus::Color& col, float op);

    void setOffset(float off);
    float getOffset() const;
    void setColor(const Gdiplus::Color& col);
    Gdiplus::Color getColor() const;
    void setOpacity(float op);
    float getOpacity() const;

private:
    float offset;
    Gdiplus::Color color;
    float opacity;
};

class GradientBase {
public:
    GradientBase();
    virtual ~GradientBase();

    void setId(const std::string& id);
    std::string getId() const;

    void addStop(const GradientStop& stop);
    void addStop(float offset, const Gdiplus::Color& color, float opacity);
    void setStops(const std::vector<GradientStop>& stopList);
    std::vector<GradientStop> getStops() const;

    void setSpreadMethod(const std::string& method);
    std::string getSpreadMethod() const;

    void setGradientUnits(const std::string& units);
    std::string getGradientUnits() const;

    void setTransform(const Transform& t);
    Transform* getTransform() const;

    virtual void handleHref(const std::string& href);
    virtual Gdiplus::Brush* createBrush(const Gdiplus::RectF& bounds) = 0;

protected:
    std::string id;
    std::vector<GradientStop> stops;
    std::string spreadMethod;
    std::string gradientUnits;
    Transform* transform;

    // Inheritance flags
    bool stops_set;
    bool spread_set;
    bool units_set;
    bool transform_set;
};

class LinearGradient : public GradientBase {
public:
    LinearGradient();

    void setX1(float x);
    void setY1(float y);
    void setX2(float x);
    void setY2(float y);

    void handleHref(const std::string& href) override;
    Gdiplus::Brush* createBrush(const Gdiplus::RectF& bounds) override;

private:
    float x1, y1, x2, y2;
    bool x1_set, y1_set, x2_set, y2_set;
};

class RadialGradient : public GradientBase {
public:
    RadialGradient();
    ~RadialGradient();

    void setCX(float x);
    void setCY(float y);
    void setR(float radius);
    void setFX(float x);
    void setFY(float y);
    void setHasFocal(bool has);

    void handleHref(const std::string& href) override;
    Gdiplus::Brush* createBrush(const Gdiplus::RectF& bounds) override;

private:
    float cx, cy, r, fx, fy;
    bool hasFocal;
    bool cx_set, cy_set, r_set, fx_set, fy_set, hasFocal_set;
};

#endif
