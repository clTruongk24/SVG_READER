#include "stdafx.h"
#include "Gradient.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include "Defs.h"

using namespace std;
using namespace Gdiplus;

// --------------------------------------------------------------
// Class: GradientStop
// Purpose: Represents a single color stop inside a gradient.
//  - offset: position along the gradient [0..1]
//  - color: GDI+ color value for the stop
//  - opacity: alpha in range [0..1]
// --------------------------------------------------------------
GradientStop::GradientStop() : offset(0.0f), color(Color::Black), opacity(1.0f) {}

GradientStop::GradientStop(float off, const Gdiplus::Color& col, float op)
	: offset(off), color(col), opacity(op) {
}

// Set stop position (0..1)
void GradientStop::setOffset(float off) {
	offset = off;
}

// Get stop position
float GradientStop::getOffset() const {
	return offset;
}

// Set stop color
void GradientStop::setColor(const Gdiplus::Color& col) {
	color = col;
}

// Get stop color
Gdiplus::Color GradientStop::getColor() const {
	return color;
}

// Set stop opacity (0..1)
void GradientStop::setOpacity(float op) {
	opacity = op;
}

// Get stop opacity
float GradientStop::getOpacity() const {
	return opacity;
}


 // --------------------------------------------------------------
 // Class: GradientBase
 // Purpose: Base for gradient types (linear, radial).
 //  - Manages id, list of stops, spreadMethod (pad/reflect/repeat),
 //    gradientUnits (objectBoundingBox/userSpaceOnUse), and optional transform.
 //  - Supports inheriting properties via xlink:href.
 // --------------------------------------------------------------
GradientBase::GradientBase()
	: id(""), spreadMethod("pad"), gradientUnits("objectBoundingBox"), transform(nullptr) {
}

GradientBase::~GradientBase() {
	if (transform) {
		delete transform;
	}
	stops.clear();
}

// Set gradient id
void GradientBase::setId(const string& id) {
	this->id = id;
}

// Get gradient id
string GradientBase::getId() const {
	return id;
}

// Add a stop and keep stops sorted by offset
void GradientBase::addStop(const GradientStop& stop) {
	stops.push_back(stop);

	sort(stops.begin(), stops.end(), [](const GradientStop& a, const GradientStop& b) {
			return a.getOffset() < b.getOffset();
		});
}

// Convenience to create and add a stop
void GradientBase::addStop(float offset, const Gdiplus::Color& color, float opacity) {
	GradientStop stop(offset, color, opacity);
	addStop(stop);
}

// Set spread method: pad/reflect/repeat
void GradientBase::setSpreadMethod(const string& method) {
	this->spreadMethod = method;
}

// Get spread method
string GradientBase::getSpreadMethod() const {
	return spreadMethod;
}

// Get gradientUnits
string GradientBase::getGradientUnits() const {
	return gradientUnits;
}

// Set gradientUnits
void GradientBase::setGradientUnits(const string& units) {
	this->gradientUnits = units;
}

// Set gradientTransform by copying a Transform
void GradientBase::setTransform(const Transform& t) {
	if (transform) {
		delete transform;
	}
	transform = new Transform(t);
}

// Get stops (copy)
vector<GradientStop> GradientBase::getStops() const {
	return stops;
}

// Set full stop list and sort by offset
void GradientBase::setStops(const vector<GradientStop>& stopList) {
	stops = stopList;
	sort(stops.begin(), stops.end(), [](const GradientStop& a, const GradientStop& b) {
			return a.getOffset() < b.getOffset();
		});
}

// Inherit stops/transform/method/units from referenced gradient via xlink:href/#id
void GradientBase::handleHref(const string& href) {
	if (href.empty() || href[0] != '#') {
		return;
	}

	string refId = href.substr(1);
	GradientBase* refGradient = Defs::getInstance().getGradient(refId);

	if (refGradient) {
		if (stops.empty()) {
			stops = refGradient->getStops();
		}

		if (transform == nullptr && refGradient->getTransform() != nullptr) {
			transform = new Transform(*refGradient->getTransform());
		}

		if (spreadMethod == "pad" && refGradient->getSpreadMethod() != "pad") {
			spreadMethod = refGradient->getSpreadMethod();
		}

		if (gradientUnits == "objectBoundingBox" && refGradient->getGradientUnits() != "objectBoundingBox") {
			gradientUnits = refGradient->getGradientUnits();
		}
	}
}

// Get gradientTransform pointer (can be nullptr)
Transform* GradientBase::getTransform() const {
	return transform;
}

// --------------------------------------------------------------
// Class: LinearGradient
// Purpose: Implements a linear gradient from (x1,y1) to (x2,y2).
//  - Coordinates are interpreted per gradientUnits (objectBoundingBox/userSpaceOnUse).
//  - Uses SetInterpolationColors for multiple stops.
//  - Applies gradientTransform if present.
// --------------------------------------------------------------
LinearGradient::LinearGradient()
	: x1(0.0f), y1(0.0f), x2(1.0f), y2(0.0f) {
}

