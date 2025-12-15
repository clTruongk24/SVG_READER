#include "stdafx.h"
#include "Text.h"
#include "Color.h"

text::text() {
	x = y = 0;
	font_size = 16;
	text_ = "";
	font_family = "Times New Roman";
	text_anchor = "start";
	italic = false;
	dx = dy = 0;
}

//void text::draw(Graphics& graphics) {
//	if (text_.empty()) return;
//
//	GraphicsState state = graphics.Save();
//
//	if (transform) {
//		transform->Apply(graphics);
//	}
//
//	SolidBrush brush(ColorWithOpacity(fill_color, fill_opacity));
//	Pen pen(ColorWithOpacity(stroke_color, stroke_opacity), stroke_width);
//
//	wstring wfont_family(font_family.begin(), font_family.end());
//	FontFamily fontFamily(wfont_family.c_str());
//
//	INT style = (italic) ? FontStyleItalic : FontStyleRegular;
//	Font font(&fontFamily, font_size, style, UnitPixel);
//
//	wstring wText(text_.begin(), text_.end());
//
//	float text_x = x + dx;
//	float text_y = y + dy;
//
//	RectF boundingBox;
//	graphics.MeasureString(wText.c_str(), -1, &font, PointF(0, 0), &boundingBox);
//
//	if (text_anchor == "middle") {
//		text_x -= boundingBox.Width / 2;
//	}
//	else if (text_anchor == "end") {
//		text_x -= boundingBox.Width;
//	}
//
//	float ascent = font.GetHeight(&graphics) * fontFamily.GetCellAscent(style) / fontFamily.GetEmHeight(style);
//	text_y -= ascent;
//
//	graphics.DrawString(wText.c_str(), -1, &font, PointF(text_x, text_y), &brush);
//
//	if (stroke_width > 0) {
//		GraphicsPath path;
//
//		RectF layoutRect(0, 0, 10000, 10000);  
//		StringFormat format;
//		format.SetAlignment(StringAlignmentNear);
//
//		path.AddString(wText.c_str(), -1, &fontFamily, style, font_size, PointF(text_x, text_y), NULL);
//		graphics.DrawPath(&pen, &path);
//	}
//
//	graphics.Restore(state);
//}

void text::draw(Graphics& graphics) {
    if (text_.empty()) return;

    GraphicsState state = graphics.Save();

    if (transform) {
        transform->Apply(graphics);
    }

    SolidBrush brush(ColorWithOpacity(fill_color, fill_opacity));

    string cleanFontFamily = font_family;
    size_t commaPos = cleanFontFamily.find(',');

    if (commaPos != string::npos) {
        cleanFontFamily = cleanFontFamily.substr(0, commaPos);
    }

    size_t start = cleanFontFamily.find_first_not_of(" \t");
    size_t end = cleanFontFamily.find_last_not_of(" \t");

    if (start != string::npos && end != string::npos) {
        cleanFontFamily = cleanFontFamily.substr(start, end - start + 1);
    }

    wstring wfont_family(cleanFontFamily.begin(), cleanFontFamily.end());

    FontFamily* pFontFamily = new FontFamily(wfont_family.c_str());
    if (pFontFamily->GetLastStatus() != Ok) {
        delete pFontFamily;
        pFontFamily = new FontFamily(L"Times New Roman");
    }

    INT style = (italic) ? FontStyleItalic : FontStyleRegular;
    Font font(pFontFamily, font_size, style, UnitPixel);

    wstring wText(text_.begin(), text_.end());

    float text_x = x + dx;
    float text_y = y + dy;

    RectF boundingBox;
    graphics.MeasureString(wText.c_str(), -1, &font, PointF(0, 0), &boundingBox);

    if (text_anchor == "middle") {
        text_x -= boundingBox.Width / 2;
    }
    else if (text_anchor == "end") {
        text_x -= boundingBox.Width;
    }

    float ascent = font.GetHeight(&graphics) * pFontFamily->GetCellAscent(style) / pFontFamily->GetEmHeight(style);
    text_y -= ascent;

    graphics.DrawString(wText.c_str(), -1, &font, PointF(text_x, text_y), &brush);

    if (stroke_width > 0) {
        Pen pen(ColorWithOpacity(stroke_color, stroke_opacity), stroke_width);
        GraphicsPath path;
        path.AddString(wText.c_str(), -1, pFontFamily, style, font_size,
            PointF(text_x, text_y), NULL);
        graphics.DrawPath(&pen, &path);
    }

    delete pFontFamily;
    graphics.Restore(state);
}


float text::getFontSize() const {
	return font_size;
}

void text::setX(float x) {
	this->x = x;
}

void text::setY(float y) {
	this->y = y;
}

void text::setFontSize(float size) {
	this->font_size = size;
}

void text::setText(const string& text) {
	this->text_ = text;
}

void text::setFontFamily(const string& family) {
	this->font_family = family;
}

void text::setTextAnchor(const string& anchor) {
	this->text_anchor = anchor;
}

void text::setItalic(bool isItalic) {
	this->italic = isItalic;
}

void text::setDX(float dx) {
	this->dx = dx;
}

void text::setDY(float dy) {
	this->dy = dy;
}