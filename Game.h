#ifndef GAME_HPP
#define GAME_HPP

#include "SDL_image.h"
#include "SDL.h"
#include "iostream"
#include <vector>
#include "Vector2D.h"


class ColliderComponent;
class AssetManager;

class Game{
public:
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

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static AssetManager* assets;

	enum groupLabels : std::size_t {
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupProjectiles
	};
	

private:
	
	int count;
	SDL_Window *window;
};

#endif