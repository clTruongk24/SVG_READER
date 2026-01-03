#include "stdafx.h"
#include "Path.h"
#include "Color.h"
#include <sstream>
#include <fstream>
#include <algorithm> 
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <string>
#include <cctype>

class SvgPathTokenizer {
public:
    explicit SvgPathTokenizer(const std::string& d)
        : m_data(d), m_pos(0) {
    }

    bool eof() const { return m_pos >= m_data.size(); }

    // Skip whitespace and commas
    void skipSeparators() {
        while (!eof()) {
            char c = m_data[m_pos];
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == ',')
                ++m_pos;
            else
                break;
        }
    }

    bool nextIsCommand() {
        skipSeparators();
        return !eof() && std::isalpha(m_data[m_pos]);
    }

    char readCommand() {
        skipSeparators();
        return eof() ? '\0' : m_data[m_pos++];
    }

    // Read SVG number (float)
    bool readNumber(float& out) {
        skipSeparators();
        if (eof()) return false;

        size_t start = m_pos;
        bool hasDigit = false;

        // sign
        if (m_data[m_pos] == '+' || m_data[m_pos] == '-') {
            ++m_pos;
        }

        // integer part
        while (!eof() && std::isdigit(m_data[m_pos])) {
            hasDigit = true;
            ++m_pos;
        }

        // fraction
        if (!eof() && m_data[m_pos] == '.') {
            ++m_pos;
            while (!eof() && std::isdigit(m_data[m_pos])) {
                hasDigit = true;
                ++m_pos;
            }
        }

        // exponent
        if (!eof() && (m_data[m_pos] == 'e' || m_data[m_pos] == 'E')) {
            size_t expPos = m_pos;
            ++m_pos;
            if (!eof() && (m_data[m_pos] == '+' || m_data[m_pos] == '-'))
                ++m_pos;

            bool expDigit = false;
            while (!eof() && std::isdigit(m_data[m_pos])) {
                expDigit = true;
                ++m_pos;
            }

            if (!expDigit) {
                m_pos = expPos; // rollback invalid exponent
            }
        }

        if (!hasDigit) {
            m_pos = start;
            return false;
        }

        out = std::stof(m_data.substr(start, m_pos - start));
        return true;
    }

    // Read SVG arc flag (0 or 1)
    bool readFlag(bool& out) {
        skipSeparators();
        if (eof()) return false;

        char c = m_data[m_pos];
        if (c == '0' || c == '1') {
            out = (c == '1');
            ++m_pos;
            return true;
        }
        return false;
    }

private:
    std::string m_data;
    size_t m_pos;
};

path::path() {
    gPath = new Gdiplus::GraphicsPath();
    currentX = 0.0f;
    currentY = 0.0f;

	lastControlX = 0.0f;
	lastControlY = 0.0f;
	hadControlPoint = false;

	lastQuadControlX = lastQuadControlY = 0.0f;
	hadQuadControlPoint = false;

	subPathStartX = 0.0f;
	subPathStartY = 0.0f;
}

path::~path() {
    if (gPath) {
        delete gPath;
        gPath = nullptr;
    }
}

void path::setD(const string& d) {
    parseD(d);
}

void path::draw(Graphics& graphics) {
	GraphicsState state = graphics.Save();

    if (transform)
    {
        transform->Apply(graphics);
    }

	/*SolidBrush* brush = createFillBrush();
	graphics.FillPath(brush, gPath);*/

	RectF bounds = getBounds();
	Brush* brush = createFillGradientBrush(bounds);
	graphics.FillPath(brush, gPath);

    if (stroke_width != 0) {
        Pen* pen = createStrokeGradientBrush(bounds);
	/*	Pen* pen = createStrokePen();*/
        graphics.DrawPath(pen, gPath);
        delete pen;
	}

	delete brush;

	graphics.Restore(state);
}

