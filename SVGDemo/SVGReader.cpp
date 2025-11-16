#include "stdafx.h"
#include "SVGReader.h"

SVGReader::SVGReader(const string& filePath) {
	this->filePath = filePath;
	m_render = nullptr;
}

SVGReader::~SVGReader() {
	for (auto shape : m_shapes) {
		delete shape;
	}
}

void SVGReader::parse() {
	for (auto shape : m_shapes) {
		delete shape;
	}

	m_shapes.clear();

	m_shapes = m_parser.parseFile(filePath);
}

void SVGReader::render(Graphics& g) {
	Renderer renderer(g);
	renderer.render(m_shapes);
}