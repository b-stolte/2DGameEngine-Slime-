#include "Game.h"
#include <iostream>
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>
#include <string>



Map* map;


SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager;
int Game::HEIGHT;
int Game::WIDTH;
int Game::level = 1;
Vector2D Game::playerPos;

std::map<int, bool> Game::keyboard;
std::map<int, bool> Game::keyPressed;
std::map<int, bool> Game::keyReleased;

AssetManager* Game::assets = new AssetManager(&manager);

SDL_Rect Game::camera = {0, 0, 1024, 896};
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
	HEIGHT = height;
	WIDTH = width;


	assets->addTexture("terrain", "assets/levels/tileset.png");
	assets->addTexture("player", "assets/slime-Sheet.png");
	assets->addTexture("projectile", "assets/projectile1.png");
	assets->addTexture("enemy", "assets/blueslime.png");
	assets->addTexture("background", "assets/background.png");
	assets->addTexture("spawner", "assets/spawner.png");

	assets->addFont("arial", "assets/arial.ttf", 16);




	SDL_Colour black = { 0,0,0,255 };
	SDL_Colour red = { 255,0,0,255 };
	label.addComponent<UILabel>(10, 10, "TEST STRING", "arial", black);

	player.addComponent<TransformComponent>(1024,1024,32,32,1);
	player.addComponent<PhysicsComponent>(.25f, true);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<AbilityComponent>();
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player 1");
	player.addComponent<UILabel>(10, 40, "HP", "arial", red);
	player.addComponent<HealthComponent>(100);
	
	playerPos = player.getComponent<TransformComponent>().position;

	player.addGroup(groupPlayers);

	map = new Map("terrain", 1, 32);
	map->loadMap("assets/levels/level1.txt", 64, 64);

	//assets->spawnEnemy(Vector2D{ 600,2000 }, "enemy");
	
	//spawner1.addComponent<TransformComponent>(540, 2206, 128, 128, 1);
	//spawner1.addComponent<SpriteComponent>("spawner", false);
	//spawner1.addComponent<ColliderComponent>("Spawner 1");
	//spawner1.addComponent<SpawnerComponent>();
	//spawner1.addGroup(groupSpawners);

	//spawner2.addComponent<TransformComponent>(940, 2206, 128, 128, 1);
	//spawner2.addComponent<SpriteComponent>("spawner", false);
	//spawner2.addComponent<ColliderComponent>("Spawner 2");
	//spawner2.addComponent<SpawnerComponent>();
	//spawner2.addGroup(groupSpawners);


	//assets->createProjectile(Vector2D(300, 100),Vector2D(2,0), 400, 1, "projectile");


};

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& spawners(manager.getGroup(Game::groupSpawners));


