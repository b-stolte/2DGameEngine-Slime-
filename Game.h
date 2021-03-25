#ifndef GAME_HPP
#define GAME_HPP

#include "SDL_image.h"
#include "SDL.h"
#include "iostream"
#include <vector>
#include <map>
#include "Vector2D.h"


class ColliderComponent;
class AssetManager;

class Game{
public:
	static int HEIGHT;
	static int WIDTH;
	static int level;
	static Vector2D playerPos;

	static SDL_Rect camera;
	static bool isRunning;
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running();
	void playerCollision();
	void enemyCollision();
	void projectileCollision();
	void spawnerCollision();

	void nextLevel();

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static AssetManager* assets;
	

	static std::map<int, bool> keyboard;
	static std::map<int, bool> keyPressed;
	static std::map<int, bool> keyReleased;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupProjectiles,
		groupSpawners
	};
	

private:
	
	int count;
	SDL_Window *window;
};

#endif