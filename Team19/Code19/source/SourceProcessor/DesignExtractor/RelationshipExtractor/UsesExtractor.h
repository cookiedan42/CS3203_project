#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include "../../../Entity/Entity.h"
#include "../../../Relationship/Relationship.h"
#include "../../../TypeAlias/TypeAlias.h"
#include "../../AST/AST.h"
#include "../DesignExtractorUtil.h"

using namespace SP;

class UsesExtractor {
private:
	vector<Rel::Uses> usesRelationships{};
	vector<Rel::Uses> procedureUsesRelationships{};
	vector<Ent::Statement> statementStack; // To handle nested uses in while/if statements

	ProcedureCallMap procedureCallMap;
	StatementCallMap statementCallMap;

	void extractStatementListUses(const Ent::Procedure p, const vector<StatementNode> &statements);

	void extractStatementUses(const Ent::Procedure p, const unique_ptr<ReadStatementNode> &read);

	void extractStatementUses(const Ent::Procedure p, const unique_ptr<PrintStatementNode> &print);

	void extractStatementUses(const Ent::Procedure p, const unique_ptr<AssignStatementNode> &assign);

	void extractStatementUses(const Ent::Procedure p, const unique_ptr<WhileStatementNode> &whiles);

	void extractStatementUses(const Ent::Procedure p, const unique_ptr<IfStatementNode> &ifs);

	void extractStatementUses(const Ent::Procedure p, const unique_ptr<CallStatementNode> &call);

	void extractIndirectProcedureUses();

	void extractIndirectStatementUses();

	// Treat procedureUses as "nested"
	void addNestedUsesToVector(const Ent::Procedure p, const Ent::Variable var);
public:
	UsesExtractor();

	vector<Rel::Uses> extractUses(const unique_ptr<ProgramNode> &AST);
};