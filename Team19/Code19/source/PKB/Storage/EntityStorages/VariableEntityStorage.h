#pragma once

#include "../Storage.h"

class VariableEntityStorage : public Storage {
public:
	VariableEntityStorage();

	void insertVariable(Ent::Variable);

	EntityResults getAllVar();

private:
	keySet variableSet;
};