// -------------------------------------------------------------------------
// Parses the 'd' attribute of the <path> element.
// Implements SVG Path Data specification:
// Reference: http://www.w3.org/TR/SVG/paths.html#PathData
// Supported commands: M (MoveTo), L (LineTo), H (Horizontal), V (Vertical), 
//                     C (Cubic Bezier), Z (ClosePath).
// -------------------------------------------------------------------------

//void path::parseD(const string& raw_d) {
//    string d = raw_d;
//    replace(d.begin(), d.end(), ',', ' ');
//
//    istringstream ss(d);
//    char cmd = '\0';
//    char lastCMD = '\0';
//
//	ofstream debugLog("path_debug.txt");
//	debugLog << "Parsed path data: " << d << endl;
//	int commandCount = 0;
//
//    gPath->Reset();
//
//    /*float lastControlX = 0.0f;
//    float lastControlY = 0.0f;
//    bool hadControlPoint = false;
//
//    while (ss >> ws) {
//        if (isalpha(ss.peek())) {
//            ss >> cmd;
//            lastCMD = cmd;
//        }
//        else {
//            cmd = lastCMD;
//        }
//
//        switch (cmd) {
//        case 'M': {
//            float x, y;
//            if (ss >> x >> y) {
//                gPath->StartFigure();
//                currentX = x;
//                currentY = y;
//                lastCMD = 'L';
//                hadControlPoint = false;
//            }
//            break;
//        }
//        case 'm': {
//            float x, y;
//            if (ss >> x >> y) {
//                gPath->StartFigure();
//                currentX += x;
//                currentY += y;
//                lastCMD = 'l';
//                hadControlPoint = false;
//            }
//            break;
//        }
//        case 'L': {
//            float x, y;
//            if (ss >> x >> y) {
//                gPath->AddLine(currentX, currentY, x, y);
//                currentX = x;
//                currentY = y;
//                hadControlPoint = false;
//            }
//            break;
//        }
//        case 'l': {
//            float x, y;
//            if (ss >> x >> y) {
//                gPath->AddLine(currentX, currentY, currentX + x, currentY + y);
//                currentX += x;
//                currentY += y;
//                hadControlPoint = false;
//            }
//            break;
//        }
//        case 'H': {
//            float x;
//            if (ss >> x) {
//                gPath->AddLine(currentX, currentY, x, currentY);
//                currentX = x;
//                hadControlPoint = false;
//            }
//            break;
//        }
//        case 'h': {
//            float x;
//            if (ss >> x) {
//                gPath->AddLine(currentX, currentY, currentX + x, currentY);
//                currentX += x;
//                hadControlPoint = false;
//            }
//            break;
//        }
//        case 'V': {
//            float y;
//            if (ss >> y) {
//                gPath->AddLine(currentX, currentY, currentX, y);
//                currentY = y;
//                hadControlPoint = false;
//            }
//            break;
//        }
//        case 'v': {
//            float y;
//            if (ss >> y) {
//                gPath->AddLine(currentX, currentY, currentX, currentY + y);
//                currentY += y;
//                hadControlPoint = false;
//            }
//            break;
//        }
//        case 'C': {
//            float x1, y1, x2, y2, x3, y3;
//            if (ss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3) {
//                gPath->AddBezier(currentX, currentY, x1, y1, x2, y2, x3, y3);
//                lastControlX = x2;
//                lastControlY = y2;
//                currentX = x3;
//                currentY = y3;
//                hadControlPoint = true;
//            }
//            break;
//        }
//        case 'c': {
//            float x1, y1, x2, y2, x3, y3;
//            if (ss >> x1 >> y1 >> x2 >> y2 >> x3 >> y3) {
//                gPath->AddBezier(currentX, currentY,
//                    currentX + x1, currentY + y1,
//                    currentX + x2, currentY + y2,
//                    currentX + x3, currentY + y3);
//                lastControlX = currentX + x2;
//                lastControlY = currentY + y2;
//                currentX += x3;
//                currentY += y3;
//                hadControlPoint = true;
//            }
//            break;
//        }
//        case 'S': {
//            float x2, y2, x, y;
//            if (ss >> x2 >> y2 >> x >> y) {
//                float x1, y1;
//                if (hadControlPoint) {
//                    x1 = 2 * currentX - lastControlX;
//                    y1 = 2 * currentY - lastControlY;
//                }
//                else {
//                    x1 = currentX;
//                    y1 = currentY;
//                }
//                gPath->AddBezier(currentX, currentY, x1, y1, x2, y2, x, y);
//                lastControlX = x2;
//                lastControlY = y2;
//                currentX = x;
//                currentY = y;
//                hadControlPoint = true;
//            }
//            break;
//        }
//        case 's': {
//            float x2, y2, x, y;
//            if (ss >> x2 >> y2 >> x >> y) {
//                float x1, y1;
//                if (hadControlPoint) {
//                    x1 = 2 * currentX - lastControlX;
//                    y1 = 2 * currentY - lastControlY;
//                }
//                else {
//                    x1 = currentX;
//                    y1 = currentY;
//                }
//                lastControlX = currentX + x2;
//                lastControlY = currentY + y2;
//                gPath->AddBezier(currentX, currentY, x1, y1,
//                    currentX + x2, currentY + y2,
//                    currentX + x, currentY + y);
//                currentX += x;
//                currentY += y;
//                hadControlPoint = true;
//            }
//            break;
//        }
//        case 'Z':
//        case 'z': {
//            gPath->CloseFigure();
//            hadControlPoint = false;
//            break;
//        }
//        default:
//            break;
//        }
//    }*/
//    while (ss >> ws) {
//        if (ss.peek() != EOF && isalpha(ss.peek())) {
//            ss >> cmd;
//            lastCMD = cmd;
//            commandCount++;
//			debugLog << "Processing command #" << commandCount << ": " << cmd << endl;
//        }
//        else if (lastCMD != '\0') {
//            cmd = lastCMD;
//			debugLog << "Repeating last command: " << cmd << endl;
//        }
//        else {
//            break;
//        }
//
//        switch (cmd) {
//        case 'M': {
//            handleMoveTo(ss, false);
//            lastCMD = 'L';
//            debugLog << "moveTo absolute" << endl;
//            break;
//        }
//        case 'm': {
//            handleMoveTo(ss, true);
//            lastCMD = 'l';
//			debugLog << "moveTo relative" << endl;
//            break;
//        }
//        case 'L': {
//            handleLineTo(ss, false);
//			debugLog << "lineTo absolute" << endl;
//            break;
//        }
//        case 'l': {
//            handleLineTo(ss, true);
//			debugLog << "lineTo relative" << endl;
//            break;
//        }
//        case 'H': {
//            handleHorizontalLineTo(ss, false);
//			debugLog << "horizontal lineTo absolute" << endl;
//            break;
//        }
//        case 'h': {
//            handleHorizontalLineTo(ss, true);
//			debugLog << "horizontal lineTo relative" << endl;
//            break;
//        }
//        case 'V': {
//            handleVerticalLineTo(ss, false);
//			debugLog << "vertical lineTo absolute" << endl;
//            break;
//        }
//        case 'v': {
//            handleVerticalLineTo(ss, true);
//			debugLog << "vertical lineTo relative" << endl;
//            break;
//        }
//        case 'C': {
//            handleCurveTo(ss, false);
//			debugLog << "curveTo absolute" << endl;
//            break;
//        }
//        case 'c': {
//            handleCurveTo(ss, true);
//			debugLog << "curveTo relative" << endl;
//            break;
//        }
//        case 'S': {
//            handleSmoothCurveTo(ss, false);
//			debugLog << "smooth curveTo absolute" << endl;
//            break;
//        }
//        case 's': {
//            handleSmoothCurveTo(ss, true);
//			debugLog << "smooth curveTo relative" << endl;
//            break;
//        }
//        case 'Q': {
//            handleQuadraticCurveTo(ss, false);
//			debugLog << "quadratic curveTo absolute" << endl;
//            break;
//        }
//        case 'q': {
//            handleQuadraticCurveTo(ss, true);
//			debugLog << "quadratic curveTo relative" << endl;
//            break;
//        }
//        case 'T': {
//            handleSmoothQuadraticCurveTo(ss, false);
//			debugLog << "smooth quadratic curveTo absolute" << endl;
//            break;
//        }
//        case 't': {
//            handleSmoothQuadraticCurveTo(ss, true);
//			debugLog << "smooth quadratic curveTo relative" << endl;
//            break;
//        }
//        case 'A': {
//            handleEllipticalArc(ss, false);
//			debugLog << "elliptical arc absolute" << endl;
//            break;
//        }
//        case 'a': {
//            handleEllipticalArc(ss, true);
//			debugLog << "elliptical arc relative" << endl;
//            break;
//        }
//        case 'Z': case 'z': {
//            handleClosePath();
//			debugLog << "close path" << endl;
//            break;
//        }
//        default:
//            break;
//        }
//    }
//	debugLog << "Finished parsing path data. Total commands: " << commandCount << endl;
//	debugLog.close();
//}

