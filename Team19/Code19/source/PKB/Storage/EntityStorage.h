#pragma once

#include "../../Entity/Entity.h"
#include "EntityStorages/ConstantEntityStorage.h"
#include "EntityStorages/VariableEntityStorage.h"
#include "EntityStorages/ProcedureEntityStorage.h"
#include "EntityStorages/StatementEntityStorage.h"
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>


class EntityStorage {
public:
	EntityStorage();

	void insertEntity(Ent::Procedure);
	void insertEntity(Ent::Variable);
	void insertEntity(Ent::Constant);
	void insertEntity(Ent::Statement);

	ConstantEntityStorage getConstantStorage();
	VariableEntityStorage getVariableStorage();
	ProcedureEntityStorage getProcedureStorage();
	StatementEntityStorage getStatementStorage();


private:
	ProcedureEntityStorage procedureStorage;
	ConstantEntityStorage constantStorage;
	VariableEntityStorage variableStorage;
	StatementEntityStorage statementStorage;
	
};