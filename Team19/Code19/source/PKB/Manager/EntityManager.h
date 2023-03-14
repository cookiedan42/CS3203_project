#pragma once

#include "../Storage/EntityStorage.h"
#include "../../Request/GetRequests/GetEntityRequests/GetEntityRequest.h"
#include "../../Request/InsertRequests/InsertEntityRequests/InsertEntityRequest.h"

using namespace Ent;



class EntityManager {
public:
	EntityManager();

	EntityResults get(std::shared_ptr<GetEntityRequest>);
	void insert(std::shared_ptr<InsertEntityRequest>);

private:
	EntityStorage* entityStorage;
};