void path::parseD(const string& raw_d) {
    gPath->Reset();

    SvgPathTokenizer tk(raw_d);

    char cmd = 0;
    char lastCmd = 0;

    while (!tk.eof()) {
        if (tk.nextIsCommand()) {
            cmd = tk.readCommand();
            lastCmd = cmd;
        }
        else if (lastCmd != 0) {
            cmd = lastCmd;
        }
        else {
            break;
        }

        switch (cmd) {
        case 'M':
        case 'm': {
            bool rel = (cmd == 'm');
            float x, y;
            bool first = true;
            while (tk.readNumber(x) && tk.readNumber(y)) {
                if (first) {
                    if (rel) {
                        currentX += x;
                        currentY += y;
                    }
                    else {
                        currentX = x;
                        currentY = y;
                    }
                    gPath->StartFigure();
                    subPathStartX = currentX;
                    subPathStartY = currentY;
                    first = false;
                }
                else {
                    float tx = rel ? currentX + x : x;
                    float ty = rel ? currentY + y : y;
                    gPath->AddLine(currentX, currentY, tx, ty);
                    currentX = tx;
                    currentY = ty;
                }
                hadControlPoint = hadQuadControlPoint = false;
            }
            lastCmd = rel ? 'l' : 'L';
            break;
        }

        case 'L':
        case 'l': {
            bool rel = (cmd == 'l');
            float x, y;
            while (tk.readNumber(x) && tk.readNumber(y)) {
                float tx = rel ? currentX + x : x;
                float ty = rel ? currentY + y : y;
                gPath->AddLine(currentX, currentY, tx, ty);
                currentX = tx;
                currentY = ty;
                hadControlPoint = hadQuadControlPoint = false;
            }
            break;
        }

        case 'H':
        case 'h': {
            bool rel = (cmd == 'h');
            float x;
            while (tk.readNumber(x)) {
                float tx = rel ? currentX + x : x;
                gPath->AddLine(currentX, currentY, tx, currentY);
                currentX = tx;
                hadControlPoint = hadQuadControlPoint = false;
            }
            break;
        }

        case 'V':
        case 'v': {
            bool rel = (cmd == 'v');
            float y;
            while (tk.readNumber(y)) {
                float ty = rel ? currentY + y : y;
                gPath->AddLine(currentX, currentY, currentX, ty);
                currentY = ty;
                hadControlPoint = hadQuadControlPoint = false;
            }
            break;
        }

        case 'C':
        case 'c': {
            bool rel = (cmd == 'c');
            float x1, y1, x2, y2, x, y;
            while (tk.readNumber(x1) && tk.readNumber(y1) &&
                tk.readNumber(x2) && tk.readNumber(y2) &&
                tk.readNumber(x) && tk.readNumber(y)) {

                float cx1 = rel ? currentX + x1 : x1;
                float cy1 = rel ? currentY + y1 : y1;
                float cx2 = rel ? currentX + x2 : x2;
                float cy2 = rel ? currentY + y2 : y2;
                float tx = rel ? currentX + x : x;
                float ty = rel ? currentY + y : y;

                gPath->AddBezier(currentX, currentY, cx1, cy1, cx2, cy2, tx, ty);
                lastControlX = cx2;
                lastControlY = cy2;
                currentX = tx;
                currentY = ty;
                hadControlPoint = true;
                hadQuadControlPoint = false;
            }
            break;
        }

        case 'S':
        case 's': {
            bool rel = (cmd == 's');
            float x2, y2, x, y;
            while (tk.readNumber(x2) && tk.readNumber(y2) &&
                tk.readNumber(x) && tk.readNumber(y)) {

                float cx1 = hadControlPoint
                    ? 2 * currentX - lastControlX
                    : currentX;
                float cy1 = hadControlPoint
                    ? 2 * currentY - lastControlY
                    : currentY;

                float cx2 = rel ? currentX + x2 : x2;
                float cy2 = rel ? currentY + y2 : y2;
                float tx = rel ? currentX + x : x;
                float ty = rel ? currentY + y : y;

                gPath->AddBezier(currentX, currentY, cx1, cy1, cx2, cy2, tx, ty);
                lastControlX = cx2;
                lastControlY = cy2;
                currentX = tx;
                currentY = ty;
                hadControlPoint = true;
                hadQuadControlPoint = false;
            }
            break;
        }

        case 'Q':
        case 'q': {
            bool rel = (cmd == 'q');
            float x1, y1, x, y;
            while (tk.readNumber(x1) && tk.readNumber(y1) &&
                tk.readNumber(x) && tk.readNumber(y)) {

                float cx = rel ? currentX + x1 : x1;
                float cy = rel ? currentY + y1 : y1;
                float tx = rel ? currentX + x : x;
                float ty = rel ? currentY + y : y;

                addQuadraticBezier(cx, cy, tx, ty);
                lastQuadControlX = cx;
                lastQuadControlY = cy;
                currentX = tx;
                currentY = ty;
                hadQuadControlPoint = true;
                hadControlPoint = false;
            }
            break;
        }

        case 'T':
        case 't': {
            bool rel = (cmd == 't');
            float x, y;
            while (tk.readNumber(x) && tk.readNumber(y)) {
                float cx = hadQuadControlPoint
                    ? 2 * currentX - lastQuadControlX
                    : currentX;
                float cy = hadQuadControlPoint
                    ? 2 * currentY - lastQuadControlY
                    : currentY;

                float tx = rel ? currentX + x : x;
                float ty = rel ? currentY + y : y;

                addQuadraticBezier(cx, cy, tx, ty);
                lastQuadControlX = cx;
                lastQuadControlY = cy;
                currentX = tx;
                currentY = ty;
                hadQuadControlPoint = true;
                hadControlPoint = false;
            }
            break;
        }

        case 'A':
        case 'a': {
            bool rel = (cmd == 'a');
            float rx, ry, rot, x, y;
            bool largeArc, sweep;
            while (tk.readNumber(rx) &&
                tk.readNumber(ry) &&
                tk.readNumber(rot) &&
                tk.readFlag(largeArc) &&
                tk.readFlag(sweep) &&
                tk.readNumber(x) &&
                tk.readNumber(y)) {

                float tx = rel ? currentX + x : x;
                float ty = rel ? currentY + y : y;

                addArc(rx, ry, rot, largeArc, sweep, tx, ty);
                currentX = tx;
                currentY = ty;
                hadControlPoint = hadQuadControlPoint = false;
            }
            break;
        }

        case 'Z':
        case 'z':
            handleClosePath();
            break;

        default:
            return;
        }
    }
}


