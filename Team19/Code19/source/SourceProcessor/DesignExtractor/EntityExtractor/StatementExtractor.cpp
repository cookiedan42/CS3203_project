#include "StatementExtractor.h"

void StatementExtractor::extractStatementListEntity(const vector<StatementNode>& statements) {
	for (const auto& statement : statements) {
		auto entity = visit([](const auto& stmt) -> Ent::Statement {
			return DesignExtractorUtil::extractStatementEntity(stmt);
			}, statement);

		statementEntities.push_back(entity);

		visit([&](const auto& stmt) {
			extractChildStatementEntity(stmt);
			}, statement);
	}
}

void StatementExtractor::extractChildStatementEntity(const unique_ptr<WhileStatementNode>& whiles) {
	extractStatementListEntity(whiles->getStatements());
}

void StatementExtractor::extractChildStatementEntity(const unique_ptr<IfStatementNode>& ifs) {
	extractStatementListEntity(ifs->getThenStatements());
	extractStatementListEntity(ifs->getElseStatements());
}

template<typename T>
void StatementExtractor::extractChildStatementEntity(const T& stmt) {
	return;
}

StatementExtractor::StatementExtractor() {}

vector<Ent::Statement> StatementExtractor::extractStatements(const unique_ptr<ProgramNode>& AST) {
	for (const auto& procedure : AST->getProcedures()) {
		extractStatementListEntity(procedure->getStatements());
	}

	return statementEntities;
}