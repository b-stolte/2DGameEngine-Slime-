#pragma once
#include "SDL.h"

class ColliderComponent;

class Collision {
public:
	static bool AABB(const ColliderComponent& A, const ColliderComponent& B);
	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);
	
};