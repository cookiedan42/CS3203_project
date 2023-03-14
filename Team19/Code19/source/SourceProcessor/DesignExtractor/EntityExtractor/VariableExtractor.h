#pragma once
#include <memory>
#include <vector>
#include "../../../Entity/Entity.h"
#include "../../AST/AST.h"
#include "../DesignExtractorUtil.h"

using namespace SP;
using namespace std;

class VariableExtractor {
private:
	vector<Ent::Variable> variableEntities{};

	void extractStatementListVariables(const vector<StatementNode>& statements);

	void extractVariableEntity(const unique_ptr<ReadStatementNode>& read);

	void extractVariableEntity(const unique_ptr<PrintStatementNode>& print);

	void extractVariableEntity(const unique_ptr<AssignStatementNode>& assign);

	void extractVariableEntity(const unique_ptr<WhileStatementNode>& whiles);

	void extractVariableEntity(const unique_ptr<IfStatementNode>& ifs);

	void extractVariableEntity(const unique_ptr<CallStatementNode>& call);

public:
	VariableExtractor();

	vector<Ent::Variable> extractVariables(const unique_ptr<ProgramNode>& AST);
};