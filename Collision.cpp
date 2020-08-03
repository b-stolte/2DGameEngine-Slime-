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