#pragma once
#include <gdiplus.h>

class Transform
{
private:
    float scaleX;
    float scaleY;
    float rotation;
    float translateX;
    float translateY;

public:
    Transform(float sx = 1.0f, float sy = 1.0f,
              float rot = 0.0f,
              float tx = 0.0f, float ty = 0.0f);

    // Áp dụng transform lên Graphics
    void Apply(Gdiplus::Graphics& g) const;

    // Hỗ trợ
    Gdiplus::Matrix ToMatrix() const;

    // Getter
    float getScaleX() const { return scaleX; }
    float getScaleY() const { return scaleY; }
    float getRotation() const { return rotation; }
    float getTranslateX() const { return translateX; }
    float getTranslateY() const { return translateY; }
};
