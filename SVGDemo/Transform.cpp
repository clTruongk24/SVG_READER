//#include "stdafx.h"
//#include "Transform.h"
//#include <sstream>
//#include <algorithm>
//
//using namespace std;
//
//Transform::Transform(float sx, float sy, float rot, float tx, float ty)
//    : scaleX(sx), scaleY(sy),
//    rotation(rot), translateX(tx), translateY(ty), matrix(nullptr), useMatrix(false) {
//}
//
//Transform::~Transform() {
//    if (matrix) {
//        delete matrix;
//        matrix = nullptr;
//    }
//}
//
//void Transform::Apply(Gdiplus::Graphics& g) const
//{
//    if (useMatrix && matrix) {
//        g.MultiplyTransform(matrix);
//    }
//    else {
//        //1. Translate
//        if (translateX != 0.0f || translateY != 0.0f) {
//            g.TranslateTransform(translateX, translateY);
//        }
//
//        //2. Rotate
//        if (rotation != 0.0f) {
//            g.RotateTransform(rotation);
//
//        }
//        //3. Scale
//        if (scaleX != 1.0f || scaleY != 1.0f) {
//            g.ScaleTransform(scaleX, scaleY);
//        }
//    }
//}
//
//void Transform::setScaleX(float sx) {
//    this->scaleX = sx;
//}
//
//void Transform::setScaleY(float sy) {
//    this->scaleY = sy;
//}
//
//void Transform::setRotation(float rot) {
//    this->rotation = rot;
//}
//
//void Transform::setRotation(float rot, float cx, float cy) {
//    if (!matrix) {
//        matrix = new Gdiplus::Matrix();
//    }
//
//    matrix->Translate(cx, cy);
//    matrix->Rotate(rot);
//    matrix->Translate(-cx, -cy);
//    useMatrix = true;
//}
//
//void Transform::setTranslateX(float tx) {
//    this->translateX = tx;
//}
//
//void Transform::setTranslateY(float ty) {
//    this->translateY = ty;
//}
//
//void Transform::setMatrix(float m11, float m12, float m21, float m22, float dx, float dy) {
//    if (!matrix) {
//        matrix = new Gdiplus::Matrix();
//    }
//    matrix->SetElements(m11, m12, m21, m22, dx, dy);
//    useMatrix = true;
//}
//
//Transform* Transform::parse(const std::string& transformStr) {
//    if (transformStr.empty()) {
//        return nullptr;
//    }
//
//    Transform* transform = new Transform();
//
//    vector<string> tokens = split(transformStr);
//
//    for (auto& token : tokens) {
//        int start = token.find('(');
//        int end = token.find(')');
//        if (start == string::npos || end == string::npos || end <= start) continue;
//        string cmd = token.substr(0, start);
//        string paramsStr = token.substr(start + 1, end - start - 1);
//
//        vector<float> params = parseParams(paramsStr);
//
//        transform->switchTransform(cmd, params);
//    }
//
//    return transform;
//}
//
//void Transform::switchTransform(const string& cmd, const vector<float>& params) {
//    if (cmd == "translate" && (params.size() == 1 || params.size() == 2)) {
//        float tx = params[0];
//        float ty = (params.size() == 2) ? params[1] : 0.0f;
//        setTranslateX(tx);
//        setTranslateY(ty);
//    }
//    else if (cmd == "scale" && (params.size() == 1 || params.size() == 2)) {
//        float sx = params[0];
//        float sy = (params.size() == 2) ? params[1] : sx;
//        setScaleX(sx);
//        setScaleY(sy);
//    }
//    else if (cmd == "rotate" && (params.size() == 1 || params.size() == 3)) {
//        float angle = params[0];
//        if (params.size() == 3) {
//            float cx = params[1];
//            float cy = params[2];
//            setRotation(angle, cx, cy);
//        }
//        else {
//            setRotation(angle);
//        }
//    }
//    else if (cmd == "matrix" && params.size() == 6) {
//        setMatrix(params[0], params[1], params[2], params[3], params[4], params[5]);
//    }
//}
//
//vector<float> parseParams(const string& paramsStr) {
//    vector<float> params;
//    istringstream ss(paramsStr);
//    string param;
//    while (ss >> param) {
//        params.push_back(stof(param));
//    }
//    return params;
//}
//
//vector<string> split(const string& str) {
//    vector<string> tokens;
//
//    int i = 0;
//    while (i < str.length()) {
//
//        i = str.find_first_not_of(" \t\n", i);
//        if (i == string::npos) break;
//
//        int j = str.find_first_of(" (", i);
//        if (j == string::npos) break;
//
//        int k = str.find_first_of(")", j);
//        if (k == string::npos) break;
//
//        string cmd = str.substr(i, k - i + 1);
//        replace(cmd.begin(), cmd.end(), ',', ' ');
//        tokens.push_back(cmd);
//        i = k + 1;
//    }
//    return tokens;
//}

