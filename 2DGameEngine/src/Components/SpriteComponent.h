#pragma once

#include <glm/glm.hpp>
#include <string>

struct SpriteComponent {
	std::string assetId;
	int width;
	int height;

	// we always need to add a default value for the components we create
	SpriteComponent(std::string assetId = "",int width = 0, int height = 0) {
		this->assetId = assetId;
		this->width = width;
		this->height = height;
	}
};