#pragma once
#include "Components.h"
#include <math.h>
#include "Vector2D.h"


class TransformComponent : public Component {

private:


public:

	int height = 32;
	int width = 32;
	int scale = 1;
	//float gravity = 0.1f;
	Vector2D position;
	Vector2D velocity;
	float acceleration;

	int speed = 3;

	bool moveLeft = false;
	bool moveRight = false;
	bool direction;


	TransformComponent() {
		position.x = 0.0f;
		position.y = 0.0f;
	};
	TransformComponent(int sc){
		position.x = 400;
		position.y = 320;
		scale = sc;
	}
	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}
	TransformComponent(float x, float y, int h, int w, int sc) {
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}
	void init() override {
		acceleration = .6f;
		velocity.zero();
		
	
	}

	void update() override {
		playerMovement();
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;

	}

	void playerMovement() {
			if (moveLeft) {
				direction = false;
				velocity.x -= acceleration;
			}
			else if (moveRight) {
				direction = true;
				velocity.x += acceleration;
			}
	}

};