void LinearGradient::setX1(float x) {
	x1 = x;
}

float LinearGradient::getX1() const {
	return x1;
}

void LinearGradient::setY1(float y) {
	y1 = y;
}

float LinearGradient::getY1() const {
	return y1;
}

void LinearGradient::setX2(float x) {
	x2 = x;
}

float LinearGradient::getX2() const {
	return x2;
}

void LinearGradient::setY2(float y) {
	y2 = y;
}

float LinearGradient::getY2() const {
	return y2;
}

// createBrush (LinearGradient):
//  - If two stops: use direct color1/color2 constructor.
//  - If >2 stops: call SetInterpolationColors with arrays of colors/positions.
//  - For objectBoundingBox, convert x1/y1/x2/y2 from percentage to pixel using bounds.
//  - Multiply brush by gradientTransform if present.
Brush* LinearGradient::createBrush(const Gdiplus::RectF& bounds)  {
	if (stops.empty()) {
		return new SolidBrush(Color::Black);
	}

	if (stops.size() == 1) {
		Color col = stops[0].getColor();
		BYTE alpha = static_cast<BYTE>(stops[0].getOpacity() * 255);
		return new SolidBrush(Color(alpha, col.GetR(), col.GetG(), col.GetB()));
	}

	float startX, startY, endX, endY;

	// Convert coordinates per gradientUnits
	if (gradientUnits == "objectBoundingBox") {
		startX = bounds.X + x1 * bounds.Width;
		startY = bounds.Y + y1 * bounds.Height;
		endX = bounds.X + x2 * bounds.Width;
		endY = bounds.Y + y2 * bounds.Height;
	} else {
		startX = x1;
		startY = y1;
		endX = x2;
		endY = y2;
	}

	PointF startPoint(startX, startY);
	PointF endPoint(endX, endY);

	LinearGradientBrush* brush = nullptr;

	if (stops.size() == 2) {
		// Two stops: simple linear gradient
		Color c1 = stops[0].getColor();
		Color c2 = stops[1].getColor();
		BYTE a1 = static_cast<BYTE>(stops[0].getOpacity() * 255);
		BYTE a2 = static_cast<BYTE>(stops[1].getOpacity() * 255);

		Color color1(a1, c1.GetR(), c1.GetG(), c1.GetB());
		Color color2(a2, c2.GetR(), c2.GetG(), c2.GetB());

		brush = new LinearGradientBrush(
			startPoint,
			endPoint,
			color1,
			color2
		);
	}
	else {
		// Multiple stops: interpolate colors along positions
		int n = (int)stops.size();
		Color* colors = new Color[n];
		REAL* positions = new REAL[n];

		for (int i = 0; i < n; ++i) {
			Color c = stops[i].getColor();
			BYTE a = static_cast<BYTE>(stops[i].getOpacity() * 255);
			colors[i] = Color(a, c.GetR(), c.GetG(), c.GetB());
			positions[i] = stops[i].getOffset();
		}

		brush = new LinearGradientBrush(
			startPoint,
			endPoint,
			colors[0],
			colors[n - 1]
		);

		brush->SetInterpolationColors(colors, positions, n);

		delete[] colors;
		delete[] positions;
	}

	// Apply gradientTransform if any
	if (transform && transform->getMatrix()) {
		brush->MultiplyTransform(transform->getMatrix());
	}

	return brush;
}

// --------------------------------------------------------------
// Class: RadialGradient
// Purpose: Implements radial gradient with center (cx,cy), radius r,
//  optional focal point (fx,fy). Supports gradientUnits/transform/spread.
//  The per-pixel bitmap renderer approximates SVG radial behavior closely.
//  A cached bitmap is owned to avoid re-render where possible.
// --------------------------------------------------------------
RadialGradient::RadialGradient()
    : cx(0.5f), cy(0.5f), r(0.5f), fx(0.5f), fy(0.5f),
      hasFocal(false), cachedBitmap(nullptr) {
}

RadialGradient::~RadialGradient() {
    // Free cached bitmap to avoid memory leaks
    if (cachedBitmap) {
        delete cachedBitmap;
        cachedBitmap = nullptr;
    }
}

// Setters/getters for radial parameters
void RadialGradient::setCX(float x) {
	cx = x;
}

float RadialGradient::getCX() const {
	return cx;
}

void RadialGradient::setCY(float y) {
	cy = y;
}

float RadialGradient::getCY() const {
	return cy;
}

void RadialGradient::setR(float radius) {
	r = radius;
}

float RadialGradient::getR() const {
	return r;
}

void RadialGradient::setFX(float x) {
	fx = x;
}

