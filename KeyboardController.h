#pragma once
#include "Game.h"
#include "ECS.h"
#include "Components.h"
#include "PhysicsComponent.h"
#include "AssetManager.h"
#include "AbilityComponent.h"
#include "HealthComponent.h"

class KeyboardController : public Component {
public:
	TransformComponent* transform;
	SpriteComponent* sprite;
	PhysicsComponent* physics;
	AbilityComponent* ability;

	int activeAbility;


	



	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		physics = &entity->getComponent<PhysicsComponent>();
		ability = &entity->getComponent<AbilityComponent>();
		activeAbility = 0;
	}

	void update() override {
		//Walk Right
		if (Game::keyboard[SDLK_d] && !ability->disableMovement) {
			if (ability->wallJumpTimer == 0 && !(physics->rightWalled)) {
				transform->moveRight = true;
				sprite->play("Walk");
			}

			//Game::keyPressed[SDLK_d] = false;
		}
		else if (Game::keyReleased[SDLK_d]) {
			transform->moveRight = false;
			sprite->play("Idle");
			Game::keyReleased[SDLK_d] = false;
		}
		//Walk Left
		if (Game::keyboard[SDLK_a] && !ability->disableMovement) {
			if (ability->wallJumpTimer == 0 && !(physics->leftWalled)) {
				transform->moveLeft = true;
				sprite->play("Walk");
			}
			//Game::keyPressed[SDLK_a] = false;
		}
		else if (Game::keyReleased[SDLK_a]) {
			transform->moveLeft = false;
			sprite->play("Idle");
			Game::keyReleased[SDLK_a] = false;
		}
		//Jumping
		if (Game::keyPressed[SDLK_w]) {
			//Regular jump
			if (physics->isGrounded) {
				ability->jump = true;
				ability->startJumpTimer();
			}
			//Wall jump
			else if (physics->leftWalled) {
				ability->Ljump = true;
				ability->startWallJumpTimer();
			}
			else if (physics->rightWalled) {
				ability->Rjump = true;
				ability->startWallJumpTimer();
			}
			Game::keyPressed[SDLK_w] = false;
		}
		else if (Game::keyReleased[SDLK_w]) {
			ability->jump = false;
			Game::keyReleased[SDLK_w] = false;
		}
		//Using Ability (Shot, Dash, or Attack)
		if (Game::keyPressed[SDLK_LEFT]) {
			switch (activeAbility) {
			case 0:
				ability->Lshot = true;
				break;
			case 1:
				ability->Lattack = true;
				break;
			case 2:
				ability->Ldash = true;
				ability->startDashTimer();
				break;
			default:
				break;
			}
			sprite->play("Attack");
			transform->direction = false;
			Game::keyPressed[SDLK_LEFT] = false;
		}
		else if (Game::keyPressed[SDLK_RIGHT]) {
			switch (activeAbility) {
			case 0:
				ability->Rshot = true;
				break;
			case 1:
				ability->Rattack = true;
				break;
			case 2:
				ability->Rdash = true;
				ability->startDashTimer();
				break;
			default:
				break;
			}
			entity->getComponent<HealthComponent>().damage = 5;
			sprite->play("Attack");
			transform->direction = true;
			Game::keyPressed[SDLK_RIGHT] = false;
		}
		//Switching abilities
		if (Game::keyPressed[SDLK_UP]) {
			activeAbility = (activeAbility + 1) % 3;
			Game::keyPressed[SDLK_UP] = false;
			std::cout << activeAbility << std::endl;

		}
		else if (Game::keyPressed[SDLK_DOWN]) {
			activeAbility = ((((activeAbility - 1) % 3) + 3) % 3);
			Game::keyPressed[SDLK_DOWN] = false;
			std::cout << activeAbility << std::endl;
		}
		

		if (ability->disableMovement) {
			transform->moveLeft = false;
			transform->moveRight = false;
		}
	}
	
		

	

	// OLD CODE:
	//		if (Game::event.type == SDL_KEYDOWN) {
	//			switch (Game::event.key.keysym.sym) {
	//			case SDLK_w:
	//				if (!airborne && physics->isGrounded == true) {
	//					physics->jump = true;
	//					physics->isGrounded = false;
	//					airborne = true;
	//					jumpTime = SDL_GetTicks();
	//				}
	//				else if (physics->isGrounded == false && physics->leftWalled == true) {
	//					physics->Ljump = true;
	//					transform->velocity.y = 0;
	//				}
	//				else if (physics->isGrounded == false && physics->rightWalled == true) {
	//					physics->Rjump = true;
	//					transform->velocity.y = 0;
	//				}
	//				sprite->play("Idle");
	//				break;
	//			case SDLK_s:
	//				//transform->velocity.y = 1;
	//				sprite->play("Walk");
	//				break;
	//			case SDLK_a:
	//				transform->velocity.x = -1.5;
	//				sprite->play("Walk");
	//				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
	//				break;
	//			case SDLK_d:
	//				transform->velocity.x = 1.5;
	//				sprite->play("Walk");
	//				break;
	//			case SDLK_LEFT:
	//				if (!abilityOnCD) {
	//					ability->abilityActive = true;
	//					ability->direction = false;
	//					abilityTimer = SDL_GetTicks();
	//					abilityOnCD = true;
	//				}
	//				break;
	//			case SDLK_RIGHT:
	//				if (!abilityOnCD) {
	//					ability->abilityActive = true;
	//					ability->direction = true;
	//					abilityTimer = SDL_GetTicks();
	//					abilityOnCD = true;
	//				}
	//				break;
	//			case SDLK_UP:
	//				if (!pressed) {
	//					ability->abilityIndex = (ability->abilityIndex + 1) % 2;
	//					abilityCD = ability->getAbilityCD();
	//					std::cout << "Using Ability: " << ability->abilityIndex << std::endl;
	//					pressed = true;
	//					break;
	//				}
	//			case SDLK_DOWN:
	//				if (!pressed) {
	//					//positive modulus
	//					ability->abilityIndex = ((((ability->abilityIndex - 1) % 2) + 2) % 2);
	//					abilityCD = ability->getAbilityCD();
	//					std::cout << "Using Ability: " << ability->abilityIndex << std::endl;
	//					pressed = true;
	//					break;
	//				}
	//			default:
	//				break;
	//			}
	//		}
	//		if (SDL_GetTicks() - abilityTimer > abilityCD) {
	//			abilityOnCD = false;
	//		}
	//		if (airborne) {
	//			if (SDL_GetTicks() - jumpTime > maxJumpTime) {
	//				airborne = false;
	//				//turn off jump acceleration after an amount of time
	//				physics->jump = false;
	//			}
	//		}
	//		if (Game::event.type == SDL_KEYUP) {
	//			switch (Game::event.key.keysym.sym) {
	//			case SDLK_w:
	//				physics->jump = false;
	//				airborne = false;
	//				//transform->velocity.y = 0;
	//				sprite->play("Idle");
	//				break;
	//			case SDLK_s:
	//				//transform->velocity.y = 0;
	//				sprite->play("Idle");
	//				break;
	//			case SDLK_a:
	//				transform->velocity.x = 0;
	//				sprite->play("Idle");
	//				sprite->spriteFlip = SDL_FLIP_NONE;
	//				break;
	//			case SDLK_d:
	//				transform->velocity.x = 0;
	//				sprite->play("Idle");
	//				break;
	//			case SDLK_UP:
	//				pressed = false;
	//				break;
	//			case SDLK_DOWN:
	//				pressed = false;
	//				break;
	//			case SDLK_ESCAPE:
	//				Game::isRunning = false;
	//			default:
	//				break;
	//			}
	//		}
	//}

};