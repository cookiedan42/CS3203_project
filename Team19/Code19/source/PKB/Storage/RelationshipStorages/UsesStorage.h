#pragma once

#include "../Storage.h"
#include "./Util.h"
#include "../../../Relationship/Relationship.h"

using namespace Rel;

class UsesStorage : public Storage {
public:
	UsesStorage();
	void insertRelationship(Rel::Uses uses);
	RelationshipResults getAllRelationship(Arg1Type arg1Type = Arg1Type::ANY);
	RelationshipResults getRelationshipByArg1(Arg arg1, Arg1Type arg1Type);
	RelationshipResults getRelationshipByArg2(Arg arg2, Arg1Type arg1Type);
	RelationshipResults getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, Arg1Type arg1Type);

private:
	intToStringPairMap usesArg1StatementMap;
	intToStringPairMap usesArg1ProcedureMap;

	intToStringPairMap usedByArg1StatementMap;
	intToStringPairMap usedByArg1ProcedureMap;


	stringPairSet globalUses;
	stringPairSet globalUsesArg1Statement;
	stringPairSet globalUsesArg1Procedure;
};