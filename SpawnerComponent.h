#pragma once
#include "Components.h"



class SpawnerComponent : public Component {

public:

	TransformComponent* transform;
	int count = 0;
	bool isActive;

	SpawnerComponent() {}
	~SpawnerComponent() {}

	void update() {
		count++;
		if (count > 300 && isActive) {
			spawnEnemy();
			count = 0;
		}
	}

	void init() {
		transform = &entity->getComponent<TransformComponent>();
		isActive = true;
	}

	void spawnEnemy() {
		Game::assets->spawnEnemy(Vector2D{ transform->position.x,transform->position.y }, "enemy");
	}
};