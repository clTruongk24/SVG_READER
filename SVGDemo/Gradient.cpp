#include "stdafx.h"
#include "Gradient.h"
#include <algorithm>
#include <cmath>
#include "Defs.h"

using namespace std;
using namespace Gdiplus;

// --------------------------------------------------------------
// GradientStop
// --------------------------------------------------------------
GradientStop::GradientStop() : offset(0.0f), color(Color::Black), opacity(1.0f) {}
GradientStop::GradientStop(float off, const Gdiplus::Color& col, float op)
    : offset(off), color(col), opacity(op) {}

void GradientStop::setOffset(float off) { offset = off; }
float GradientStop::getOffset() const { return offset; }
void GradientStop::setColor(const Gdiplus::Color& col) { color = col; }
Gdiplus::Color GradientStop::getColor() const { return color; }
void GradientStop::setOpacity(float op) { opacity = op; }
float GradientStop::getOpacity() const { return opacity; }

// --------------------------------------------------------------
// GradientBase
// --------------------------------------------------------------
GradientBase::GradientBase()
    : id(""), spreadMethod("pad"), gradientUnits("objectBoundingBox"), transform(nullptr),
      stops_set(false), spread_set(false), units_set(false), transform_set(false) {}

GradientBase::~GradientBase() {
    if (transform) delete transform;
}

void GradientBase::setId(const string& id) { this->id = id; }
string GradientBase::getId() const { return id; }

void GradientBase::addStop(const GradientStop& stop) {
    stops.push_back(stop);
    sort(stops.begin(), stops.end(), [](const GradientStop& a, const GradientStop& b) {
        return a.getOffset() < b.getOffset();
    });
    stops_set = true;
}

void GradientBase::addStop(float offset, const Gdiplus::Color& color, float opacity) {
    addStop(GradientStop(offset, color, opacity));
}

void GradientBase::setStops(const vector<GradientStop>& stopList) {
    stops = stopList;
    sort(stops.begin(), stops.end(), [](const GradientStop& a, const GradientStop& b) {
        return a.getOffset() < b.getOffset();
    });
    stops_set = true;
}

vector<GradientStop> GradientBase::getStops() const { return stops; }

void GradientBase::setSpreadMethod(const string& method) { spreadMethod = method; spread_set = true; }
string GradientBase::getSpreadMethod() const { return spreadMethod; }

void GradientBase::setGradientUnits(const string& units) { gradientUnits = units; units_set = true; }
string GradientBase::getGradientUnits() const { return gradientUnits; }

void GradientBase::setTransform(const Transform& t) {
    if (transform) delete transform;
    transform = new Transform(t);
    transform_set = true;
}

Transform* GradientBase::getTransform() const { return transform; }

void GradientBase::handleHref(const string& href) {
    if (href.empty() || href[0] != '#') return;
    string refId = href.substr(1);
    GradientBase* ref = Defs::getInstance().getGradient(refId);
    if (!ref) return;

    if (!stops_set && !ref->getStops().empty()) { stops = ref->getStops(); stops_set = true; }
    if (!spread_set) spreadMethod = ref->getSpreadMethod();
    if (!units_set) gradientUnits = ref->getGradientUnits();
    if (!transform_set && ref->getTransform()) { transform = new Transform(*ref->getTransform()); transform_set = true; }
}

// --------------------------------------------------------------
// LinearGradient
// --------------------------------------------------------------
LinearGradient::LinearGradient()
    : x1(0.0f), y1(0.0f), x2(1.0f), y2(0.0f),
      x1_set(false), y1_set(false), x2_set(false), y2_set(false) {}

void LinearGradient::setX1(float x) { x1 = x; x1_set = true; }
void LinearGradient::setY1(float y) { y1 = y; y1_set = true; }
void LinearGradient::setX2(float x) { x2 = x; x2_set = true; }
void LinearGradient::setY2(float y) { y2 = y; y2_set = true; }

void LinearGradient::handleHref(const string& href) {
    GradientBase::handleHref(href);
    if (href.empty() || href[0] != '#') return;
    LinearGradient* ref = dynamic_cast<LinearGradient*>(Defs::getInstance().getGradient(href.substr(1)));
    if (!ref) return;

    if (!x1_set) { x1 = ref->x1; x1_set = ref->x1_set; }
    if (!y1_set) { y1 = ref->y1; y1_set = ref->y1_set; }
    if (!x2_set) { x2 = ref->x2; x2_set = ref->x2_set; }
    if (!y2_set) { y2 = ref->y2; y2_set = ref->y2_set; }
}

