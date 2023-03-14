#pragma once

#include "../Storage.h"

class ProcedureEntityStorage : public Storage {
public:
	ProcedureEntityStorage();
	void insertProcedure(Ent::Procedure);

	EntityResults getAllProc();


private:
	keySet procedureSet;
};