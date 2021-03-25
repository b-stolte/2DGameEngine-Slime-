#pragma once
#include "Components.h"
#include "SDL.h"
#include <string>


class spawnComponent : public Component {
public:

	std::string enemyType;
	TransformComponent* transform;
	Vector2D spawnLocation;
	int spawnDelay = 5000;
	int spawnTimer;

	bool playerSpawn = false;


	spawnComponent(std::string eID) {
		transform = &entity->getComponent<TransformComponent>();
		enemyType = eID;
		spawnTimer = SDL_GetTicks();
		spawnLocation = Vector2D{ transform->position.x + 48, transform->position.y + 96 };

	}

	~spawnComponent(){}

	void update() {
		if (SDL_GetTicks() - spawnTimer > spawnDelay  && ) {
			spawnEnemy();
			spawnTimer = SDL_GetTicks();
		}
	}


	//draw call needs to draw display sprite (dynamic)
	//void draw() {

	//}

	//spawn a new enemy depending on which display sprite we have
	void spawnEnemy() {
		Game::assets->spawnEnemy(spawnLocation, enemyType);
	}

	
private:


};