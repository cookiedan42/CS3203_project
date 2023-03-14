#pragma once
#include <memory>
#include <vector>
#include "../../../Entity/Entity.h"
#include "../../../Pattern/Pattern.h"
#include "../../AST/AST.h"
#include "../DesignExtractorUtil.h"

using namespace SP;
using namespace std;

class WhilePatternExtractor {
private:
	vector<Ptn::WhilePtn> whilePatterns{};

	void extractStatementListPattern(const vector<StatementNode>& statements);

	void extractStatementPattern(const unique_ptr<WhileStatementNode>& whiles);

	void extractStatementPattern(const unique_ptr<IfStatementNode>& ifs);

	template<typename T>
	void extractStatementPattern(const T& stmt);

public:
	WhilePatternExtractor();

	vector<Ptn::WhilePtn> extractWhilePattern(const unique_ptr<ProgramNode>& AST);
};