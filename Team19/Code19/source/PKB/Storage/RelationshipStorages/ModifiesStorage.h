#pragma once

#include "../Storage.h"
#include "./Util.h"

using namespace Rel;

class ModifiesStorage : public Storage {
public:
	ModifiesStorage();
	void insertRelationship(Rel::Modifies modifies);
	RelationshipResults getAllRelationship(Arg1Type arg1Type = Arg1Type::ANY);
	RelationshipResults getRelationshipByArg1(Arg arg1, Arg1Type arg1Type);
	RelationshipResults getRelationshipByArg2(Arg arg2, Arg1Type arg1Type);
	RelationshipResults getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, Arg1Type arg1Type);

private:
	intToStringPairMap modifiesArg1StatementMap;
	intToStringPairMap modifiesArg1ProcedureMap;

	intToStringPairMap modifiedByArg1StatementMap;
	intToStringPairMap modifiedByArg1ProcedureMap;


	stringPairSet globalModifies;
	stringPairSet globalModifiesArg1Statement;
	stringPairSet globalModifiesArg1Procedure;
};