void path::handleMoveTo(istream& ss, bool isRelative) {
    float x, y;
	bool firstPoint = true;

    while (ss >> x >> y) {
        if (firstPoint) {
            if (isRelative) {
				currentX += x;
				currentY += y;
            }
            else {
				currentX = x;
				currentY = y;
            }
            gPath->StartFigure();
			subPathStartX = currentX;
			subPathStartY = currentY;
            firstPoint = false;
        }
        else {
			float targetX = isRelative ? currentX + x : x;
			float targetY = isRelative ? currentY + y : y;
			gPath->AddLine(currentX, currentY, targetX, targetY);
			currentX = targetX;
			currentY = targetY;
        }
		hadControlPoint = false;
		hadQuadControlPoint = false;

        if (ss >> ws, ss.peek() == EOF || isalpha(ss.peek())) {
            break;
		}
		
    } 
}

void path::handleLineTo(istream& ss, bool isRelative) {
    float x, y;
    while (ss >> x >> y) {
        float targetX = isRelative ? currentX + x : x;
        float targetY = isRelative ? currentY + y : y;
        gPath->AddLine(currentX, currentY, targetX, targetY);
        currentX = targetX;
        currentY = targetY;
        hadControlPoint = false;
		hadQuadControlPoint = false;

        if (ss >> ws, ss.peek() == EOF || isalpha(ss.peek())) {
            break;
		}
    }
}

