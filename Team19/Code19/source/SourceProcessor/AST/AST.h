#pragma once
#include <vector>
#include <string>
#include <memory>

#include "BaseNode.h"
#include "ExpressionNode.h"
#include "ConditionalExpressionNode.h"
#include "StatementNode.h"

namespace SP {
	/*
		procedure: 'procedure' proc_name '{' stmtLst '}'
	*/
	struct ProcedureNode : Node {
	private:
		std::vector<StatementNode> statements;
		std::string name;
	public:
		ProcedureNode(std::string name, std::vector<StatementNode> statements);

		bool operator==(const Node& other) const override;
		const std::string& getName () const;
		const std::vector<StatementNode>& getStatements() const;

	};

	/*
		program: procedure+
	*/
	struct ProgramNode : Node {
	private:
		std::vector<std::unique_ptr<ProcedureNode>> procedures;
	public:
		ProgramNode(std::vector<std::unique_ptr<ProcedureNode>> procedures);

		bool operator==(const Node& other) const override;
		const std::vector<std::unique_ptr<ProcedureNode>>& getProcedures() const;
	};
}
