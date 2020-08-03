#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"

class TileComponent : public Component {
public:

	SDL_Rect srcRect, destRect;
	SDL_Texture* tex;
	Vector2D position;




	TileComponent() = default;
	~TileComponent() {
		SDL_DestroyTexture(tex);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tSize, int tScale, std::string id) {
		tex = Game::assets->getTexture(id);
		position.x = xpos;
		position.y = ypos;
		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tSize;
		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = destRect.h = tSize * tScale;
	}
		
	void draw() override {
		TextureManager::draw(tex, srcRect, destRect, SDL_FLIP_NONE);
	}

	void update() override {
		destRect.x = position.x - Game::camera.x;
		destRect.y = position.y - Game::camera.y;
	}

};