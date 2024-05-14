#include "AssetBank.h"
#include "../Logger/Logger.h"
#include "SDL_image.h"

AssetBank::AssetBank() {
	Logger::Log("AssetBank constructor called.");
}

AssetBank::~AssetBank() {
	ClearAssets();

	Logger::Err("AssetBank destructor called");
}

// why the for loop is not auto texture& ?
void AssetBank::ClearAssets() {
	for (auto texture : textures) {
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetBank::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filepath) {
	SDL_Surface* surface = IMG_Load(filepath.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,surface);
	SDL_FreeSurface(surface);

	// Add the texture to the map
	textures.emplace(assetId, texture);

	Logger::Log("New texture added to the asset bank with id: " + assetId);
}

// We previously marked this method as const but since we are trying to get a map value using brackets we had to remove it, C++ stuff
SDL_Texture* AssetBank::GetTexture(const std::string& assetId) {
	return textures[assetId];
}