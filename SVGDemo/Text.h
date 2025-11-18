#ifndef TEXT_H
#define TEXT_H

#include "Shape.h"

class text : public shape {
private:
	float x;
	float y;
	float font_size;
	string text_;
	string font_family;
	string text_anchor;
	bool italic;
	float dx;
	float dy;
public:
	text();
	void draw(Graphics&) override;

	void setX(float);
	void setY(float);
	void setFontSize(float);
	void setText(const string&);
	void setFontFamily(const string&);
	void setTextAnchor(const string&);
	void setItalic(bool);
	void setDX(float);
	void setDY(float);
};
#endif // !TEXT_H
