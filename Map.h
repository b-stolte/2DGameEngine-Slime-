#pragma once
#include "Game.h"
#include <string>

class Map {

public:
	Map(std::string tID, int mScale, int tSize);
	~Map();

	void loadMap(std::string path, int sizeX, int sizeY);
	void drawMap();
	void addTile(int srcX, int srcY, int x, int y);


private:
	std::string texID;
	int mapScale;
	int tileSize;

};