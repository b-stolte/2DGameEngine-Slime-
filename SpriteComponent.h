#pragma once
#include "SDL.h"
#include "Components.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include "AssetManager.h"

class SpriteComponent : public Component {

private:
	SDL_Texture* tex;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	TransformComponent* transform;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:
	int animIndex = 0;
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	std::map<const char*, Animation> animations;


	SpriteComponent() = default;
	SpriteComponent(std::string id) {
		setTex(id);
	}
	~SpriteComponent() {
	}
	SpriteComponent(std::string id, bool isAnimated) {
		if (isAnimated) {
			animated = true;
			Animation idle = Animation(0, 3, 100);
			Animation walk = Animation(1, 3, 100);
			animations.emplace("Idle", idle);
			animations.emplace("Walk", walk);


			play("Idle");
		}
		setTex(id);
	}
	void setTex(std::string id) {
		tex = Game::assets->getTexture(id);
	}
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		srcRect.w = transform->width;
		srcRect.h = transform->height;
		srcRect.x = 0;
		srcRect.y = 0;
		
		
	}
	void update() override {

		if (animated) {
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}
		
		srcRect.y = animIndex * transform->height;
		destRect.x = (int)transform->position.x - Game::camera.x;
		destRect.y = (int)transform->position.y -  Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}
	void draw() override {
		TextureManager::draw(tex, srcRect, destRect, spriteFlip);
	}
	void play(const char* animName) {
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};