void path::handleHorizontalLineTo(istream& ss, bool isRelative) {
    float x;
    while (ss >> x) {
        float targetX = isRelative ? currentX + x : x;
        gPath->AddLine(currentX, currentY, targetX, currentY);
        currentX = targetX;
        hadControlPoint = false;
		hadQuadControlPoint = false;

        if (ss >> ws, ss.peek() == EOF || isalpha(ss.peek())) {
            break;
        }
    }
}

void path::handleVerticalLineTo(istream& ss, bool isRelative) {
    float y;
    while (ss >> y) {
        float targetY = isRelative ? currentY + y : y;
        gPath->AddLine(currentX, currentY, currentX, targetY);
        currentY = targetY;
        hadControlPoint = false;
        hadQuadControlPoint = false;

        if (ss >> ws, ss.peek() == EOF || isalpha(ss.peek())) {
            break;
        }
    }
}

void path::handleCurveTo(istream& ss, bool isRelative) {
    float x1, y1, x2, y2, x, y;
    while (ss >> x1 >> y1 >> x2 >> y2 >> x >> y) {
        float ctrl1X = isRelative ? currentX + x1 : x1;
        float ctrl1Y = isRelative ? currentY + y1 : y1;
        float ctrl2X = isRelative ? currentX + x2 : x2;
        float ctrl2Y = isRelative ? currentY + y2 : y2;
        float targetX = isRelative ? currentX + x : x;
        float targetY = isRelative ? currentY + y : y;

        gPath->AddBezier(currentX, currentY, ctrl1X, ctrl1Y, ctrl2X, ctrl2Y, targetX, targetY);

        lastControlX = ctrl2X;
        lastControlY = ctrl2Y;
        currentX = targetX;
        currentY = targetY;
        hadControlPoint = true;
		hadQuadControlPoint = false;

        if (ss >> ws, ss.peek() == EOF || isalpha(ss.peek())) {
            break;
		}
    }
}

