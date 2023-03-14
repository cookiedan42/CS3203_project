#include "ProcedureEntityStorage.h"

ProcedureEntityStorage::ProcedureEntityStorage() {}

void ProcedureEntityStorage::insertProcedure(Ent::Procedure proc) {
	procRef procName = proc.getName();
	procedureSet.insert(procName);
}

EntityResults ProcedureEntityStorage::getAllProc() {
	return procedureSet;
}
