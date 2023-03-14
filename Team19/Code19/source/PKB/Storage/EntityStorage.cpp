#include "EntityStorage.h"

EntityStorage::EntityStorage() {}

void EntityStorage::insertEntity(Ent::Procedure proc) {
	procedureStorage.insertProcedure(proc);
}

void EntityStorage::insertEntity(Ent::Constant cons) {
	constantStorage.insertConstant(cons);
}

void EntityStorage::insertEntity(Ent::Variable var) {
	variableStorage.insertVariable(var);
}

void EntityStorage::insertEntity(Ent::Statement stat) {
	statementStorage.insertStatement(stat);
}


ConstantEntityStorage EntityStorage::getConstantStorage() {
	return constantStorage;
}

VariableEntityStorage EntityStorage::getVariableStorage() {
	return variableStorage;
}

ProcedureEntityStorage EntityStorage::getProcedureStorage() {
	return procedureStorage;
}

StatementEntityStorage EntityStorage::getStatementStorage() {
	return statementStorage;
}

