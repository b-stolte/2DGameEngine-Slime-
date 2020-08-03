#pragma once
#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"

class AssetManager {
public:
	AssetManager(Manager* man);
	~AssetManager();

	void addTexture(std::string id, const char* path);
	SDL_Texture* getTexture(std::string id);


	void createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string tID);

	void addFont(std::string id, std::string path, int fontSize);
	TTF_Font* getFont(std::string id);
private:
	Manager* manager;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;



};