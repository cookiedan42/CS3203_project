#pragma once
#include <vector>
#include "./BaseNode.h"
#include "./ExpressionNode.h"
#include "./ConditionalExpressionNode.h"

// Nodes representing a statement

namespace SP {
	struct ReadStatementNode;
	struct PrintStatementNode;
	struct WhileStatementNode;
	struct IfStatementNode;
	struct AssignStatementNode;
	struct CallStatementNode;
	using StatementNode = std::variant<
		std::unique_ptr<ReadStatementNode>, std::unique_ptr<PrintStatementNode>,
		std::unique_ptr<WhileStatementNode>, std::unique_ptr<IfStatementNode>,
		std::unique_ptr<AssignStatementNode>, std::unique_ptr<CallStatementNode>
	>;

	struct StatementBase {
	protected:
		int statementNo;
	public:
		StatementBase(int statementNo) : statementNo(statementNo) {};
		const int getStatementNo() const { return statementNo; };
	};


	/*
		read: 'read' var_name';'
	*/
	struct ReadStatementNode : Node, StatementBase {
	private:
		std::unique_ptr<VariableNode> variable;
	public:
		ReadStatementNode(int statementNo, std::unique_ptr<VariableNode> variable);

		bool operator==(const Node& other) const override;
		const std::unique_ptr<VariableNode>& getVariable() const;
	};

	/*
		print: 'print' var_name';'
	*/
	struct PrintStatementNode : Node, StatementBase {
	private:
		std::unique_ptr<VariableNode> variable;
	public:
		PrintStatementNode(int statementNo, std::unique_ptr<VariableNode> variable);

		bool operator==(const Node& other) const override;
		const std::unique_ptr<VariableNode>& getVariable() const;

	};

	/*
		call: 'call' proc_name';'
	*/
	struct CallStatementNode : Node, StatementBase {
	private:
		std::string procName;
	public:
		CallStatementNode(int statementNo, std::string procName);

		bool operator==(const Node& other) const override;
		const std::string& getProcName() const;

	};

	/*
		while: 'while' '(' cond_expr ')' '{' stmtLst '}'
	*/
	struct WhileStatementNode : Node, StatementBase {
	private:
		std::unique_ptr<ConditionalExpressionNode> condition;
		std::vector<StatementNode> statements;
	public:
		WhileStatementNode(int statementNo,
			std::unique_ptr<ConditionalExpressionNode> condition, std::vector<StatementNode> statements);

		bool operator==(const Node& other) const override;
		const std::unique_ptr<ConditionalExpressionNode>& getCondition() const;
		const std::vector<StatementNode>& getStatements() const;

	};

	/*
		if: 'if' '(' cond_expr ')' 'then' '{' stmtLst '}' 'else' '{' stmtLst '}'
	*/
	struct IfStatementNode : Node, StatementBase {
	private:
		std::unique_ptr<ConditionalExpressionNode> condition;
		std::vector<StatementNode> thenStatements;
		std::vector<StatementNode> elseStatements;
	public:
		IfStatementNode(int statementNo, std::unique_ptr<ConditionalExpressionNode> condition,
			std::vector<StatementNode> thenStatements, std::vector<StatementNode> elseStatements);

		bool operator==(const Node& other) const override;
		const std::unique_ptr<ConditionalExpressionNode>& getCondition() const;
		const std::vector<StatementNode>& getThenStatements() const;
		const std::vector<StatementNode>& getElseStatements() const;

	};

	/*
		assign: var_name '=' expr ';'
	*/
	struct AssignStatementNode : Node, StatementBase {
	private:
		std::unique_ptr<VariableNode> LHS;
		ExpressionNode RHS;
	public:
		AssignStatementNode(int statementNo, std::unique_ptr<VariableNode> LHS, ExpressionNode RHS);

		bool operator==(const Node& other) const override;
		const std::unique_ptr<VariableNode>& getLHS() const;
		const ExpressionNode& getRHS() const;
	};
}