#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(Graphics& grap) : graphics(grap) {}

void Renderer::render(const vector<shape*>& s) {
	for (auto shape : s) {
		if (shape) render(shape);
	}
}

void Renderer::render(shape* s) {
	if (s) {
		s->draw(graphics);
	}
}