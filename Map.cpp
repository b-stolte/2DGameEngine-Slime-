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
	BGsrc = {0,0,800,640 };
	BGtex = Game::assets->getTexture("background");
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
	//Parse collision/asset map and add colliders/assets
	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(c);
			// COLLISION
			if (c == '3') {
				
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * (tileSize * mapScale), y * (tileSize * mapScale), tileSize * mapScale );
				tcol.addGroup(Game::groupColliders);
				std::cout << "Collider at " << x << ", " << y << std::endl;
				
			}
			// SPAWNPOINT
			else if (c == '4') {
				auto& players(manager.getGroup(Game::groupPlayers));
				for (auto p : players){
					p->getComponent<HealthComponent>().spawnPoint = Vector2D{(float)( x * (tileSize * mapScale)) ,(float)(y * (tileSize * mapScale)) };
					p->getComponent<HealthComponent>().respawn();
				}
			}
			else if (c == '5') {
				auto& spawner(manager.addEntity());
				spawner.addComponent<TransformComponent>(x * (tileSize * mapScale), y * (tileSize * mapScale) -64, 128, 128, 1);
				spawner.addComponent<SpriteComponent>("spawner", false);
				spawner.addComponent<ColliderComponent>("Spawner");
				spawner.addComponent<SpawnerComponent>();
				spawner.addGroup(Game::groupSpawners);
			}
			// else if spanwer tile, add spawner there
			// else if .................
			mapFile.ignore();
		}
		mapFile.ignore();
	}
	mapFile.close();
}

//destroys all tile objects
void Map::clearMap() {
	auto& tiles(manager.getGroup(Game::groupMap));
	auto& colliders(manager.getGroup(Game::groupColliders));
	for (auto t : tiles) {
		t->destroy();
	}
	for (auto c : colliders) {
		if (c->getComponent<ColliderComponent>().tag == "terrain") {
			c->destroy();
		}
	}
}

void Map::addTile(int srcX, int srcY, int x, int y) {
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, x, y, tileSize, mapScale, texID);
	tile.addGroup(Game::groupMap);
}

void Map::drawBackground() {
	//Draw background
	//BGsrc.x = Game::camera.x / 4;
	//BGsrc.y = Game::camera.y / 4;
	TextureManager::draw(BGtex, BGsrc, Game::camera, SDL_FLIP_NONE);

	
}