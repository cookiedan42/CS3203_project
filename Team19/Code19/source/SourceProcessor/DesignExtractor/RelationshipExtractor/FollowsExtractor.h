#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "../../../Entity/Entity.h"
#include "../../../Relationship/Relationship.h"
#include "../../AST/AST.h"
#include "../DesignExtractorUtil.h"

using lineNo = int;

using namespace SP;

class FollowsExtractor {
private:
	vector<Rel::Follows> followsRelationships{};
public:
	FollowsExtractor();

	vector<Rel::Follows> extractFollows(const unique_ptr<ProgramNode> &AST);

private:
	void extractProc(const unique_ptr<ProcedureNode>& procedure);
	void extractStatementList(const vector<StatementNode>& statements);

private:
	void extractSublist(const unique_ptr<WhileStatementNode>& stmt);
	void extractSublist(const unique_ptr<IfStatementNode>& stmt);

	// use template for other 4 which do not have sublists
	template<typename T>
	void extractSublist(const T& stmt);
};