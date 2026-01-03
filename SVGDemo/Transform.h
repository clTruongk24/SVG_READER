//#ifndef TRANSFORM_H
//#define TRANSFORM_H
//#include <gdiplus.h>
//#include <string>
//#include <vector>
//
//class Transform
//{
//private:
//    float scaleX;
//    float scaleY;
//    float rotation;
//    float translateX;
//    float translateY;
//
//    Gdiplus::Matrix* matrix;
//    bool useMatrix;
//
//public:
//    Transform(float sx = 1.0f, float sy = 1.0f,
//        float rot = 0.0f,
//        float tx = 0.0f, float ty = 0.0f);
//
//    virtual ~Transform();
//
//    // Áp dụng transform lên Graphics
//    void Apply(Gdiplus::Graphics& g) const;
//
//    // Hỗ trợ
//    //Gdiplus::Matrix ToMatrix() const;
//
//    // Getter
//    float getScaleX() const { return scaleX; }
//    float getScaleY() const { return scaleY; }
//    float getRotation() const { return rotation; }
//    float getTranslateX() const { return translateX; }
//    float getTranslateY() const { return translateY; }
//
//    void setScaleX(float);
//    void setScaleY(float);
//    void setRotation(float);
//    void setRotation(float, float, float);
//    void setTranslateX(float);
//    void setTranslateY(float);
//    void setMatrix(float, float, float, float, float, float);
//
//    void switchTransform(const std::string&, const std::vector<float>&);
//
//    static Transform* parse(const std::string&);
//};
//
//std::vector<std::string> split(const std::string&);
//std::vector<float> parseParams(const std::string&);
//
//#endif // !TRANSFORM_H

#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <gdiplus.h>
#include <string>
#include <vector>

class Transform
{
private:
    Gdiplus::Matrix* matrix;
    bool useMatrix;

public:
    Transform();
    Transform(const Transform&);
    virtual ~Transform();

    void Apply(Gdiplus::Graphics& g) const;

    // Transform operations - these build the matrix
    void addTranslate(float tx, float ty);
    void addScale(float sx, float sy);
    void addRotate(float angle);
    void addRotate(float angle, float cx, float cy);
    void addMatrix(float m11, float m12, float m21, float m22, float dx, float dy);
    void addSkewX(float angle);
    void addSkewY(float angle);

    static Transform* parse(const std::string& transformStr);

    Gdiplus::Matrix* getMatrix() const;

    Transform& operator=(const Transform&);

private:
    void ensureMatrix();
    void parseTransformFunction(const std::string& func, const std::vector<float>& params);
};

std::vector<std::string> splitTransforms(const std::string& str);
std::vector<float> parseTransformParams(const std::string& str);

#endif // !TRANSFORM_H