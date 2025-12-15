#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <gdiplus.h>
#include <string>
#include <vector>

class Transform
{
private:
    float scaleX;
    float scaleY;
    float rotation;
    float translateX;
    float translateY;

	Gdiplus::Matrix* matrix;
    bool useMatrix;

public:
    Transform(float sx = 1.0f, float sy = 1.0f,
              float rot = 0.0f,
              float tx = 0.0f, float ty = 0.0f);

    virtual ~Transform();

    // Áp dụng transform lên Graphics
    void Apply(Gdiplus::Graphics& g) const;

    // Hỗ trợ
    //Gdiplus::Matrix ToMatrix() const;

    // Getter
    float getScaleX() const { return scaleX; }
    float getScaleY() const { return scaleY; }
    float getRotation() const { return rotation; }
    float getTranslateX() const { return translateX; }
    float getTranslateY() const { return translateY; }

    void setScaleX(float);
	void setScaleY(float);
	void setRotation(float);
	void setRotation(float, float, float);
	void setTranslateX(float);
    void setTranslateY(float);
	void setMatrix(float, float, float, float, float, float);

    void switchTransform(const std::string&, const std::vector<float>&);

	static Transform* parse(const std::string&);
};

std::vector<std::string> split(const std::string&);
std::vector<float> parseParams(const std::string&);

#endif // !TRANSFORM_H