Brush* LinearGradient::createBrush(const Gdiplus::RectF& bounds) {
    if (stops.empty()) return new SolidBrush(Color::Black);
    if (stops.size() == 1) {
        Color c = stops[0].getColor();
        return new SolidBrush(Color(static_cast<BYTE>(stops[0].getOpacity() * 255), c.GetR(), c.GetG(), c.GetB()));
    }

    // SVG Spec transformation order for objectBoundingBox:
    // 1. Unit coordinates [0,1] are transformed by gradientTransform
    // 2. Then mapped to objectBoundingBox
    // P_user = M_bboxMapping * M_gradTransform * P_unit
    Matrix totalMat;
    if (transform && transform->getMatrix()) {
        totalMat.Multiply(transform->getMatrix(), MatrixOrderAppend);
    }
    if (gradientUnits == "objectBoundingBox") {
        Matrix bboxMat;
        bboxMat.Scale(bounds.Width, bounds.Height, MatrixOrderAppend);
        bboxMat.Translate(bounds.X, bounds.Y, MatrixOrderAppend);
        totalMat.Multiply(&bboxMat, MatrixOrderAppend);
    }

    PointF p1_world(x1, y1), p2_world(x2, y2);
    PointF pts[2] = {p1_world, p2_world};
    totalMat.TransformPoints(pts, 2);
    p1_world = pts[0]; p2_world = pts[1];

    float dx = p2_world.X - p1_world.X;
    float dy = p2_world.Y - p1_world.Y;
    float dist = sqrt(dx * dx + dy * dy);
    if (dist < 0.001f) {
        Color c = stops.back().getColor();
        return new SolidBrush(Color(static_cast<BYTE>(stops.back().getOpacity() * 255), c.GetR(), c.GetG(), c.GetB()));
    }

    // Expand to avoid tiling
    float factor = 1000.0f;
    PointF p1_huge(p1_world.X - dx * factor, p1_world.Y - dy * factor);
    PointF p2_huge(p2_world.X + dx * factor, p2_world.Y + dy * factor);

    LinearGradientBrush* brush = new LinearGradientBrush(p1_huge, p2_huge, Color::Black, Color::Black);

    std::vector<Color> colors;
    std::vector<REAL> positions;
    float totalDist = dist * (2 * factor + 1);
    float startOffset = (dist * factor) / totalDist;
    float scale = dist / totalDist;

    colors.push_back(Color(static_cast<BYTE>(stops[0].getOpacity() * 255), 
                           stops[0].getColor().GetR(), stops[0].getColor().GetG(), stops[0].getColor().GetB()));
    positions.push_back(0.0f);
    for (const auto& s : stops) {
        Color c = s.getColor();
        colors.push_back(Color(static_cast<BYTE>(s.getOpacity() * 255), c.GetR(), c.GetG(), c.GetB()));
        positions.push_back(startOffset + s.getOffset() * scale);
    }
    colors.push_back(colors.back());
    positions.push_back(1.0f);

    brush->SetInterpolationColors(colors.data(), positions.data(), (int)colors.size());
    brush->SetWrapMode(WrapModeTile); 

    return brush;
}

// --------------------------------------------------------------
// RadialGradient
// --------------------------------------------------------------
RadialGradient::RadialGradient()
    : cx(0.5f), cy(0.5f), r(0.5f), fx(0.5f), fy(0.5f), hasFocal(false),
      cx_set(false), cy_set(false), r_set(false), fx_set(false), fy_set(false), hasFocal_set(false) {}

RadialGradient::~RadialGradient() {}

void RadialGradient::setCX(float x) { cx = x; cx_set = true; if (!fx_set) fx = x; }
void RadialGradient::setCY(float y) { cy = y; cy_set = true; if (!fy_set) fy = y; }
void RadialGradient::setR(float radius) { r = radius; r_set = true; }
void RadialGradient::setFX(float x) { fx = x; fx_set = true; hasFocal = true; }
void RadialGradient::setFY(float y) { fy = y; fy_set = true; hasFocal = true; }
void RadialGradient::setHasFocal(bool has) { hasFocal = has; hasFocal_set = true; }

void RadialGradient::handleHref(const string& href) {
    GradientBase::handleHref(href);
    if (href.empty() || href[0] != '#') return;
    string refId = href.substr(1);
    RadialGradient* ref = dynamic_cast<RadialGradient*>(Defs::getInstance().getGradient(refId));
    if (!ref) return;

    if (!cx_set) { cx = ref->cx; cx_set = ref->cx_set; }
    if (!cy_set) { cy = ref->cy; cy_set = ref->cy_set; }
    if (!r_set) { r = ref->r; r_set = ref->r_set; }
    if (!hasFocal_set) {
        hasFocal = ref->hasFocal;
        fx = ref->fx; fy = ref->fy;
        hasFocal_set = ref->hasFocal_set;
        fx_set = ref->fx_set; fy_set = ref->fy_set;
    }
}

