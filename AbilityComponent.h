#pragma once
#include "ECS.h"
#include "TransformComponent.h"



class AbilityComponent : public Component {
public:

	
	bool hasShot;
	bool hasDash;
	bool hasWallJump;

	//shot timers later
	bool Lshot;
	bool Rshot;
	float shotCD;

	bool Lattack;
	bool Rattack;
	float attackCD;

	bool Ldash;
	bool Rdash;
	float dashTimer;
	float dashTime = 100;
	float dashCD;

	float maxJumpTime = 100;
	float jumpTimer;
	bool jump;

	float maxWallJumpTime = 200;
	float wallJumpTimer;
	bool Ljump;
	bool Rjump;

	bool disableMovement;


	AbilityComponent() {

	}

	~AbilityComponent(){}

	void update() override {
		shot();
		playerJump();
		WallJump();
		dash();
		attack();
	
	}

	void init() override {
		hasShot = true;
		hasDash = true;
		hasWallJump = true;
		Lshot = Rshot = false;
		jump = false;
		Ljump = false;
		Rjump = false;
		Ldash = Rdash = false;
		Lattack = Rattack = false;
		disableMovement = false;

		transform = &entity->getComponent<TransformComponent>();
	}

	void dash() {
		if (hasDash && Rdash) {
			if (SDL_GetTicks() - dashTimer < dashTime) {
				transform->velocity.x += 1.5f;
			}
			else {
				dashTimer = 0;
				Rdash = false;
				disableMovement = false;
			}
		}
		else if (hasDash && Ldash) {
			if (SDL_GetTicks() - dashTimer < dashTime) {
				transform->velocity.x -= 1.5f;
			}
			else {
				dashTimer = 0;
				Rdash = false;
				disableMovement = false;
			}
		}
	}

	void attack() {
		if (Lattack || Rattack) {
			std::cout << "Attacking" << std::endl;
			Lattack = Rattack = false;
		}
	}


	void shot() {
		if (hasShot && Lshot) {
			Game::assets->createProjectile(Vector2D(transform->position.x - 32, transform->position.y), Vector2D(-3, -.4), 600, 2, "projectile");
			Lshot = false;
		}
		else if (hasShot && Rshot){
			Game::assets->createProjectile(Vector2D(transform->position.x + 32, transform->position.y), Vector2D(3, -.4), 600, 2, "projectile");
			Rshot = false;
		}
	}

	void startJumpTimer() {
		jumpTimer = SDL_GetTicks();
	}

	void startWallJumpTimer() {
		wallJumpTimer = SDL_GetTicks();
		disableMovement = true;
	}

	void startDashTimer() {
		dashTimer = SDL_GetTicks();
		disableMovement = true;
	}

	void playerJump() {
		if (jump) {
			if (SDL_GetTicks() - jumpTimer < maxJumpTime) {
				transform->velocity.y -= 1.5f;
			}
			else {
				jumpTimer = 0;
				jump = false;
			}
		}
	}

	void WallJump() {
		if (Ljump && hasWallJump) {
			transform->velocity.y = 0;
			if (SDL_GetTicks() - wallJumpTimer < maxWallJumpTime) {
				if (transform->velocity.y > -6) {
					transform->velocity.y -= 1.0f;
				}
				if (transform->velocity.x < 4) {
					transform->velocity.x += 1.0f;
				}

			}
			else {
				wallJumpTimer = 0;
				Ljump = false;
				disableMovement = false;
			}

		}
		else if (Rjump && hasWallJump) {
			transform->velocity.y = 0;
			if (SDL_GetTicks() - wallJumpTimer < maxWallJumpTime) {
				if (transform->velocity.y > -6) {
					transform->velocity.y -= 1.0f;
				}
				if (transform->velocity.x > -4) {
					transform->velocity.x -= 1.0f;
				}
			}
			else {
				wallJumpTimer = 0;
				Rjump = false;
				disableMovement = false;
			}
		}
	}

private:
	TransformComponent* transform;
	unsigned int startTime;

};