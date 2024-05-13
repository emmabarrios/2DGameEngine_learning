#pragma once

#include <glm/glm.hpp>

struct SpriteComponent {
	int width;
	int height;

	// we always need to add a default value for the components we create
	SpriteComponent(int width = 0, int height = 0) {
		this->width = width;
		this->height = height;
	}
};