void Game::handleEvents() {
	
	while(SDL_PollEvent(&event)) {

		switch (event.type) {

		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (keyboard[event.key.keysym.sym] == false) {
				keyPressed[event.key.keysym.sym] = true;
			}
			keyboard[event.key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			if (keyboard[event.key.keysym.sym] == true) {
				keyReleased[event.key.keysym.sym] = true;
			}
			keyboard[event.key.keysym.sym] = false;
			break;
		default:
			break;
		}
	}
}

void Game::update() {

	//std::cout << player.getComponent<TransformComponent>().velocity.x << std::endl;
	playerPos = player.getComponent<TransformComponent>().position;
	//Vector2D oldPlayerPos = player.getComponent<TransformComponent>().position;
	std::stringstream posSS;
	std::stringstream hpSS;
	posSS << "Position: " << playerPos;
	hpSS << "Health: " << player.getComponent<HealthComponent>().HP;
	label.getComponent<UILabel>().setLabelText(posSS.str(), "arial");
	player.getComponent<UILabel>().setLabelText(hpSS.str(), "arial");
	
	// IDEALLY DETECT COLLISION HERE 
	playerCollision();

	manager.refresh();

	// RESOLVE COLLISION HERE
	manager.update();
	//std::cout << player.getComponent<TransformComponent>().velocity.y << std::endl;

	playerCollision();
	enemyCollision();
	projectileCollision();
	spawnerCollision();


	
	camera.x = player.getComponent<TransformComponent>().position.x - HEIGHT/2;
	camera.y = player.getComponent<TransformComponent>().position.y - WIDTH/2;
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
	//TextureManager::draw(assets->getTexture("background"), SDL_Rect{ Game::camera.x/4,Game::camera.y/4,200,160}, SDL_Rect{ 0,0,800,640 }, SDL_FLIP_NONE);

	for (auto& t : tiles) {
		int tileX = t->getComponent<TileComponent>().position.x;
		int tileY = t->getComponent<TileComponent>().position.y;
		if (tileX >= camera.x - 33 && tileX <= camera.x + camera.w + 1 && tileY >= camera.y - 33 && tileY <= camera.y + camera.w + 1) {
			t->draw();
		}
	}

	
	for (auto& c : colliders) {
		c->draw();
	}
	for (auto& p : projectiles) {
		p->draw();
	}
	for (auto& p : players) {
		p->draw();
	}
	for (auto& e : enemies) {
		e->draw();
	}
	for (auto& s : spawners) {
		s->draw();
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

void Game::nextLevel() {
	level++;
	// create filepath string with new level
	// clear current map
	std::ostringstream oss;
	oss << "assets/levels/level" << level << ".txt";
	std::string path = oss.str();
	map->clearMap();
	map->loadMap(path, 64, 64);
}

void Game::playerCollision() {
	
	Vector2D nextPlayerPos = player.getComponent<TransformComponent>().position;
	nextPlayerPos.x  += player.getComponent<TransformComponent>().velocity.x;
	nextPlayerPos.y  += player.getComponent<TransformComponent>().velocity.y;


	int pHeight = player.getComponent<TransformComponent>().height;
	int pWidth = player.getComponent<TransformComponent>().width;

	float xOverlap = 0;
	float yOverlap = 0;
	for (auto c : colliders) {
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (cCol.x + 32 >= camera.x && camera.x + camera.w >= cCol.x
			&& cCol.y + 32 >= camera.y && camera.y + camera.h >= cCol.y) {
			if (Collision::AABB(nextPlayerPos,pHeight,pWidth, cCol )) {
				float eBottom = nextPlayerPos.y + pHeight -1;
				float cBottom = cCol.y + cCol.h -1;
				float eRight = nextPlayerPos.x + pWidth -1;
				float cRight = cCol.x + cCol.w -1;

				float bCollision = cBottom - nextPlayerPos.y;
				float tCollision = eBottom - cCol.y;
				float lCollision = eRight - cCol.x;
				float rCollision = cRight - nextPlayerPos.x;
				// if it's a purely downward collision set isGrounded
				if (tCollision < bCollision && tCollision < lCollision && tCollision < rCollision) {
					player.getComponent<PhysicsComponent>().isGrounded = true;
					yOverlap = nextPlayerPos.y + pHeight - cCol.y;
					player.getComponent<TransformComponent>().position.y = nextPlayerPos.y = cCol.y - pHeight - 1;
				}
				// if upward collision turn off jump acceleration and stop velocity
				else if (bCollision < tCollision && bCollision < lCollision && bCollision < rCollision) {
					player.getComponent<TransformComponent>().velocity.y = 0;
					player.getComponent<TransformComponent>().position.y = nextPlayerPos.y = cCol.y + cCol.h + 1;
					
				}
				// leftward collision
				else if (rCollision < bCollision && rCollision < tCollision && rCollision < lCollision) {
					xOverlap = cCol.x + cCol.w - nextPlayerPos.x;
					player.getComponent<PhysicsComponent>().leftWalled = true;
					player.getComponent<TransformComponent>().position.x = nextPlayerPos.x = cCol.x + cCol.w + 1;
					if (player.getComponent<AbilityComponent>().wallJumpTimer > 0) {
						player.getComponent<AbilityComponent>().wallJumpTimer = 0;
					}
					
				}
				//rightward collision
				else if (lCollision < bCollision && lCollision < tCollision && lCollision < rCollision) {
					xOverlap = nextPlayerPos.x + pWidth - cCol.x;
					player.getComponent<PhysicsComponent>().rightWalled = true;
					player.getComponent<TransformComponent>().position.x = nextPlayerPos.x = cCol.x - pWidth - 1;
					if (player.getComponent<AbilityComponent>().wallJumpTimer > 0) {
						player.getComponent<AbilityComponent>().wallJumpTimer = 0;
					}
					
				}
			}
		}
	}
	if (yOverlap > xOverlap) {
		player.getComponent<TransformComponent>().velocity.y = 0;
		//std::cout << "Stoping y velocity" << std::endl;
		//std::cout << xOverlap << " : " << yOverlap << std::endl;
	}
	else if (xOverlap > yOverlap) {
		player.getComponent<TransformComponent>().velocity.x = 0;
		//std::cout << xOverlap << " : " << yOverlap << std::endl;
	}

	//Check to see if there is ground under the player 
	if (player.getComponent<PhysicsComponent>().isGrounded) {
		bool downwardCollision = false;
		nextPlayerPos.y += 2;
		for (auto c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (cCol.x + 32 >= camera.x && camera.x + camera.w >= cCol.x && cCol.y + 32 >= camera.y && camera.y + camera.h >= cCol.y) {
				if (Collision::AABB(nextPlayerPos, pHeight, pWidth, cCol)) {
					downwardCollision = true;
				}
			}
		}
		if (!downwardCollision) {
			player.getComponent<PhysicsComponent>().isGrounded = false;
			std::cout << "Free fall" << std::endl;
		}
	}
	nextPlayerPos.y -= 2;

	//check to see if there is a wall to the right of the player
	if (player.getComponent<PhysicsComponent>().rightWalled) {
		bool wallCollision = false;
		nextPlayerPos.x += 2;
		for (auto c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (cCol.x + 32 >= camera.x && camera.x + camera.w >= cCol.x && cCol.y + 32 >= camera.y && camera.y + camera.h >= cCol.y) {
				if (Collision::AABB(nextPlayerPos, pHeight, pWidth, cCol)) {
					wallCollision = true;
				}
			}
		}
		if (!wallCollision) {
			player.getComponent<PhysicsComponent>().rightWalled = false;
			std::cout << "Away from right wall" << std::endl;
		}
	}
	nextPlayerPos.x -= 2;

	//check to see if there is a wall to the left of the player
	if (player.getComponent<PhysicsComponent>().leftWalled) {
		bool wallCollision = false;
		nextPlayerPos.x -= 2;
		for (auto c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (cCol.x + 32 >= camera.x && camera.x + camera.w >= cCol.x && cCol.y + 32 >= camera.y && camera.y + camera.h >= cCol.y) {
				if (Collision::AABB(nextPlayerPos, pHeight, pWidth, cCol)) {
					wallCollision = true;
				}
			}
		}
		if (!wallCollision) {
			player.getComponent<PhysicsComponent>().leftWalled = false;
			//std::cout << "Away from left wall" << std::endl;
		}
	}
}

void Game::enemyCollision() {
	for (auto e : enemies) {
		SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
		for (auto c : colliders) {
			SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			if (cCol.x + 32 >= eCol.x - 200 && eCol.x + eCol.w + 200 >= cCol.x && cCol.y + 32 >= eCol.y - 200 && eCol.y + eCol.h + 200 >= cCol.y) {
				if(Collision::AABB(cCol, eCol)) {
					int eBottom = eCol.y + eCol.h;
					int cBottom = cCol.y + cCol.h;
					int eRight = eCol.x + eCol.w;
					int cRight = cCol.x + cCol.w;

					int bCollision = cBottom - eCol.y;
					int tCollision = eBottom - cCol.y;
					int lCollision = eRight - cCol.x;
					int rCollision = cRight - eCol.x;

					// UPWARD
					if (bCollision < tCollision && bCollision < lCollision && bCollision < rCollision) {
						
						e->getComponent<TransformComponent>().velocity.y = 0;
						e->getComponent<TransformComponent>().position.y = cCol.y + eCol.h + 1;
						//std::cout << "Collision Down" << std::endl;
					}
					// DOWNWARD
					else if (tCollision < bCollision && tCollision < lCollision && tCollision < rCollision) {
						e->getComponent<PhysicsComponent>().isGrounded = true;
						e->getComponent<TransformComponent>().velocity.y = 0;
						e->getComponent<TransformComponent>().position.y = cCol.y - cCol.h - 1;
						//std::cout << "Collision Up" << std::endl;
					}
					// leftward collision
					else if (rCollision < bCollision && rCollision < tCollision && rCollision < lCollision) {
						e->getComponent<TransformComponent>().position.x = cCol.x + cCol.w + 1;
						e->getComponent<TransformComponent>().velocity.x = 0;
						//player.getComponent<AbilityComponent>().dashCounter = 8;
						//std::cout << "Collision Left" << std::endl;
					}
					//rightward collision
					else if (lCollision < bCollision && lCollision < tCollision && lCollision < rCollision) {
						e->getComponent<TransformComponent>().position.x = cCol.x - eCol.w - 1;
						e->getComponent<TransformComponent>().velocity.x = 0;
						//player.getComponent<AbilityComponent>().dashCounter = 8;
						//std::cout << "Collision Right" << std::endl;
					}
				}
			}
		}
	}
}

void Game::spawnerCollision() {
	for (auto s : spawners) {
		SDL_Rect sCol = s->getComponent<ColliderComponent>().collider;
		SDL_Rect pCol = player.getComponent<ColliderComponent>().collider;
		if (Collision::AABB(pCol, sCol)) {
			//set player spawn location
			//Deactivate spawner
			//change spawner sprite??->different animations
			if (s->getComponent<SpawnerComponent>().isActive == true){
				player.getComponent<HealthComponent>().spawnPoint = s->getComponent<TransformComponent>().position;
				s->getComponent<SpawnerComponent>().isActive = false;
				std::cout << "New spwn point at " << player.getComponent<HealthComponent>().spawnPoint << std::endl;
			}

		}
	}
}

void Game::projectileCollision() {
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	for (auto p : projectiles) {
		SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(pCol, playerCol)) {
			p->destroy();
			player.getComponent<HealthComponent>().damage = 20;
			player.getComponent<SpriteComponent>().play("Damaged");
			//damage animaton and noise
		}
		for (auto& e : enemies) {
			SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(pCol, eCol)) {
				std::cout << "Projectile: " << pCol.x << ", " << pCol.y << std::endl;
				std::cout << "Enemy: " << eCol.x << ", " << eCol.y << std::endl;
				p->destroy();
				e->getComponent<HealthComponent>().damage = 20;
				
			}
		}
		//for (auto& c : colliders) {
		//	SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		//	if (cCol.x + 32 >= pCol.x - 200 && pCol.x + pCol.w + 200 >= cCol.x && cCol.y + 32 >= pCol.y - 200 && pCol.y + pCol.h + 200 >= cCol.y) {
		//		if (Collision::AABB(pCol, cCol)) {
		//			std::cout << "Projectile: " << pCol.x << ", " << pCol.y << std::endl;
		//			std::cout << "Collider: " << cCol.x << ", " << cCol.y << std::endl;
		//			p->destroy();
		//		}
		//	}
		//}
	}
}