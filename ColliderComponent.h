#pragma once
#include <string>
#include "SDL.h"
#include "ECS.h"
#include "Components.h"


class ColliderComponent : public Component {
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcRect, destRect;
	TransformComponent* transform;
	
	ColliderComponent(std::string tg) {
		tag = tg;
	}
	ColliderComponent(std::string tg, int xPos, int yPos, int size) {
		tag = tg;
		collider.x = xPos;
		collider.y = yPos;
		collider.h = size;
		collider.w = size;
	}
	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
		tex = TextureManager::loadTexture("assets/collision_texture.png");
		srcRect = { 0,0,32,32 };
		destRect = { collider.x,collider.y,collider.w,collider.h };
	}

	void update() override {
		if (tag != "terrain") {
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * transform->scale;
			collider.h = transform->height * transform->scale;
		}
		destRect.x = collider.x - Game::camera.x;
		destRect.y = collider.y - Game::camera.y;

	}

	void draw() override {
		TextureManager::draw(tex, srcRect, destRect, SDL_FLIP_NONE);
	}

};