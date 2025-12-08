#include "Transform.h"
#include <cmath>

Transform::Transform(float sx, float sy, float rot, float tx, float ty)
    : scaleX(sx), scaleY(sy),
      rotation(rot), translateX(tx), translateY(ty)
{
}

void Transform::Apply(Gdiplus::Graphics& g) const
{
    g.ResetTransform();

    Gdiplus::Matrix m;

    m.Scale(scaleX, scaleY, Gdiplus::MatrixOrderAppend);
    m.Rotate(rotation, Gdiplus::MatrixOrderAppend);
    m.Translate(translateX, translateY, Gdiplus::MatrixOrderAppend);

    g.MultiplyTransform(&m, Gdiplus::MatrixOrderAppend);
}

Gdiplus::Matrix Transform::ToMatrix() const
{
    Gdiplus::Matrix m;
    m.Scale(scaleX, scaleY, Gdiplus::MatrixOrderAppend);
    m.Rotate(rotation, Gdiplus::MatrixOrderAppend);
    m.Translate(translateX, translateY, Gdiplus::MatrixOrderAppend);
    return m;
}
