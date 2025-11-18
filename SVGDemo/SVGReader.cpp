#include "SVGReader.h"
#include "Renderer.h"
#include "SVGParser.h"
#include "Shape.h"

#include <fstream>
#include <iostream>

// Constructor
SVGReader::SVGReader(Renderer* renderer, SVGParser* parser)
    : m_renderer(renderer), m_parser(parser) {}

// Destructor
SVGReader::~SVGReader() {
    for (auto shape : m_shapes) {
        delete shape;
    }
    m_shapes.clear();
}

// Load file SVG: ghi nhận đường dẫn và kiểm tra file có khả dụng
bool SVGReader::Load(const std::string& file) {
    m_filePath = file;

    std::ifstream inputFile(m_filePath);
    if (!inputFile.is_open()) {
        std::cerr << "Không thể mở file SVG: " << m_filePath << std::endl;
        return false;
    }
    inputFile.close();
    return true;
}

// Parse file SVG 
void SVGReader::Parse() {
    if (!m_parser) {
        std::cerr << "SVGParser chưa được khởi tạo." << std::endl;
        return;
    }

    for (auto shape : m_shapes) {
        delete shape;
    }
    m_shapes.clear();
    m_shapes = m_parser->ParseFile(m_filePath);
}

// Render các shape sử dụng Renderer
void SVGReader::Render() {
    if (!m_renderer) {
        std::cerr << "Renderer chưa được khởi tạo." << std::endl;
        return;
    }

    for (auto shape : m_shapes) {
        m_renderer->RenderShape(shape);
    }
}

// Lấy danh sách các Shape đã parse
std::vector<Shape*> SVGReader::GetShapes() const {
    return m_shapes;
}
