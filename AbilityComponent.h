#pragma once
#include "ECS.h"
#include "TransformComponent.h"



class AbilityComponent : public Component {
public:

	// 0 for shot 1 for dash more later
	int abilityIndex;
	unsigned int abilityTime;

	bool abilityActive;
	bool direction;
	int dashCounter;

	AbilityComponent() {

	}

	~AbilityComponent(){}

	void update() override {
		useAbility();
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		abilityIndex = 0;
		abilityActive = false;
		abilityTime = 20;
		direction = false;
	}

	void useAbility() {
		if (abilityActive) {
			abilityActive = false;
			if (abilityIndex == 0) {
				shot();
			}
			if (abilityIndex == 1) {
				dashCounter = 0;
			}
			
		}
		dash();
	}

	
	unsigned int getAbilityCD() {
		switch (abilityIndex) {
			case 0:
				return 300;
				break;
			case 1:
				return 2000;
				break;
			default:
				break;
		}
	}

	void shot() {
		if (!direction) {
			Game::assets->createProjectile(Vector2D(transform->position.x - 32, transform->position.y), Vector2D(-3, -1), 600, 2, "projectile");
		}
		else {
			Game::assets->createProjectile(Vector2D(transform->position.x + 32, transform->position.y), Vector2D(3, -1), 600, 2, "projectile");
		}
	}

	// currently adds a velocity for a set amount of frames
	void dash() {
		int dirScale;
		if (direction) {
			dirScale = 1;
		}
		else {
			dirScale = -1;
		}
		if (dashCounter < 4) {
			std::cout << transform->velocity.x << std::endl;
			transform->velocity.x += 4*dirScale;
			dashCounter++;
		}
		if (dashCounter >= 4 && dashCounter < 8) {
			std::cout << transform->velocity.x << std::endl;
			transform->velocity.x -= 4 * dirScale;
			dashCounter++;
		}
		if (dashCounter == 8) {
			if (transform->velocity.y > 0) {
				transform->velocity.y = 0;
			}
			transform->velocity.x = 0;
			dashCounter++;
		}
	}



private:
	TransformComponent* transform;
	unsigned int startTime;


	

};