void path::handleSmoothCurveTo(istream& ss, bool isRelative) {
    float x2, y2, x, y;
    while (ss >> x2 >> y2 >> x >> y) {
        float ctrl1X, ctrl1Y;
        if (hadControlPoint) {
            ctrl1X = 2 * currentX - lastControlX;
            ctrl1Y = 2 * currentY - lastControlY;
        }
        else {
            ctrl1X = currentX;
            ctrl1Y = currentY;
        }
        float ctrl2X = isRelative ? currentX + x2 : x2;
        float ctrl2Y = isRelative ? currentY + y2 : y2;
        float targetX = isRelative ? currentX + x : x;
        float targetY = isRelative ? currentY + y : y;

        gPath->AddBezier(currentX, currentY, ctrl1X, ctrl1Y, ctrl2X, ctrl2Y, targetX, targetY);

        lastControlX = ctrl2X;
        lastControlY = ctrl2Y;
        currentX = targetX;
        currentY = targetY;
        hadControlPoint = true;
		hadQuadControlPoint = false;

        if (ss >> ws, ss.peek() == EOF || isalpha(ss.peek())) {
            break;
        }
    }
}

void path::handleQuadraticCurveTo(istream& ss, bool isRelative) {
    float x1, y1, x, y;
    while (ss >> x1 >> y1 >> x >> y) {
        float ctrlX = isRelative ? currentX + x1 : x1;
        float ctrlY = isRelative ? currentY + y1 : y1;
        float targetX = isRelative ? currentX + x : x;
        float targetY = isRelative ? currentY + y : y;

        addQuadraticBezier(ctrlX, ctrlY, targetX, targetY);

        lastQuadControlX = ctrlX;
        lastQuadControlY = ctrlY;
        currentX = targetX;
        currentY = targetY;
		hadQuadControlPoint = true;
		hadControlPoint = false;

        if (ss >> ws, ss.peek() == EOF || isalpha(ss.peek())) {
            break;
		}
    }
}