#include "stdafx.h"
#include "Transform.h"
#include <sstream>
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;


Transform::Transform() : matrix(nullptr), useMatrix(false) {
}

Transform::Transform(const Transform& other) : matrix(nullptr), useMatrix(other.useMatrix) {
    if (other.matrix) {
        matrix = other.matrix->Clone();
    }
}

Transform::~Transform() {
    if (matrix) {
        delete matrix;
        matrix = nullptr;
    }
}

void Transform::ensureMatrix() {
    if (!matrix) {
        matrix = new Gdiplus::Matrix();
        useMatrix = true;
    }
}

void Transform::Apply(Gdiplus::Graphics& g) const {
    if (useMatrix && matrix) {
        g.MultiplyTransform(matrix);
    }
}

void Transform::addTranslate(float tx, float ty) {
    ensureMatrix();
    // In SVG, transforms are applied right-to-left in the transform string
    // but when building the matrix, we multiply on the right (prepend)
    Gdiplus::Matrix temp;
    temp.Translate(tx, ty);
    matrix->Multiply(&temp, Gdiplus::MatrixOrderPrepend);
}

void Transform::addScale(float sx, float sy) {
    ensureMatrix();
    Gdiplus::Matrix temp;
    temp.Scale(sx, sy);
    matrix->Multiply(&temp, Gdiplus::MatrixOrderPrepend);
}

void Transform::addRotate(float angle) {
    ensureMatrix();
    Gdiplus::Matrix temp;
    temp.Rotate(angle);
    matrix->Multiply(&temp, Gdiplus::MatrixOrderPrepend);
}

void Transform::addRotate(float angle, float cx, float cy) {
    ensureMatrix();
    // Rotate around a point: translate to origin, rotate, translate back
    Gdiplus::Matrix temp;
    temp.Translate(cx, cy);
    temp.Rotate(angle);
    temp.Translate(-cx, -cy);
    matrix->Multiply(&temp, Gdiplus::MatrixOrderPrepend);
}

void Transform::addMatrix(float m11, float m12, float m21, float m22, float dx, float dy) {
    ensureMatrix();
    Gdiplus::Matrix temp(m11, m12, m21, m22, dx, dy);
    matrix->Multiply(&temp, Gdiplus::MatrixOrderPrepend);
}

void Transform::addSkewX(float angle) {
    ensureMatrix();
    float tanAngle = tan(angle * M_PI / 180.0f);
    Gdiplus::Matrix temp(1.0f, 0.0f, tanAngle, 1.0f, 0.0f, 0.0f);
    matrix->Multiply(&temp, Gdiplus::MatrixOrderPrepend);
}

void Transform::addSkewY(float angle) {
    ensureMatrix();
    float tanAngle = tan(angle * M_PI / 180.0f);
    Gdiplus::Matrix temp(1.0f, tanAngle, 0.0f, 1.0f, 0.0f, 0.0f);
    matrix->Multiply(&temp, Gdiplus::MatrixOrderPrepend);
}

