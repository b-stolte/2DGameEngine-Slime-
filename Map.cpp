#include "Map.h"
#include "Game.h"
#include "TextureManager.h"
#include <fstream>
#include <iostream>
#include "ECS.h"
#include "Components.h"

extern Manager manager;


Map::Map(std::string tID, int mScale, int tSize) {
	texID = tID;
	mapScale = mScale;
	tileSize = tSize;


}
Map::~Map() {

}


void Map::loadMap(std::string path, int sizeX, int sizeY) {
	char c;
	std::fstream mapFile;
	mapFile.open(path);
	int srcX;
	int srcY;
	//Parse tile map and add tiles
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			srcY = atoi(&c) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * 32;
			addTile(srcX, srcY, x * (tileSize * mapScale), y * (tileSize * mapScale));
			mapFile.ignore();
		}
		mapFile.ignore();
		
	}
	//Ignore newline
	mapFile.ignore();
	//Parse collision map and add colliders
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			if (c == '1') {
				std::cout << "Collider at: (" << x << "," << y << ")"  << std::endl;
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * (tileSize * mapScale), y * (tileSize * mapScale), tileSize * mapScale );
				tcol.addGroup(Game::groupColliders);
				
			}
			mapFile.ignore();
		}
		mapFile.ignore();
	}
	mapFile.close();
}
void Map::addTile(int srcX, int srcY, int x, int y) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, x, y, tileSize, mapScale, texID);
	tile.addGroup(Game::groupMap);
}

void Map::drawMap() {


}