void path::handleSmoothQuadraticCurveTo(istream& ss, bool isRelative) {
    float x, y;
    if (ss >> x >> y) {
        float ctrlX, ctrlY;
        if (hadQuadControlPoint) {
            ctrlX = 2 * currentX - lastQuadControlX;
            ctrlY = 2 * currentY - lastQuadControlY;
        }
        else {
            ctrlX = currentX;
            ctrlY = currentY;
        }
        float targetX = isRelative ? currentX + x : x;
        float targetY = isRelative ? currentY + y : y;

        addQuadraticBezier(ctrlX, ctrlY, targetX, targetY);

        lastQuadControlX = ctrlX;
        lastQuadControlY = ctrlY;
        currentX = targetX;
        currentY = targetY;
		hadQuadControlPoint = true;
		hadControlPoint = false;

        if (ss >> ws, ss.peek() == EOF || isalpha(ss.peek())) {
            return;
        }
    }
}

void path::handleEllipticalArc(istream& ss, bool isRelative) {
    float rx, ry, xAxisRotation, x, y;
	int largeArcFlag, sweepFlag;
    while (ss >> rx >> ry >> xAxisRotation >> largeArcFlag >> sweepFlag >> x >> y) {
        float targetX = isRelative ? currentX + x : x;
        float targetY = isRelative ? currentY + y : y;

        addArc(rx, ry, xAxisRotation, largeArcFlag != 0.0f, sweepFlag != 0.0f, targetX, targetY);

        currentX = targetX;
        currentY = targetY;
        hadControlPoint = false;
		hadQuadControlPoint = false;

        if (ss >> ws, ss.peek() == EOF || isalpha(ss.peek())) {
            break;
		}
    }
}

void path::handleClosePath() {
    gPath->CloseFigure();
    hadControlPoint = false;
	hadQuadControlPoint = false;
	currentX = subPathStartX;
	currentY = subPathStartY;
}

void path::addQuadraticBezier(float x1, float y1, float x, float y ) {

    float cp1x = currentX + 2.0f / 3.0f * (x1 - currentX);
    float cp1y = currentY + 2.0f / 3.0f * (y1 - currentY);
    float cp2x = x + 2.0f / 3.0f * (x1 - x);
    float cp2y = y + 2.0f / 3.0f * (y1 - y);

    gPath->AddBezier(currentX, currentY, cp1x, cp1y, cp2x, cp2y, x, y);
}

