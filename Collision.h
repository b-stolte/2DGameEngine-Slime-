#pragma once
#include "SDL.h"


class ColliderComponent;
class Vector2D;

class Collision {
public:
	static bool AABB(const ColliderComponent& A, const ColliderComponent& B);
	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool AABB(Vector2D pos, int height, int width, const SDL_Rect& col);
	
	static int getXDistance(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static int getYDistance(const SDL_Rect& rectA, const SDL_Rect& rectB);
};