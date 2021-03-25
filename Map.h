#pragma once
#include "Game.h"
#include <string>

class Map {

public:
	Map(std::string tID, int mScale, int tSize);
	~Map();

	SDL_Rect BGsrc;
	SDL_Texture* BGtex;

	void loadMap(std::string path, int sizeX, int sizeY);
	void clearMap();
	void drawBackground();
	void addTile(int srcX, int srcY, int x, int y);


private:
	std::string texID;
	int mapScale;
	int tileSize;

};