#pragma once
#include "Components.h"
#include "SDL.h"
#include <math.h>

class AIComponent : public Component {

public:
	TransformComponent* transform;

	float attackRaduis = 320;
	float shotCD = 2000;
	float shotTimer;
	bool shotPause;
	

	bool direction;

	float playerx = Game::playerPos.x;
	float playery = Game::playerPos.y;
	float distanceToPlayer = 10000;


	AIComponent(){}
	~AIComponent(){}


	void init() {
		transform = &entity->getComponent<TransformComponent>();
		shotPause = false;
		shotTimer = SDL_GetTicks();
	}

	void update() {
		updatePlayerPos();
		move();
		startPauseTimer();
	}
	
	// behaviour: shoot player when in range, if you have recently shot move towards player instead
	void move() {
		if (inAttackRaduis() && !shotPause) {
			direction = playerx < transform->position.x ? false : true;
			if (SDL_GetTicks() - shotTimer > shotCD) {
				if (direction) {
					Game::assets->createProjectile(Vector2D{ transform->position.x + 32, transform->position.y }, Vector2D{ (playerx - transform->position.x)/distanceToPlayer, (playery - transform->position.y)/distanceToPlayer }*3, 500, 3, "projectile");
				}
				else {
					Game::assets->createProjectile(Vector2D{ transform->position.x - 32, transform->position.y }, Vector2D{ (playerx - transform->position.x) / distanceToPlayer, (playery - transform->position.y) / distanceToPlayer }*3, 500, 3, "projectile");
				}
				shotPause = true;
				shotTimer = SDL_GetTicks();
			}
			else{
				if (direction) {
					transform->velocity.x = .55;
				}
				else {
					transform->velocity.x = -.55;
				}
			}
		}
		else {
			transform->velocity.x = 0;
		}
	}

	bool inAttackRaduis() {

		if (distanceToPlayer < attackRaduis) {
			return true;
		}
		else {
			return false;
		}
	}


	void updatePlayerPos() {
		playerx = Game::playerPos.x;
		playery = Game::playerPos.y;
		distanceToPlayer = sqrt(pow(playerx - transform->position.x, 2) + pow(playery - transform->position.y, 2));
	}

	float pauseTimer = 0;
	float pauseTime = 1000;

	void startPauseTimer() {
		if (shotPause && pauseTimer == 0) {
			pauseTimer = SDL_GetTicks();
		}
		if (SDL_GetTicks() - pauseTimer > pauseTime) {
			shotPause = false;
			pauseTimer = 0;
		}
	}
};