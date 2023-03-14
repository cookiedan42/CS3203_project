#pragma once
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include "../../Definitions/Definitions.h"
#include "../../Entity/Entity.h"
#include "../../TypeAlias/TypeAlias.h"
#include "../AST/AST.h"

using namespace std;

class CalledProcedure {
private:
	Ent::Procedure procedure;
	stmtNo_t stmtNo; // The stmtNo in which this procedure is called at
public:
	CalledProcedure(Ent::Procedure procedure, stmtNo_t stmtNo);

	Ent::Procedure getProcedure() const;
	stmtNo_t getStmtNo() const;

	bool operator==(const CalledProcedure& other) const;
};

using ProcedureCallMap = unordered_map<Ent::Procedure, unordered_set<CalledProcedure>>;
using StatementCallMap = unordered_map<Ent::Statement, unordered_set<CalledProcedure>>;
using ProcedureCallPairs = vector<pair<Ent::Procedure, CalledProcedure>>;
using StatementCallPairs = vector<pair<Ent::Statement, CalledProcedure>>;

namespace std {
    template<> struct hash<CalledProcedure> {
        size_t operator()(const CalledProcedure& calledProcedure) const {
            size_t procedureHash = std::hash<Ent::Procedure>()(calledProcedure.getProcedure());
            size_t stmtNoHash = std::hash<stmtNo_t>()(calledProcedure.getStmtNo()) << 1;
            return procedureHash ^ stmtNoHash;
        }
    };
}

class DesignExtractorUtil {
public:
	/*
		Helper to generate all indirect procedure calls
	*/
	static void generateAllProcedureCalls(ProcedureCallMap& procedureCallMap);

	/*
		Helper to generate all indirect container statement calls
	*/
	static void generateAllStatementCalls(ProcedureCallMap& procedureCallMap, StatementCallMap& statementCallMap);

	/*
		Helper to insert an element into the procedureCallMap
	*/
	static void insertIntoProcedureMap(const Ent::Procedure& key, const CalledProcedure& element, ProcedureCallMap& map);
	
	/*
		Helper to insert an element into the statementCallMap
	*/
	static void insertIntoStatementMap(const Ent::Statement& key, const CalledProcedure& element, StatementCallMap& map);

	static ProcedureCallPairs convertMapToPairs(ProcedureCallMap map);
	static StatementCallPairs convertMapToPairs(StatementCallMap map);

	static Ent::Statement extractStatementEntity(const unique_ptr<SP::ReadStatementNode>& stmt);
	static Ent::Statement extractStatementEntity(const unique_ptr<SP::PrintStatementNode>& stmt);
	static Ent::Statement extractStatementEntity(const unique_ptr<SP::AssignStatementNode>& stmt);
	static Ent::Statement extractStatementEntity(const unique_ptr<SP::WhileStatementNode>& stmt);
	static Ent::Statement extractStatementEntity(const unique_ptr<SP::IfStatementNode>& stmt);
	static Ent::Statement extractStatementEntity(const unique_ptr<SP::CallStatementNode>& stmt);

	/*
		Helper to extract variable entities in a conditional expression
	*/
	static void extractConditionVariables(vector<Ent::Variable>& variables, const unique_ptr<SP::ConditionalExpressionNode>& condition);

	/*
		Helper to extract variable entities in an expression node
	*/
	static void extractExpressionVariables(vector<Ent::Variable>& variables, const SP::ExpressionNode& expression);

	/*
		Helper to traverse expression node to obtain prefix expression
	*/
	static string getPreOrderTraversal(const SP::ExpressionNode& expression);
	
	/*
		Helper to traverse expression node to obtain infix expression
	*/
	static string getInOrderTraversal(const SP::ExpressionNode& expression);

private:
	static void extractRelationalFactorVariables(vector<Ent::Variable>& variables, const unique_ptr<SP::BinaryOperatorNode>& bnOp);
	static void extractRelationalFactorVariables(vector<Ent::Variable>& variables, const unique_ptr<SP::VariableNode>& variable);
	static void extractRelationalFactorVariables(vector<Ent::Variable>& variables, const unique_ptr<SP::ConstantNode>& constant);

	static string traverseExpression(const unique_ptr<SP::BinaryOperatorNode>& bnOp, bool isPreOrder = false);
	static string traverseExpression(const unique_ptr<SP::VariableNode>& variable, bool isPreOrder = false);
	static string traverseExpression(const unique_ptr<SP::ConstantNode>& constant, bool isPreOrder = false);
};