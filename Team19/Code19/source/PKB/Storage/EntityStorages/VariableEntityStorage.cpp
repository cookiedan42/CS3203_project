#include "VariableEntityStorage.h"

VariableEntityStorage::VariableEntityStorage() {}

void VariableEntityStorage::insertVariable(Ent::Variable var) {
	varRef varName = var.getName();
	variableSet.insert(varName);
}

EntityResults VariableEntityStorage::getAllVar() {
	return variableSet;
}
