#include "./ModifiesStorage.h"

ModifiesStorage::ModifiesStorage() {}

void ModifiesStorage::insertRelationship(Rel::Modifies modifies) {
	Arg1Type arg1Type = modifies.getArg1Type();
	Arg var = modifies.getArg2()->getName();
	Arg arg1;
	stringPair relationshipPair;

	if (arg1Type == Arg1Type::STATEMENT) {
		auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(modifies.getArg1());

		arg1 = std::to_string(castedArg1->getStmtNo());
		relationshipPair = stringPair(arg1, var);

		Util::insertIntoTable(arg1, relationshipPair, modifiesArg1StatementMap);
		Util::insertIntoTable(var, relationshipPair, modifiedByArg1StatementMap);
		Util::globalInsert(relationshipPair, globalModifiesArg1Statement);
	}
	else if (arg1Type == Arg1Type::PROCEDURE) {
		arg1 = modifies.getArg1()->getName();
		relationshipPair = stringPair(arg1, var);

		Util::insertIntoTable(arg1, relationshipPair, modifiesArg1ProcedureMap);
		Util::insertIntoTable(var, relationshipPair, modifiedByArg1ProcedureMap);
		Util::globalInsert(relationshipPair, globalModifiesArg1Procedure);
	}

	Util::globalInsert(relationshipPair, globalModifies);
}

RelationshipResults ModifiesStorage::getAllRelationship(Arg1Type arg1Type) {
	if (arg1Type == Arg1Type::ANY) {
		return globalModifies;
	}
	if (arg1Type == Arg1Type::STATEMENT) {
		return globalModifiesArg1Statement;
	}
	if (arg1Type == Arg1Type::PROCEDURE) {
		return globalModifiesArg1Procedure;
	}
	throw std::invalid_argument("arg1Type only allow: any | statement | procedure");
}

RelationshipResults ModifiesStorage::getRelationshipByArg1(Arg arg1, Arg1Type arg1Type) {
	if (arg1Type == Arg1Type::STATEMENT) {
		return modifiesArg1StatementMap[arg1];
	}
	if (arg1Type == Arg1Type::PROCEDURE) {
		return modifiesArg1ProcedureMap[arg1];
	}
	throw std::invalid_argument("arg1Type only allow: any | statement | procedure");
}

RelationshipResults ModifiesStorage::getRelationshipByArg2(Arg arg2, Arg1Type arg1Type) {
	if (arg1Type == Arg1Type::STATEMENT) {
		return modifiedByArg1StatementMap[arg2];
	}
	if (arg1Type == Arg1Type::PROCEDURE) {
		return  modifiedByArg1ProcedureMap[arg2];
	}
	throw std::invalid_argument("arg1Type only allow: any | statement | procedure");
}

RelationshipResults ModifiesStorage::getRelationshipByArg1AndArg2(Arg arg1, Arg arg2, Arg1Type arg1Type) {
	if (arg1Type == Arg1Type::STATEMENT) {
		return Util::handleGetRelationshipByArg1AndArg2(modifiesArg1StatementMap, arg1, arg2);
	}
	if (arg1Type == Arg1Type::PROCEDURE) {
		return Util::handleGetRelationshipByArg1AndArg2(modifiesArg1ProcedureMap, arg1, arg2);
	}
	throw std::invalid_argument("arg1Type only allow: any | statement | procedure");
}

