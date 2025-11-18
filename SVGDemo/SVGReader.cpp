#include "stdafx.h"
#include "SVGReader.h"
#include <stdexcept>

SVGReader::SVGReader(const string& filePath) {
	this->filePath = filePath;
	m_render = nullptr;
}

SVGReader::SVGReader(const string& filePath, Graphics& graphics) {
	this->filePath = filePath;
	m_render = new Renderer(graphics);
}

SVGReader::~SVGReader() {
	for (auto shape : m_shapes) {
		delete shape;
	}
	delete m_render;
}

void SVGReader::Parse() {
	for (auto shape : m_shapes) {
		delete shape;
	}

	m_shapes.clear();

	m_shapes = m_parser.ParseFile(filePath);
}

void SVGReader::Render() {
	m_render->Draw(m_shapes);
}

void SVGReader::setGraphics(Graphics& graphics) {
	m_render = new Renderer(graphics);
	if (!m_render) {
		throw runtime_error("Cant render");
	}
}

vector<shape*> SVGReader::GetShapes() {
	return m_shapes;
}