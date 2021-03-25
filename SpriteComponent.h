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

	int frameCount;
	int curFrame;
	int lastFrame;
	bool repeating;

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
			Animation jump = Animation(0, 8, 100, false);
			Animation damaged = Animation(1, 4, 100, false);
			Animation walk = Animation(2, 7, 100, true);
			Animation idle = Animation(3, 6, 200, true);
			Animation  attack = Animation(4, 6, 50, false);
			Animation death = Animation(5, 12, 100, false);

			animations.emplace("Idle", idle);
			animations.emplace("Walk", walk);
			animations.emplace("Jump", jump);
			animations.emplace("Damaged", damaged);
			animations.emplace("Attack", attack);
			animations.emplace("Death", death);


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
		lastFrame = -1;
		
		
	}
	void update() override {

		if (animated) {
			// Non repeating animations need to play for set number of frames (starting from frame 0) then return to the Idle animation
			if (!repeating) {
				
				curFrame = static_cast<int>((SDL_GetTicks() / speed) % frames);
				if (curFrame != lastFrame) {
					frameCount++;
				}
				lastFrame = curFrame;
				if (frameCount == frames) {
					play("Idle");
					srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
				}
				else {
					srcRect.x = srcRect.w * frameCount;
				}

			}
			else {
				srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
			}
		}

		
		srcRect.y = animIndex * transform->height;
		destRect.x = (int)transform->position.x - Game::camera.x;
		destRect.y = (int)transform->position.y -  Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
	}
	void draw() override {
		if (!transform->direction) {
			spriteFlip = SDL_FLIP_NONE;
		}
		else {
			spriteFlip = SDL_FLIP_HORIZONTAL;
		}
		TextureManager::draw(tex, srcRect, destRect, spriteFlip);
	}
	void play(const char* animName) {
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
		repeating = animations[animName].repeat;
		frameCount = -1;
	}
};