Brush* RadialGradient::createBrush(const Gdiplus::RectF& bounds) {
    if (stops.empty()) return new SolidBrush(Color::Black);
    if (stops.size() == 1) {
        Color c = stops[0].getColor();
        return new SolidBrush(Color(static_cast<BYTE>(stops[0].getOpacity() * 255), c.GetR(), c.GetG(), c.GetB()));
    }

    // Transformation order: P_user = M_bboxMapping * M_gradTransform * P_unit
    Matrix totalMat;
    if (transform && transform->getMatrix()) {
        totalMat.Multiply(transform->getMatrix(), MatrixOrderAppend);
    }
    if (gradientUnits == "objectBoundingBox") {
        Matrix bboxMat;
        bboxMat.Scale(bounds.Width, bounds.Height, MatrixOrderAppend);
        bboxMat.Translate(bounds.X, bounds.Y, MatrixOrderAppend);
        totalMat.Multiply(&bboxMat, MatrixOrderAppend);
    }

    // Dynamic Expansion Logic: ensure the gradient circle covers the entire shape
    Matrix invMat;
    invMat.Multiply(&totalMat);
    if (invMat.Invert() != Ok) return new SolidBrush(Color::Black);

    PointF corners[4] = {
        PointF(bounds.X, bounds.Y), PointF(bounds.X + bounds.Width, bounds.Y),
        PointF(bounds.X, bounds.Y + bounds.Height), PointF(bounds.X + bounds.Width, bounds.Y + bounds.Height)
    };
    invMat.TransformPoints(corners, 4);

    float maxDist = r;
    for (int i = 0; i < 4; ++i) {
        float dx = corners[i].X - cx, dy = corners[i].Y - cy;
        float d = sqrt(dx * dx + dy * dy);
        if (d > maxDist) maxDist = d;
    }
    float R_eff = maxDist * 1.1f; // Use expanded radius

    GraphicsPath path;
    path.AddEllipse(cx - R_eff, cy - R_eff, R_eff * 2, R_eff * 2);
    PathGradientBrush* pgb = new PathGradientBrush(&path);

    // Focal point clamping (against original radius r to preserve gradient slope)
    float effFX = hasFocal ? fx : cx;
    float effFY = hasFocal ? fy : cy;
    float fdx = effFX - cx, fdy = effFY - cy;
    float fdist = sqrt(fdx * fdx + fdy * fdy);
    if (fdist > r * 0.99f) {
        float ratio = (r * 0.99f) / fdist;
        effFX = cx + fdx * ratio;
        effFY = cy + fdy * ratio;
    }
    pgb->SetCenterPoint(PointF(effFX, effFY));

    std::vector<Color> colors;
    std::vector<REAL> positions;
    
    // GDI+ PathGradientBrush: position 0 = BOUNDARY(R_eff), position 1 = CENTER
    // SVG stops 0..1 map to center..boundary(r)
    // Map SVG offset x to GDI+ pos = 1.0 - (x * r / R_eff)
    for (int i = (int)stops.size() - 1; i >= 0; --i) {
        Color c = stops[i].getColor();
        colors.push_back(Color(static_cast<BYTE>(stops[i].getOpacity() * 255), c.GetR(), c.GetG(), c.GetB()));
        positions.push_back(1.0f - (stops[i].getOffset() * r / R_eff));
    }
    
    // Pad the outer region (from r to R_eff) with the last color (position 0)
    if (positions[0] > 0.0f) {
        colors.insert(colors.begin(), colors[0]);
        positions.insert(positions.begin(), 0.0f);
    }
    // Ensure center is 1.0
    if (positions.back() < 1.0f) {
        colors.push_back(colors.back());
        positions.push_back(1.0f);
    }

    pgb->SetInterpolationColors(colors.data(), positions.data(), (int)colors.size());
    pgb->SetCenterColor(colors.back());
    
    Color boundaryColor = colors[0];
    int sCount = path.GetPointCount();
    std::vector<Color> surroundColors(sCount, boundaryColor);
    pgb->SetSurroundColors(surroundColors.data(), &sCount);

    pgb->SetTransform(&totalMat);

    if (spreadMethod == "reflect") pgb->SetWrapMode(WrapModeTileFlipXY);
    else if (spreadMethod == "repeat") pgb->SetWrapMode(WrapModeTile);
    else pgb->SetWrapMode(WrapModeClamp);

    return pgb;
}
