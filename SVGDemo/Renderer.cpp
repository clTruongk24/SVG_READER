#include "stdafx.h"
#include "Renderer.h"
#include "Shape.h"

Renderer::Renderer(Graphics& g) : graphics(g) {}



void Renderer::Draw(const std::vector<shape*>& shapes)
{
    for (auto shape : shapes) {
        if (shape) Draw(shape);
    }
}

void Renderer::Draw(shape* s)
{
    if (!s) return;
    
    s->draw(graphics);

    /*Pen pen(shape.stroke_color, shape.stroke_width);
    SolidBrush brush(shape.fill_color);

    Matrix mtx;
    shape.transform.apply(mtx);
    g->SetTransform(&mtx);
    ///Vẽ hình 
    ///...
    ///
    Matrix identity;
    g->SetTransform(&identity);*/
}

//void SetTransform(const Transform& transform, Graphics* g)
//{
//    if (!g) return;
//    Matrix mtx;
//    transform.apply(mtx);
//    g->SetTransform(&mtx);
//}