void Transform::parseTransformFunction(const string& func, const vector<float>& params) {
    if (func == "translate") {
        if (params.size() == 1) {
            addTranslate(params[0], 0.0f);
        }
        else if (params.size() == 2) {
            addTranslate(params[0], params[1]);
        }
    }
    else if (func == "scale") {
        if (params.size() == 1) {
            addScale(params[0], params[0]);
        }
        else if (params.size() == 2) {
            addScale(params[0], params[1]);
        }
    }
    else if (func == "rotate") {
        if (params.size() == 1) {
            addRotate(params[0]);
        }
        else if (params.size() == 3) {
            addRotate(params[0], params[1], params[2]);
        }
    }
    else if (func == "skewX") {
        if (params.size() == 1) {
            addSkewX(params[0]);
        }
    }
    else if (func == "skewY") {
        if (params.size() == 1) {
            addSkewY(params[0]);
        }
    }
    else if (func == "matrix") {
        if (params.size() == 6) {
            addMatrix(params[0], params[1], params[2], params[3], params[4], params[5]);
        }
    }
}

Transform* Transform::parse(const std::string& transformStr) {
    if (transformStr.empty()) {
        return nullptr;
    }

    Transform* transform = new Transform();
    vector<string> tokens = splitTransforms(transformStr);

    for (const auto& token : tokens) {
        size_t parenPos = token.find('(');
        if (parenPos == string::npos) continue;

        size_t closeParenPos = token.find(')', parenPos);
        if (closeParenPos == string::npos) continue;

        string func = token.substr(0, parenPos);
        string paramsStr = token.substr(parenPos + 1, closeParenPos - parenPos - 1);

        // Trim whitespace
        func.erase(0, func.find_first_not_of(" \t\n\r"));
        func.erase(func.find_last_not_of(" \t\n\r") + 1);

        vector<float> params = parseTransformParams(paramsStr);
        transform->parseTransformFunction(func, params);
    }

    if (!transform->useMatrix) {
        delete transform;
        return nullptr;
    }

    return transform;
}

vector<float> parseTransformParams(const string& str) {
    vector<float> params;
    string cleaned = str;
    
    for (size_t pos = 0; pos < cleaned.length(); ++pos) {
        if (cleaned[pos] == ',') {
            cleaned[pos] = ' ';
        } 
        else if (cleaned[pos] == '-' && pos > 0 && cleaned[pos - 1] != ' ' && cleaned[pos - 1] != '(') {
			cleaned.insert(pos, " ");
            pos++;
		}
	}

	istringstream ss(cleaned);
    float value;
    while (ss >> value) {
		params.push_back(value);
    }

    return params;

}

vector<string> splitTransforms(const string& str) {
    vector<string> tokens;
    size_t i = 0;

    while (i < str.length()) {
        // Skip whitespace
        while (i < str.length() && isspace(str[i])) {
            i++;
        }
        if (i >= str.length()) break;

        // Find function name
        size_t funcStart = i;
        while (i < str.length() && str[i] != '(') {
            i++;
        }
        if (i >= str.length()) break;

        // Find matching closing parenthesis
        size_t parenStart = i;
        int depth = 0;
        while (i < str.length()) {
            if (str[i] == '(') depth++;
            else if (str[i] == ')') {
                depth--;
                if (depth == 0) {
                    i++;
                    break;
                }
            }
            i++;
        }

        tokens.push_back(str.substr(funcStart, i - funcStart));
    }

    return tokens;
}

Gdiplus::Matrix* Transform::getMatrix() const {
    return matrix;
}

Transform& Transform::operator=(const Transform& other) {
    if (this != &other) {
        if (matrix) {
            delete matrix;
            matrix = nullptr;
        }
        useMatrix = other.useMatrix;
        if (other.matrix) {
            matrix = other.matrix->Clone();
        }
    }
    return *this;
}