#include "EntityManager.h"


EntityManager::EntityManager() {
	entityStorage = new EntityStorage;
}
void EntityManager::insert(std::shared_ptr<InsertEntityRequest> request) {
	return request->execute(*entityStorage);
}

EntityResults EntityManager::get(std::shared_ptr<GetEntityRequest> request) {
	return request->execute(*entityStorage);
}
