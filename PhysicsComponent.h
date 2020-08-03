#pragma once
#include "TransformComponent.h"
#include "ECS.h"

class PhysicsComponent : public Component {

public:
	TransformComponent* transform;
	// SET on a downward collision
	bool isGrounded;
	bool rightWalled;
	bool leftWalled;
	float gravity;
	float terminalVelocity;
	bool jump;
	bool Ljump;
	bool Rjump;
	int wallJumpCount;


	PhysicsComponent(float g) {
		gravity = g;
		terminalVelocity = 4.8f;
	}

	void applyAccelerations() {
		if (!isGrounded && transform->velocity.y < terminalVelocity) {
			transform->velocity.y += gravity;
		}
		else if(isGrounded){
			transform->velocity.y = 0;
		}
		playerJump();
		leftWallJump();
		rightWallJump();
		setJumpCount();
	}

	void playerJump() {
		if (jump) {
			//std::cout << "vertical jumping" << std::endl;
			if (transform->velocity.y > -4) {
				transform->velocity.y -= 0.8f;
			}
		}
	}

	void leftWallJump() {
		if (Ljump) {
			if(wallJumpCount < 20) {
				transform->position.y -= 12;
				transform->position.x += 4;
				wallJumpCount++;
			}
			if (wallJumpCount == 20) {
				Ljump = false;
				wallJumpCount = 0;
				transform->velocity.y = -1;
				transform->velocity.x = .5;
			}
		}
	}
	void rightWallJump() {
		if (Rjump) {
			if(wallJumpCount < 20) {
				transform->position.y -= 12;
				transform->position.x -= 4;
				wallJumpCount++;
			}
			if (wallJumpCount == 20) {
				Rjump = false;
				wallJumpCount = 0;
				transform->velocity.y = -1;
				transform->velocity.x = -.5;
			}
			
		}
	}

	void setJumpCount() {
		if (!Rjump && !Ljump) {
			wallJumpCount = 0;
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
		jump = false;
		Ljump = false;
		Rjump = false;
		wallJumpCount = 0;
		
	}

private:
 

};