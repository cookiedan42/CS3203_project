#pragma once
#include <memory>
#include <vector>
#include "../../../Definitions/Definitions.h"
#include "../../../Entity/Entity.h"
#include "../../AST/AST.h"

using namespace SP;
using namespace std;

class ConstantExtractor {
private:
	vector<Ent::Constant> constantEntities{};

	void extractStatementListEntity(const vector<StatementNode>& statements);

	void extractConstantEntity(const unique_ptr<AssignStatementNode>& assign);

	void extractConstantEntity(const unique_ptr<WhileStatementNode>& whiles);

	void extractConstantEntity(const unique_ptr<IfStatementNode>& ifs);

	template<typename T>
	void extractConstantEntity(const T& stmt);

	void extractConditionEntity(const unique_ptr<ConditionalExpressionNode>& condition);

	void extractExpressionEntity(const unique_ptr<BinaryOperatorNode>& expression);

	void extractExpressionEntity(const unique_ptr<VariableNode>& variable);

	void extractExpressionEntity(const unique_ptr<ConstantNode>& constant);

public:
	ConstantExtractor();

	vector<Ent::Constant> extractConstants(const unique_ptr<ProgramNode>& AST);
};