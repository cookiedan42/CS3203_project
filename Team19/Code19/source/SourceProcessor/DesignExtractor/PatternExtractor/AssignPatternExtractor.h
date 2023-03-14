#pragma once
#include <memory>
#include <vector>
#include "../../../Pattern/Pattern.h"
#include "../../AST/AST.h"
#include "../DesignExtractorUtil.h"

using namespace SP;
using namespace std;

class AssignPatternExtractor {
private:
	vector<Ptn::AssignPtn> assignPatterns{};

	void extractStatementListPattern(const vector<StatementNode>& statements);

	void extractStatementPattern(const unique_ptr<AssignStatementNode>& assign);

	void extractStatementPattern(const unique_ptr<WhileStatementNode>& whiles);

	void extractStatementPattern(const unique_ptr<IfStatementNode>& ifs);

	template<typename T>
	void extractStatementPattern(const T& stmt);

public:
	AssignPatternExtractor();

	vector<Ptn::AssignPtn> extractAssignPattern(const unique_ptr<ProgramNode>& AST);
};