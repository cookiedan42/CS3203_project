#include "ConstantEntityStorage.h"

ConstantEntityStorage::ConstantEntityStorage() {}


void ConstantEntityStorage::insertConstant(Ent::Constant con) {
	constRef val = con.getName();
	constantSet.insert(val);
}

EntityResults ConstantEntityStorage::getAllConstant() {
	return constantSet;
}