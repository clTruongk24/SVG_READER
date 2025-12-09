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

    void parseD(const string& d);

public:
    path();
    ~path();

    void setD(const string& d);

    void draw(Graphics& graphics) override;
};

#endif // PATH_H