#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include "PhysicsComponent.h"
#include "AssetManager.h"
#include "AbilityComponent.h"

class KeyboardController : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	PhysicsComponent* physics;
	AbilityComponent* ability;
	// airborne = true implies an upward acceleration is being applied
	bool airborne;
	bool abilityOnCD;
	bool pressed;
	unsigned int jumpTime, maxJumpTime, abilityTimer, abilityCD, dashTime, maxDashTime;
	



	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		physics = &entity->getComponent<PhysicsComponent>();
		ability = &entity->getComponent<AbilityComponent>();
		airborne = false;
		abilityOnCD = false;
		maxJumpTime = 75;
		abilityCD = 300;
		pressed = false;
	}

	void update() override {

		while (SDL_PollEvent(&Game::event) != 0) {
			if (Game::event.type == SDL_KEYDOWN) {
				switch (Game::event.key.keysym.sym) {
				case SDLK_w:
					if (!airborne && physics->isGrounded == true) {
						physics->jump = true;
						physics->isGrounded = false;
						airborne = true;
						jumpTime = SDL_GetTicks();
					}
					else if (physics->isGrounded == false && physics->leftWalled == true) {
						physics->Ljump = true;
						transform->velocity.y = 0;
					}
					else if (physics->isGrounded == false && physics->rightWalled == true) {
						physics->Rjump = true;
						transform->velocity.y = 0;
					}
					sprite->play("Idle");
					break;
				case SDLK_s:
					//transform->velocity.y = 1;
					sprite->play("Walk");
					break;
				case SDLK_a:
					transform->velocity.x = -1.5;
					sprite->play("Walk");
					sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
					break;
				case SDLK_d:
					transform->velocity.x = 1.5;
					sprite->play("Walk");
					break;
				case SDLK_LEFT:
					if (!abilityOnCD) {
						ability->abilityActive = true;
						ability->direction = false;
						abilityTimer = SDL_GetTicks();
						abilityOnCD = true;
					}
					break;
				case SDLK_RIGHT:
					if (!abilityOnCD) {
						ability->abilityActive = true;
						ability->direction = true;
						abilityTimer = SDL_GetTicks();
						abilityOnCD = true;
					}
					break;
				case SDLK_UP:
					if (!pressed) {
						ability->abilityIndex = (ability->abilityIndex + 1) % 2;
						abilityCD = ability->getAbilityCD();
						std::cout << "Using Ability: " << ability->abilityIndex << std::endl;
						pressed = true;
						break;
					}
				case SDLK_DOWN:
					if (!pressed) {
						//positive modulus
						ability->abilityIndex = ((((ability->abilityIndex - 1) % 2) + 2) % 2);
						abilityCD = ability->getAbilityCD();
						std::cout << "Using Ability: " << ability->abilityIndex << std::endl;
						pressed = true;
						break;
					}
				default:
					break;
				}
			}
			if (SDL_GetTicks() - abilityTimer > abilityCD) {
				abilityOnCD = false;
			}
			if (airborne) {
				if (SDL_GetTicks() - jumpTime > maxJumpTime) {
					airborne = false;
					//turn off jump acceleration after an amount of time
					physics->jump = false;
				}
			}
			if (Game::event.type == SDL_KEYUP) {
				switch (Game::event.key.keysym.sym) {
				case SDLK_w:
					physics->jump = false;
					airborne = false;
					//transform->velocity.y = 0;
					sprite->play("Idle");
					break;
				case SDLK_s:
					//transform->velocity.y = 0;
					sprite->play("Idle");
					break;
				case SDLK_a:
					transform->velocity.x = 0;
					sprite->play("Idle");
					sprite->spriteFlip = SDL_FLIP_NONE;
					break;
				case SDLK_d:
					transform->velocity.x = 0;
					sprite->play("Idle");
					break;
				case SDLK_UP:
					pressed = false;
					break;
				case SDLK_DOWN:
					pressed = false;
					break;
				case SDLK_ESCAPE:
					Game::isRunning = false;
				default:
					break;
				}
			}
		}
	}

};