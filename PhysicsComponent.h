#pragma once
#include "TransformComponent.h"
#include "ECS.h"

class PhysicsComponent : public Component {

public:
	TransformComponent* transform;
	bool isGrounded;
	bool rightWalled;
	bool leftWalled;

	bool hasDrag;
	float gravity;
	float terminalVelocity;
	float drag;
	float friction;
	float wallSlide;

	


	PhysicsComponent(float g, bool d) {
		hasDrag = d;
		gravity = g;
		terminalVelocity = 4.8f;
		drag = 0.2f;
		wallSlide = 1.5f;
	}

	void applyAccelerations() {
		//wall friction
		if (!isGrounded && (leftWalled || rightWalled) && transform->velocity.y > 0) {
			transform->velocity.y = wallSlide;
		}
		//gravity
		else if (!isGrounded && transform->velocity.y < terminalVelocity) {
			transform->velocity.y += gravity;
		}
		if (hasDrag) {
			applyDrag();
		}
		
	}


	void applyDrag() {
		if (transform->velocity.x < 0.15f && transform->velocity.x > -0.15f) {
			transform->velocity.x = 0;
		}
		else if (!isGrounded) {
			if (transform->velocity.x > 0) {
				if (transform->velocity.x < 2.0f) {
					drag = .1f;
				}
				else if(transform->velocity.x < 2.4f){
					drag = transform->acceleration;
				}
				else {
					drag = 0.8f;
				}
				transform->velocity.x -= drag;
			}
			else if (transform->velocity.x < 0) {
				if (transform->velocity.x > -2.0f) {
					drag = .1f;
				}
				else if (transform->velocity.x > -2.4f) {
					drag = transform->acceleration;
				}
				else {
					drag = 0.8f;
				}
				transform->velocity.x += drag;
			}
		}
		else {
			if (transform->velocity.x > 0) {
				if (transform->velocity.x < 2.0f) {
					drag = .2f;
				}
				else if (transform->velocity.x < 2.4f) {
					friction = transform->acceleration;
				}
				else {
					drag = 0.8f;
				}
				transform->velocity.x -= drag;
			}
			else if (transform->velocity.x < 0) {
				if (transform->velocity.x > -2.0f) {
					drag = .2f;
				}
				else if (transform->velocity.x > -2.4f) {
					friction = transform->acceleration;
				}
				else {
					drag = 0.8f;
				}
				transform->velocity.x += drag;
			}
		}
	}





	void update() {
		applyAccelerations();
	}

	void init() {
		transform = &entity->getComponent<TransformComponent>();
		isGrounded = false;
		leftWalled = false;
		rightWalled = false;
		
	}

private:
 

};