#include "Game.h"
#include <iostream>
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>



Map* map;


SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager;

AssetManager* Game::assets = new AssetManager(&manager);

SDL_Rect Game::camera = {0, 0, 800, 640};
bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& label(manager.addEntity());

Game::Game() {}
Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	count = 0;
	int flag = 0;
	if (fullscreen == true) {
		flag = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Game Initialized!!!!!!!!!!!!!!!!!!!!" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flag);
		renderer = SDL_CreateRenderer(window, -1, 0);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		isRunning = true;
	}
	else {
		isRunning = false;
	}

	if (TTF_Init() == -1) {
		std::cout << "TFF init failed" << std::endl;
	}

	assets->addTexture("terrain", "assets/mainTileMap_terrain.png");
	assets->addTexture("player", "assets/slime_anims.png");
	assets->addTexture("projectile", "assets/projectile1.png");

	assets->addFont("arial", "assets/arial.ttf", 16);


	map = new Map("terrain", 1, 32);

	map->loadMap("assets/mainTileMap.txt", 150, 150);

	SDL_Colour white = { 255,255,255,255 };
	label.addComponent<UILabel>(10, 10, "TEST STRING", "arial", white);

	player.addComponent<TransformComponent>(1);
	player.addComponent<PhysicsComponent>(.15f);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<AbilityComponent>();
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player 1");

	player.addGroup(groupPlayers);

	//assets->createProjectile(Vector2D(300, 100),Vector2D(2,0), 400, 1, "projectile");


};

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents() {
	
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

void Game::update() {

	Vector2D oldPlayerPos = player.getComponent<TransformComponent>().position;
	bool freeFall = true;
	bool leftWall = false;
	bool rightWall = false;

	std::stringstream ss;
	ss << "Player Position: " << oldPlayerPos;
	label.getComponent<UILabel>().setLabelText(ss.str(), "arial");

	manager.refresh();
	manager.update();

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	playerCol.h = playerCol.h - 2;
	playerCol.w = playerCol.w - 2;
	for (auto c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (cCol.x + 32 >= camera.x && camera.x + camera.w >= cCol.x
			&& cCol.y + 32 >= camera.y && camera.y + camera.h >= cCol.y) {
			while (Collision::AABB(cCol, playerCol)) {
				// if it's a purely downward collision set isGrounded
				if (playerCol.y + playerCol.h >= cCol.y && oldPlayerPos.y + playerCol.h < cCol.y) {
					player.getComponent<PhysicsComponent>().isGrounded = true;
					player.getComponent<TransformComponent>().velocity.y = 0;
					player.getComponent<TransformComponent>().position.y = cCol.y - playerCol.h - 1;
					//std::cout << "Collision Down" << std::endl;
				}
				// if upward collision turn off jump acceleration and stop velocity
				else if (playerCol.y <= cCol.y + cCol.h && oldPlayerPos.y > cCol.y + cCol.h) {
					player.getComponent<TransformComponent>().velocity.y = 0;
					player.getComponent<TransformComponent>().position.y = cCol.y + cCol.h + 1;
					//std::cout << "Collision Up" << std::endl;
				}
				// leftward collision
				else if (playerCol.x <= cCol.x + cCol.w && oldPlayerPos.x > cCol.x + cCol.w) {
					player.getComponent<PhysicsComponent>().leftWalled = true;
					player.getComponent<TransformComponent>().position.x = cCol.x + cCol.w + 1;
					player.getComponent<TransformComponent>().velocity.x = 0;
					player.getComponent<AbilityComponent>().dashCounter == 8;
					//std::cout << "Collision Left" << std::endl;
				}
				//rightward collision
				else if (playerCol.x + playerCol.w >= cCol.x && oldPlayerPos.x + playerCol.w < cCol.x) {
					player.getComponent<PhysicsComponent>().rightWalled = true;
					player.getComponent<TransformComponent>().position.x = cCol.x - playerCol.w - 1;
					player.getComponent<TransformComponent>().velocity.x = 0;
					player.getComponent<AbilityComponent>().dashCounter == 8;
					//std::cout << "Collision Right" << std::endl;
				}
				playerCol.x = player.getComponent<TransformComponent>().position.x;
				playerCol.y = player.getComponent<TransformComponent>().position.y;
			}
			// if grounded, check for a collision underneath the player. if theres no collision turn on gravity
			if (player.getComponent<PhysicsComponent>().isGrounded == true) {
				playerCol.y += 5;
				if ((Collision::AABB(cCol, playerCol))) {
					freeFall = false;
				}
				playerCol.y -= 5;
			}
			// if walled, check for collision to the left/right 
			if (player.getComponent<PhysicsComponent>().leftWalled == true) {
				playerCol.x -= 5;
				if ((Collision::AABB(cCol, playerCol))) {
					leftWall = true;
					//std::cout << "LEFT wall" << std::endl;
				}
				playerCol.x += 5;
			}
			if (player.getComponent<PhysicsComponent>().rightWalled == true) {
				playerCol.x += 5;
				if ((Collision::AABB(cCol, playerCol))) {
					rightWall = true;
					//std::cout << "RIGHT wall" << std::endl;
				}
				playerCol.x -= 5;
			}
		}
	}
	if (freeFall) {
		//std::cout << "Free falling" << std::endl;
		player.getComponent<PhysicsComponent>().isGrounded = false;
	}
	if (!leftWall) {
		player.getComponent<PhysicsComponent>().leftWalled = false;
		//std::cout << "Not on LEFT wall" << std::endl;
	}
	if (!rightWall) {
		player.getComponent<PhysicsComponent>().rightWalled = false;
		//std::cout << "Not on RIGHT wall" << std::endl;
	}

	//for (auto p : projectiles) {
	//	SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;
	//	if (Collision::AABB(pCol, playerCol)) {
	//		p->destroy();
	//		std::cout << "Player Hit!!" << std::endl;
	//	}
	//}
	
	camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;
	//if (camera.x < 0) {
	//	camera.x = 0;
	//}
	//if (camera.y < 0) {
	//	camera.y = 0;
	//}
	//if (camera.x > 960 - camera.w) {
	//	camera.x = 960 - camera.w;
	//}
	//if (camera.y > 1600 - camera.h) {
	//	camera.y = 1600 - camera.h;
	//}
}



void Game::render() {
	SDL_RenderClear(renderer);
	for (auto& t : tiles) {
		int tileX = t->getComponent<TileComponent>().position.x;
		int tileY = t->getComponent<TileComponent>().position.y;
		if (tileX >= camera.x - 33 && tileX <= camera.x + camera.w + 1 && tileY >= camera.y - 33 && tileY <= camera.y + camera.w + 1) {
			t->draw();
		}
	}
	//for (auto& c : colliders) {
	//	c->draw();
	//}
	for (auto& p : projectiles) {
		p->draw();
	}
	for (auto& p : players) {
		p->draw();
	}
	label.draw();
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}

bool Game::running() {
	return isRunning;
}
