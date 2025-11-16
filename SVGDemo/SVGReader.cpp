#include "SVGReader.h"

SVGReader::SVGReader(const string& filePath) {}

SVGReader::~SVGReader() {
	for (auto shape : m_shapes) {
		delete shape;
	}
}