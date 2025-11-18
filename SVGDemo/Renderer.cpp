
#include "Renderer.h"

void Draw(const std::vector<Shape*>& shapes, Graphics* g)
{
    for (auto shape : shapes) {
        if (!shape) continue;
        Draw(*shape, g);
    }
}

void Draw(const Shape& shape, Graphics* g)
{
    if (!g) return;

    Pen pen(shape.stroke_color, shape.stroke_width);
    SolidBrush brush(shape.fill_color);

    Matrix mtx;
    shape.transform.apply(mtx);
    g->SetTransform(&mtx);
    ///Vẽ hình 
    ///...
    ///
    Matrix identity;
    g->SetTransform(&identity);
}

void SetTransform(const Transform& transform, Graphics* g)
{
    if (!g) return;
    Matrix mtx;
    transform.apply(mtx);
    g->SetTransform(&mtx);
}