float RadialGradient::getFX() const {
	return fx;
}

void RadialGradient::setFY(float y) {
	fy = y;
}

float RadialGradient::getFY() const {
	return fy;
}

// createBrush (RadialGradient):
//  - Computes transformed center/focal/radius using getTransformedCoordinates.
//  - Renders a bitmap per-pixel and returns a TextureBrush positioned to bounds.
//  - Handles trivial cases with SolidBrush.
Brush* RadialGradient::createBrush(const Gdiplus::RectF& bounds)  {
    if (stops.empty()) {
        return new SolidBrush(Color::Black);
    }
    if (stops.size() == 1) {
        Color col = stops[0].getColor();
        BYTE alpha = static_cast<BYTE>(stops[0].getOpacity() * 255);
        return new SolidBrush(Color(alpha, col.GetR(), col.GetG(), col.GetB()));
    }

    float centerX, centerY, radius, focalX, focalY;
    getTransformedCoordinates(centerX, centerY, radius, focalX, focalY, bounds);

    if (radius <= 0.001f) {
        Color col = stops.back().getColor();
        BYTE a = static_cast<BYTE>(stops.back().getOpacity() * 255);
        return new SolidBrush(Color(a, col.GetR(), col.GetG(), col.GetB()));
    }

    int w = static_cast<int>(std::ceil(bounds.Width));
    int h = static_cast<int>(std::ceil(bounds.Height));
    if (w <= 0 || h <= 0) {
        Color col = stops.back().getColor();
        BYTE a = static_cast<BYTE>(stops.back().getOpacity() * 255);
        return new SolidBrush(Color(a, col.GetR(), col.GetG(), col.GetB()));
    }

    const int MAX_DIM = 4096;
    if (w > MAX_DIM || h > MAX_DIM) {
        float scale = min(static_cast<float>(MAX_DIM) / w, static_cast<float>(MAX_DIM) / h);
        w = static_cast<int>(w * scale);
        h = static_cast<int>(h * scale);
    }

    Bitmap* bmp = new Bitmap(w, h, PixelFormat32bppARGB);
    Rect lockRect(0, 0, w, h);
    BitmapData data;

    if (bmp->LockBits(&lockRect, ImageLockModeWrite, PixelFormat32bppARGB, &data) != Ok) {
        delete bmp;
        Color col = stops.back().getColor();
        BYTE a = static_cast<BYTE>(stops.back().getOpacity() * 255);
        return new SolidBrush(Color(a, col.GetR(), col.GetG(), col.GetB()));
    }

    BYTE* pixels = reinterpret_cast<BYTE*>(data.Scan0);
    const int stride = data.Stride;

    const float localFocalX = focalX - bounds.X;
    const float localFocalY = focalY - bounds.Y;
    const float localCenterX = centerX - bounds.X;
    const float localCenterY = centerY - bounds.Y;

    // Per-pixel render: offset is distance(focal->pixel)/radius
    for (int y = 0; y < h; ++y) {
        BYTE* row = pixels + y * stride;
        float py = bounds.Y + y;

        for (int x = 0; x < w; ++x) {
            float px = bounds.X + x; 

            float dx = px - focalX;
            float dy = py - focalY;
            float dist = std::sqrt(dx * dx + dy * dy);

            float offset = dist / radius;

            Color c = getColorAtOffset(offset);

            BYTE* p = row + x * 4;
            p[0] = c.GetB();
            p[1] = c.GetG();
            p[2] = c.GetR();
            p[3] = c.GetA();
        }
    }

    bmp->UnlockBits(&data);

    TextureBrush* textureBrush = new TextureBrush(bmp, WrapModeClamp);

    textureBrush->TranslateTransform(bounds.X, bounds.Y);

    if (spreadMethod == "reflect") {
        textureBrush->SetWrapMode(WrapModeTileFlipXY);
    } else if (spreadMethod == "repeat") {
        textureBrush->SetWrapMode(WrapModeTile);
    }

    return textureBrush;
}

// Mark gradient as having a focal point
void RadialGradient::setHasFocal(bool has) {
	hasFocal = has;
}

// Query focal flag
bool RadialGradient::getHasFocal() const {
	return hasFocal;
}

