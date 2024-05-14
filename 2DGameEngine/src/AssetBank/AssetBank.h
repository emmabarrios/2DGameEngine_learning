#pragma once
#include <map>
#include <string>
#include "SDL_image.h"

class AssetBank {
private:
	std::map<std::string, SDL_Texture*> textures;
public:
	AssetBank();
	~AssetBank();

	void ClearAssets();
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filePath);
	SDL_Texture* GetTexture(const std::string& assetId);
};