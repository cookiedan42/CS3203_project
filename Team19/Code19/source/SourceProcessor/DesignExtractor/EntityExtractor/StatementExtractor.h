#pragma once
#include <memory>
#include <vector>
#include "../../../Entity/Entity.h"
#include "../../AST/AST.h"
#include "../DesignExtractorUtil.h"

using namespace SP;
using namespace std;

class StatementExtractor {
private:
	vector<Ent::Statement> statementEntities{};

	void extractStatementListEntity(const vector<StatementNode>& statements);

	void extractChildStatementEntity(const unique_ptr<WhileStatementNode>& whiles);

	void extractChildStatementEntity(const unique_ptr<IfStatementNode>& ifs);

	template<typename T>
	void extractChildStatementEntity(const T& stmt);

public:
	StatementExtractor();

	vector<Ent::Statement> extractStatements(const unique_ptr<ProgramNode>& AST);
};