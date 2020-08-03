#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Manager;
class Entity;


using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID(){
	static ComponentID lastID = 0u;
	return lastID++;
}

// no except????
template <typename T> inline ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t MaxComponents = 32;
constexpr std::size_t MaxGroups = 32;

using ComponentBitSet = std::bitset<MaxComponents>;
using GroupBitSet = std::bitset<MaxGroups>;

using ComponentArray = std::array<Component*, MaxComponents>;

class Component {
public:
	
	Entity* entity;

	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};

	virtual ~Component() {};
};

class Entity {
public:

	Entity(Manager& mManager) : manager(mManager){}
	void update() {for (auto& c : components) c->update();}
	void draw() {for (auto& c : components) c->draw();}
	bool isActive() {return active;}
	void destroy() { active = false; }

	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... Targs>
	T& addComponent(Targs&&...Margs) {

		T* c(new T(std::forward<Targs>(Margs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T> T& getComponent() const {

		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	bool hasGroup(Group mGroup) {
		return groupBitSet[mGroup];
	}
	void addGroup(Group mGroup);
	void delGroup(Group mGroup) {
		groupBitSet[mGroup] = false;
	}
private:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;
	GroupBitSet groupBitSet;
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

};

class Manager {

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, MaxGroups> groupedEntities;

public:
	void update() {
		for (auto& e : entities) e->update();
	}

	void draw() {
		for (auto& e : entities) e->draw();
	}

	void refresh() {
		for (auto i(0u); i < MaxGroups; i++) {
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v), [i](Entity* mEntity) {
					return !mEntity->isActive() || !mEntity->hasGroup(i);
					}),
				std::end(v));
		}


		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity>& mEntity) {
			return !mEntity->isActive();
			}), std::end(entities));

	}
	void addToGroup(Entity* mEntity, Group mGroup) {
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup) {
		return groupedEntities[mGroup];
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{e};
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

};