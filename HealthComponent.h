#pragma once
#include "Components.h"

class HealthComponent : public Component {

public:
	int HP;
	int maxHP;

	int damage = 0;

	Vector2D spawnPoint;

	HealthComponent(int maxHealth) {
		HP = maxHP = maxHealth;
		spawnPoint.x = 500;
		spawnPoint.y = 2000;
	}
	~HealthComponent() {};

	void checkDeath() {
		if (HP <= 0) {
			kill();
		}
	}

	void calcHP() {
		HP = HP - damage;
		damage = 0;
	}

	void update() {
		calcHP();
		checkDeath();
	}

	void kill() {
		//if entity = player respawn, else kill
		if (entity->hasGroup(Game::groupPlayers)) {
			respawn();
		}
		else {
			entity->destroy();
		}
		
	}

	void respawn() {
		HP = maxHP;
		entity->getComponent<TransformComponent>().position = spawnPoint;
	}
};