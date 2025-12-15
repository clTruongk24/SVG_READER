#include "stdafx.h"
#include "Transform.h"
#include <sstream>
#include <algorithm>

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

Transform* Transform::parse(const std::string& transformStr) {
    if (transformStr.empty()) {
        return nullptr;
	}

    Transform* transform = new Transform();

	vector<string> tokens = split(transformStr);

    for (auto& token : tokens) {
		int start = token.find('(');
		int end = token.find(')');
        if (start == string::npos || end == string::npos || end <= start) continue;
		string cmd = token.substr(0, start);
		string paramsStr = token.substr(start + 1, end - start - 1);

		vector<float> params = parseParams(paramsStr);

		transform->switchTransform(cmd, params);
    } 

    return transform;
}

void Transform::switchTransform(const string& cmd, const vector<float>& params) {
    if (cmd == "translate" && (params.size() == 1 || params.size() == 2)) {
        float tx = params[0];
        float ty = (params.size() == 2) ? params[1] : 0.0f;
        setTranslateX(tx);
        setTranslateY(ty);
    }
    else if (cmd == "scale" && (params.size() == 1 || params.size() == 2)) {
        float sx = params[0];
        float sy = (params.size() == 2) ? params[1] : sx;
        setScaleX(sx);
        setScaleY(sy);
    }
    else if (cmd == "rotate" && (params.size() == 1 || params.size() == 3)) {
        float angle = params[0];
        if (params.size() == 3) {
            float cx = params[1];
            float cy = params[2];
            setRotation(angle, cx, cy);
        }
        else {
            setRotation(angle);
        }
    }
    else if (cmd == "matrix" && params.size() == 6) {
        setMatrix(params[0], params[1], params[2], params[3], params[4], params[5]);
    }
}

vector<float> parseParams(const string& paramsStr) {
    vector<float> params;
    istringstream ss(paramsStr);
    string param;
    while (ss >> param) {
        params.push_back(stof(param));
    }
    return params;
}

vector<string> split(const string& str) {
    vector<string> tokens;

    int i = 0;
    while (i < str.length()) {
        
		i = str.find_first_not_of(" \t\n", i);
        if (i == string::npos) break;
        
		int j = str.find_first_of(" (", i);
		if (j == string::npos) break;

		int k = str.find_first_of(")", j);
		if (k == string::npos) break;

		string cmd = str.substr(i, k - i + 1);
		replace(cmd.begin(), cmd.end(), ',', ' ');
		tokens.push_back(cmd);
		i = k + 1;
    }
    return tokens;
}