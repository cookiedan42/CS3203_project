#pragma once

#include "../Storage.h"

class ConstantEntityStorage : public Storage {
public:
	ConstantEntityStorage();

	void insertConstant(Ent::Constant);

	EntityResults getAllConstant();

private:
	keySet constantSet;
};