#include "./UsesStorage.h"

UsesStorage::UsesStorage() {}

void UsesStorage::insertRelationship(Rel::Uses uses) {
	Arg1Type arg1Type = uses.getArg1Type();
	Arg var = uses.getArg2()->getName();
	Arg arg1;
	stringPair relationshipPair;

	if (arg1Type == Arg1Type::STATEMENT) {
		auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(uses.getArg1());

		arg1 = std::to_string(castedArg1->getStmtNo());
		relationshipPair = stringPair(arg1, var);

		Util::insertIntoTable(arg1, relationshipPair, usesArg1StatementMap);
		Util::insertIntoTable(var, relationshipPair, usedByArg1StatementMap);
		Util::globalInsert(relationshipPair, globalUsesArg1Statement);
	}
	else if (arg1Type == Arg1Type::PROCEDURE) {
		arg1 = uses.getArg1()->getName();
		relationshipPair = stringPair(arg1, var);

		Util::insertIntoTable(arg1, relationshipPair, usesArg1ProcedureMap);
		Util::insertIntoTable(var, relationshipPair, usedByArg1ProcedureMap);
		Util::globalInsert(relationshipPair, globalUsesArg1Procedure);
	}

	Util::globalInsert(relationshipPair, globalUses);
}

RelationshipResults UsesStorage::getAllRelationship(Arg1Type arg1Type) {
	if (arg1Type == Arg1Type::ANY) {
		return globalUses;
	}
	if (arg1Type == Arg1Type::STATEMENT) {
		return globalUsesArg1Statement;
	}
	if (arg1Type == Arg1Type::PROCEDURE) {
		return globalUsesArg1Procedure;
	}
	throw std::invalid_argument("arg1Type only allow: any | statement | procedure");
}

RelationshipResults UsesStorage::getRelationshipByArg1(Arg arg1, Arg1Type arg1Type) {
	if (arg1Type == Arg1Type::STATEMENT) {
		return usesArg1StatementMap[arg1];
	}
	if (arg1Type == Arg1Type::PROCEDURE) {
		return usesArg1ProcedureMap[arg1];
	}
	throw std::invalid_argument("arg1Type only allow: any | statement | procedure");
}

RelationshipResults UsesStorage::getRelationshipByArg2(Arg arg2, Arg1Type arg1Type) {
	if (arg1Type == Arg1Type::STATEMENT) {
		return usedByArg1StatementMap[arg2];
	}
	if (arg1Type == Arg1Type::PROCEDURE) {
		return  usedByArg1ProcedureMap[arg2];
	}
	throw std::invalid_argument("arg1Type only allow: any | statement | procedure");
}

RelationshipResults UsesStorage::getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, Arg1Type arg1Type) {
	if (arg1Type == Arg1Type::STATEMENT) {
		return Util::handleGetRelationshipByArg1AndArg2(usesArg1StatementMap, arg1, arg2);
	}
	if (arg1Type == Arg1Type::PROCEDURE) {
		return Util::handleGetRelationshipByArg1AndArg2(usesArg1ProcedureMap, arg1, arg2);
	}
	throw std::invalid_argument("arg1Type only allow: any | statement | procedure");
}

