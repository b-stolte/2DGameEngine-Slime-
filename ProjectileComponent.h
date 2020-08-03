#pragma once
#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"
#include <iostream>

class ProjectileComponent : public Component {

public:
	ProjectileComponent(int rng, int sp, Vector2D vel) {
		range = rng;
		speed = sp;
		velocity = vel;
	}
	~ProjectileComponent() {};

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override {
		distance += speed;

		if (distance > range) {
			std::cout << "Projectile reached range" << std::endl;
			entity->destroy();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w || transform->position.x < Game::camera.x 
			     || transform->position.y > Game::camera.y + Game::camera.h || transform->position.y < Game::camera.y) {
			entity->destroy();
		}
	}
private:
	Vector2D velocity;
	TransformComponent* transform;
	int range = 0;
	int speed = 0;
	int distance = 0;
};