// Compute color at a normalized offset using stops and spreadMethod
Color RadialGradient::getColorAtOffset(float offset) const {
	if (stops.empty()) return Color::Black;

	if (stops.size() == 1) {
		Color c = stops[0].getColor();
		BYTE a = static_cast<BYTE>(stops[0].getOpacity() * 255);
		return Color(a, c.GetR(), c.GetG(), c.GetB());
	}

	// Apply spreadMethod normalization
	float originalOffset = offset;
	if (spreadMethod == "pad") {
		offset = max(0.0f, min(1.0f, offset));
	}
	else if (spreadMethod == "reflect") {
		if (offset < 0) offset = -offset;
		offset = fmod(offset, 2.0f);
		if (offset > 1.0f) offset = 2.0f - offset;
	}
	else if (spreadMethod == "repeat") {
		offset = fmod(offset, 1.0f);
		if (offset < 0) offset += 1.0f;
	}

	// Edge cases
	if (offset <= stops[0].getOffset()) {
		Color c = stops[0].getColor();
		BYTE a = static_cast<BYTE>(stops[0].getOpacity() * 255);
		return Color(a, c.GetR(), c.GetG(), c.GetB());
	}

	if (offset >= stops[stops.size() - 1].getOffset()) {
		Color c = stops[stops.size() - 1].getColor();
		BYTE a = static_cast<BYTE>(stops[stops.size() - 1].getOpacity() * 255);
		return Color(a, c.GetR(), c.GetG(), c.GetB());
	}

	// Interpolate between surrounding stops
	for (size_t i = 0; i < stops.size() - 1; i++) {
		if (offset >= stops[i].getOffset() && offset <= stops[i + 1].getOffset()) {
			float t = (offset - stops[i].getOffset()) /
				(stops[i + 1].getOffset() - stops[i].getOffset());

			Color c1 = stops[i].getColor();
			Color c2 = stops[i + 1].getColor();
			float a1 = stops[i].getOpacity();
			float a2 = stops[i + 1].getOpacity();

			BYTE r = (BYTE)(c1.GetR() * (1 - t) + c2.GetR() * t);
			BYTE g = (BYTE)(c1.GetG() * (1 - t) + c2.GetG() * t);
			BYTE b = (BYTE)(c1.GetB() * (1 - t) + c2.GetB() * t);
			BYTE a = (BYTE)((a1 * (1 - t) + a2 * t) * 255);

			return Color(a, r, g, b);
		}
	}

	return Color::Black;
}

// Compute world-space center/focal/radius based on gradientUnits and transform
void RadialGradient::getTransformedCoordinates(float& outCX, float& outCY, float& outR,
	float& outFX, float& outFY,
	const RectF& bounds) const {

	if (gradientUnits == "userSpaceOnUse" && transform && transform->getMatrix()) {
		PointF center(cx, cy);
		PointF points[] = { center };
		transform->getMatrix()->TransformPoints(points, 1);
		outCX = points[0].X;
		outCY = points[0].Y;

		if (hasFocal) {
			PointF focal(fx, fy);
			PointF focalPoints[] = { focal };
			transform->getMatrix()->TransformPoints(focalPoints, 1);
			outFX = focalPoints[0].X;
			outFY = focalPoints[0].Y;
		}
		else {
			outFX = outCX;
			outFY = outCY;
		}

		PointF radiusVec(r, 0);
		PointF radiusVecs[] = { radiusVec };
		transform->getMatrix()->TransformVectors(radiusVecs, 1);
		outR = sqrt(radiusVecs[0].X * radiusVecs[0].X +
			radiusVecs[0].Y * radiusVecs[0].Y);
	}
	else if (gradientUnits == "objectBoundingBox") {
		float mappedCX = bounds.X + cx * bounds.Width;
		float mappedCY = bounds.Y + cy * bounds.Height;
		// Map r relative to bbox size (approximate circle in bbox)
		float mappedR = r * sqrt(bounds.Width * bounds.Width +
			bounds.Height * bounds.Height) / sqrt(2.0f);
		float mappedFX = hasFocal ? (bounds.X + fx * bounds.Width) : mappedCX;
		float mappedFY = hasFocal ? (bounds.Y + fy * bounds.Height) : mappedCY;

		if (transform && transform->getMatrix()) {
			PointF center(mappedCX, mappedCY);
			PointF points[] = { center };
			transform->getMatrix()->TransformPoints(points, 1);
			outCX = points[0].X;
			outCY = points[0].Y;

			PointF focal(mappedFX, mappedFY);
			PointF focalPoints[] = { focal };
			transform->getMatrix()->TransformPoints(focalPoints, 1);
			outFX = focalPoints[0].X;
			outFY = focalPoints[0].Y;

			PointF radiusVec(mappedR, 0);
			PointF radiusVecs[] = { radiusVec };
			transform->getMatrix()->TransformVectors(radiusVecs, 1);
			outR = sqrt(radiusVecs[0].X * radiusVecs[0].X +
				radiusVecs[0].Y * radiusVecs[0].Y);
		}
		else {
			outCX = mappedCX;
			outCY = mappedCY;
			outR = mappedR;
			outFX = mappedFX;
			outFY = mappedFY;
		}
	}
	else {
		outCX = cx;
		outCY = cy;
		outR = r;
		outFX = hasFocal ? fx : cx;
		outFY = hasFocal ? fy : cy;
	}

	if (outR < 0.1f) {
		outR = 0.1f;
	}
}