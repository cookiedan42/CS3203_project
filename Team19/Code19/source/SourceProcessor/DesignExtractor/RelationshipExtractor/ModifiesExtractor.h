#pragma once
#include<stdio.h>
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

class ModifiesExtractor {
private:
	vector<Rel::Modifies> modifiesRelationships{};
	vector<Rel::Modifies> procedureModifiesRelationships{};
	vector<Ent::Statement> statementStack; // To handle nested modifies in while/if statements

	ProcedureCallMap procedureCallMap;
	StatementCallMap statementCallMap;

	void extractStatementListModifies(const Ent::Procedure p, const vector<StatementNode> &statements);

	void extractStatementModifies(const Ent::Procedure p, const unique_ptr<ReadStatementNode> &read);

	void extractStatementModifies(const Ent::Procedure p, const unique_ptr<PrintStatementNode> &print);

	void extractStatementModifies(const Ent::Procedure p, const unique_ptr<AssignStatementNode> &assign);

	void extractStatementModifies(const Ent::Procedure p, const unique_ptr<WhileStatementNode> &whiles);

	void extractStatementModifies(const Ent::Procedure p, const unique_ptr<IfStatementNode> &ifs);

	void extractStatementModifies(const Ent::Procedure p, const unique_ptr<CallStatementNode> &call);

	void extractIndirectProcedureModifies();

	void extractIndirectStatementModifies();

	// Treat procedureModifies as "nested"
	void addNestedModifiesToVector(const Ent::Procedure p, const Ent::Variable var);

public:
	ModifiesExtractor();

	vector<Rel::Modifies> extractModifies(const unique_ptr<ProgramNode> &AST);
};