void path::addArc(float rx, float ry, float xAxisRotation, bool largeArc,
    bool sweep, float x2, float y2) {
    
    float x1 = currentX;
	float y1 = currentY;

    if (x1 == x2 && y1 == y2) {
        return;
	}

    if (rx == 0.0f || ry == 0.0f) {
        gPath->AddLine(x1, y1, x2, y2);
        return;
	}

	rx = fabsf(rx);
	ry = fabsf(ry);

	float phi = xAxisRotation * (M_PI / 180.0f);
	float cosPhi = cosf(phi);
	float sinPhi = sinf(phi);

	float dx = (x1 - x2) / 2.0f;
	float dy = (y1 - y2) / 2.0f;
	float x1p = cosPhi * dx + sinPhi * dy;
	float y1p = -sinPhi * dx + cosPhi * dy;

    float lambda = (x1p * x1p) / (rx * rx) + (y1p * y1p) / (ry * ry);

    if (lambda > 1.0f) {
        rx *= sqrt(lambda);
        ry *= sqrt(lambda);
    }

    float sq = max(0.0f, (rx * rx * ry * ry - rx * rx * y1p * y1p - ry * ry * x1p * x1p) /
        (rx * rx * y1p * y1p + ry * ry * x1p * x1p));

    float coef = sqrt(sq);
    if (largeArc == sweep) {
        coef = -coef;
	}

	float cxp = coef * rx * y1p / ry;
	float cyp = -coef * ry * x1p / rx;

	float cx = cosPhi * cxp - sinPhi * cyp + (x1 + x2) / 2.0f;
	float cy = sinPhi * cxp + cosPhi * cyp + (y1 + y2) / 2.0f;

    auto vectorAngle = [](float ux, float uy, float vx, float vy) -> float {
        float dot = ux * vx + uy * vy;
        float len = sqrt(ux * ux + uy * uy) * sqrt(vx * vx + vy * vy);
        float ang = acos(max(-1.0f, min(1.0f, dot / len)));
        if (ux * vy - uy * vx < 0) {
            ang = -ang;
        }
        return ang;
        };

    float theta1 = vectorAngle(1, 0, (x1p - cxp) / rx, (y1p - cyp) / ry);
    float dtheta = vectorAngle((x1p - cxp) / rx, (y1p - cyp) / ry, (-x1p - cxp) / rx, (-y1p - cyp) / ry);

    if (sweep && dtheta < 0) {
        dtheta += 2 * M_PI;
    }
    else if (!sweep && dtheta > 0) {
        dtheta -= 2 * M_PI;
    }

    int segments = max(1, (int)ceil(abs(dtheta) / (M_PI / 2.0f)));
    float delta = dtheta / segments;
    float t = (4.0f / 3.0f) * tan(delta / 4.0f);

    float cosTheta1 = cos(theta1);
    float sinTheta1 = sin(theta1);

    for (int i = 0; i < segments; i++) {
        float theta2 = theta1 + delta;
        float cosTheta2 = cos(theta2);
        float sinTheta2 = sin(theta2);

        float q1x = cosTheta1;
        float q1y = sinTheta1;
        float q2x = cosTheta2;
        float q2y = sinTheta2;

        float cp1x = q1x - q1y * t;
        float cp1y = q1y + q1x * t;
        float cp2x = q2x + q2y * t;
        float cp2y = q2y - q2x * t;

        float p1x = rx * cp1x;
        float p1y = ry * cp1y;
        float p2x = rx * cp2x;
        float p2y = ry * cp2y;
        float p3x = rx * q2x;
        float p3y = ry * q2y;

        float c1x = cosPhi * p1x - sinPhi * p1y + cx;
        float c1y = sinPhi * p1x + cosPhi * p1y + cy;
        float c2x = cosPhi * p2x - sinPhi * p2y + cx;
        float c2y = sinPhi * p2x + cosPhi * p2y + cy;
        float endx = cosPhi * p3x - sinPhi * p3y + cx;
        float endy = sinPhi * p3x + cosPhi * p3y + cy;

        gPath->AddBezier(currentX, currentY, c1x, c1y, c2x, c2y, endx, endy);

        currentX = endx;
        currentY = endy;
        theta1 = theta2;
        cosTheta1 = cosTheta2;
        sinTheta1 = sinTheta2;
    }
}

RectF path::getBounds() const {
    if (!gPath) 
        {
        return RectF();
	}

	RectF bounds;
	gPath->GetBounds(&bounds);

    if (bounds.Width == 0 || bounds.Height == 0)
		return RectF();

    return bounds;
}