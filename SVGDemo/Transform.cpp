#include "stdafx.h"
#include "Transform.h"

using namespace std;

Transform::Transform(float sx, float sy, float rot, float tx, float ty)
    : scaleX(sx), scaleY(sy),
    rotation(rot), translateX(tx), translateY(ty), matrix(nullptr), useMatrix(false) {}

Transform::~Transform() {
    if (matrix) {
        delete matrix;
        matrix = nullptr;
    }
}

void Transform::Apply(Gdiplus::Graphics& g) const
{
    /* g.ResetTransform();

     Gdiplus::Matrix m;

     m.Scale(scaleX, scaleY, Gdiplus::MatrixOrderAppend);
     m.Rotate(rotation, Gdiplus::MatrixOrderAppend);
     m.Translate(translateX, translateY, Gdiplus::MatrixOrderAppend);

     g.MultiplyTransform(&m, Gdiplus::MatrixOrderAppend);*/

    if (useMatrix && matrix) {
        g.MultiplyTransform(matrix);
    }
    else {
        //1. Translate
        if (translateX != 0.0f || translateY != 0.0f) {
            g.TranslateTransform(translateX, translateY);
        }

        //2. Rotate
        if (rotation != 0.0f) {
            g.RotateTransform(rotation);

        }
        //3. Scale
        if (scaleX != 1.0f || scaleY != 1.0f) {
            g.ScaleTransform(scaleX, scaleY);
        }
    }
}

//Gdiplus::Matrix Transform::ToMatrix() const
//{
//    Gdiplus::Matrix m;
//    m.Scale(scaleX, scaleY, Gdiplus::MatrixOrderAppend);
//    m.Rotate(rotation, Gdiplus::MatrixOrderAppend);
//    m.Translate(translateX, translateY, Gdiplus::MatrixOrderAppend);
//    // Do NOT return a copy of m (which would invoke the copy constructor).
//    // Instead, return m by value (which uses the public default constructor and assignment).
//    return &m;
//}

void Transform::setScaleX(float sx) {
    this->scaleX = sx;
}

void Transform::setScaleY(float sy) {
    this->scaleY = sy;
}

void Transform::setRotation(float rot) {
    this->rotation = rot;
}

void Transform::setRotation(float rot, float cx, float cy) {
    if (!matrix) {
        matrix = new Gdiplus::Matrix();
	}

    matrix->Translate(cx, cy);
	matrix->Rotate(rot);
    matrix->Translate(-cx, -cy);
	useMatrix = true;
}

void Transform::setTranslateX(float tx) {
    this->translateX = tx;
}

void Transform::setTranslateY(float ty) {
    this->translateY = ty;
}

void Transform::setMatrix(float m11, float m12, float m21, float m22, float dx, float dy) {
    if (!matrix) {
        matrix = new Gdiplus::Matrix();
    }
    matrix->SetElements(m11, m12, m21, m22, dx, dy);
    useMatrix = true;
}

Transform* Transform::parse(const std::string& str) {
    if (str.empty()) return nullptr;

	Transform* transform = new Transform();

	vector<string> tokens = split(str);

    for (const string& token : tokens) {
        if (token.find("scale") == 0) {
            float sx = 1.0f, sy = 1.0f;
            sscanf_s(token.c_str(), "scale(%f %f)", &sx, &sy);
            transform->setScaleX(sx);
            transform->setScaleY(sy);
        }
        else if (token.find("rotate") == 0) {
            float angle = 0.0f;
            sscanf_s(token.c_str(), "rotate(%f)", &angle);
            transform->setRotation(angle);
        }
        else if (token.find("translate") == 0) {
            float tx = 0.0f, ty = 0.0f;
            sscanf_s(token.c_str(), "translate(%f %f)", &tx, &ty);
            transform->setTranslateX(tx);
            transform->setTranslateY(ty);
        }
        else if (token.find("matrix") == 0) {
            float m11, m12, m21, m22, dx, dy;
            sscanf_s(token.c_str(), "matrix(%f %f %f %f %f %f)", &m11, &m12, &m21, &m22, &dx, &dy);
            transform->setMatrix(m11, m12, m21, m22, dx, dy);
		}
    }

	return transform;
}

vector<string> split(const string& str) {
    vector<string> tokens;
    size_t start = 0;
	size_t end = str.find(')');
    while (end != string::npos) {
        string token = str.substr(start, end - start + 1);
        tokens.push_back(token);
        start = end + 2;
        end = str.find(')', start);
	}
   
	return tokens;
}
