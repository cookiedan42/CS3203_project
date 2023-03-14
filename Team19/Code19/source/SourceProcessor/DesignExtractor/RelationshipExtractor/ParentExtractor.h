#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "../../../Entity/Entity.h"
#include "../../../Relationship/Relationship.h"
#include "../../AST/AST.h"
#include "../DesignExtractorUtil.h"

using namespace SP;

class ParentExtractor {
private:
	const Ent::Statement dummyStatement = Ent::Statement(-1, "");
	vector<Rel::Parent> parentRelationships{};
public:
	ParentExtractor();
	
	vector<Rel::Parent> extractParent(const unique_ptr<ProgramNode> &AST);

private:
	void extractProc(const unique_ptr<ProcedureNode>& procedure);
	void extractStatementList(const Ent::Statement& parent, const vector<StatementNode>& statements);

private:
	void extractSublist(const unique_ptr<WhileStatementNode>& stmt);
	void extractSublist(const unique_ptr<IfStatementNode>& stmt);

	// template class for statement types with no sublist
	template<typename T>
	void extractSublist(const T& stmt);
};