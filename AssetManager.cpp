#include "AssetManager.h"
#include "Components.h"

AssetManager::AssetManager(Manager* man) {
	manager = man;
}

AssetManager::~AssetManager() {

}

void AssetManager::addTexture(std::string id, const char* path) {
	textures.emplace(id, TextureManager::loadTexture(path));
}

SDL_Texture* AssetManager::getTexture(std::string id) {
	return textures[id];
}

void AssetManager::addFont(std::string id, std::string path, int fontSize) {

	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::getFont(std::string id) {
	return fonts[id];
}

void AssetManager::createProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string tID) {
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(tID, false);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addComponent<PhysicsComponent>(0.02f);
	projectile.addGroup(Game::groupProjectiles);
}