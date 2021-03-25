#include "Collision.h"
#include "ColliderComponent.h"


bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB) {
	if (recA.x + recA.w >= recB.x && recB.x + recB.w >= recA.x
		&& recA.y + recA.h >= recB.y && recB.y + recB.h >= recA.y) {
		return true;
	}
	else {
		return false;
	}
}

bool Collision::AABB(const ColliderComponent& A, const ColliderComponent& B) {
	if (AABB(A.collider, B.collider)) {
		std::cout << A.tag << " hit " << B.tag << std::endl;
		return(true);
	}
	else {
		return false;
	}
}

// Using floats for more presice collsion
bool Collision::AABB(Vector2D pos, int height, int width, const SDL_Rect& col) {
	if (pos.x + width >= col.x && col.x + col.w >= pos.x && pos.y + height >= col.y && col.y + col.h >= pos.y) {
		return true;
	}
	else {
		return false;
	}
}



//int Collision::getXDistance(const SDL_Rect& rectA, const SDL_Rect& rectB) {
//	int dx = 0;
//
//
//
//	if (rectA.x < rectB.x) {
//		dx = rectB.x - (rectA.x + rectA.w);
//	}
//	else if (rectB.x < rectA.x) {
//		dx = rectA.x - (rectB.x + rectB.w);
//	}
//	return(dx);
//
//
//}
//
//int Collision::getYDistance(const SDL_Rect& rectA, const SDL_Rect& rectB) {
//	int dy = 0;
//
//	if (rectA.y < rectB.y) {
//		dy = rectB.y - (rectA.y + rectA.h);
//	}
//	else if (rectB.y < rectA.y) {
//		dy = rectA.y - (rectB.y + rectB.h);
//	}
//
//	return(dy);


//}