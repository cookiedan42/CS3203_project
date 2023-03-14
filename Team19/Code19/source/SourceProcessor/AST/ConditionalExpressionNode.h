#pragma once
#include "../../Definitions/Definitions.h"
#include "./ExpressionNode.h"
// Nodes representing the inside of a condition

namespace SP {
	using RelationalFactorNode = ExpressionNode;

	/*
		cond_expr: rel_expr | '!' '(' cond_expr ')' |
		   '(' cond_expr ')' '&&' '(' cond_expr ')' |
		   '(' cond_expr ')' '||' '(' cond_expr ')'
	 */
	struct ConditionalExpressionNode : Node {
	private:
		std::string op;
		std::unique_ptr<ConditionalExpressionNode> condLHS = nullptr;
		std::unique_ptr<ConditionalExpressionNode> condRHS = nullptr;
		RelationalFactorNode relLHS;
		RelationalFactorNode relRHS;
	public:
		ConditionalExpressionNode(std::string op);
		ConditionalExpressionNode(std::string op, std::unique_ptr<ConditionalExpressionNode> condLHS);
		ConditionalExpressionNode(std::string op,
			std::unique_ptr<ConditionalExpressionNode> condLHS, std::unique_ptr<ConditionalExpressionNode> condRHS);
		ConditionalExpressionNode(std::string op, RelationalFactorNode relLHS, RelationalFactorNode relRHS);

		bool operator==(const Node& other) const override;
		const std::string& getOp() const;
		const std::unique_ptr<ConditionalExpressionNode>& getCondLHS() const;
		const std::unique_ptr<ConditionalExpressionNode>& getCondRHS() const;
		const RelationalFactorNode& getRelLHS() const;
		const RelationalFactorNode& getRelRHS() const;
		// TODO: add boolean method for type check
	};

	struct NotConditionalExpressionNode : ConditionalExpressionNode {
		NotConditionalExpressionNode(std::unique_ptr<ConditionalExpressionNode> LHS);

		bool operator==(const Node& other) const override;
	};

	struct AndConditionalExpressionNode : ConditionalExpressionNode {
		AndConditionalExpressionNode(
			std::unique_ptr<ConditionalExpressionNode> LHS, std::unique_ptr<ConditionalExpressionNode> RHS);

		bool operator==(const Node& other) const override;
	};

	struct OrConditionalExpressionNode : ConditionalExpressionNode {
		OrConditionalExpressionNode(
			std::unique_ptr<ConditionalExpressionNode> LHS, std::unique_ptr<ConditionalExpressionNode> RHS);

		bool operator==(const Node& other) const override;
	};

	/*
		rel_expr: rel_factor '>' rel_factor | rel_factor '>=' rel_factor |
		  rel_factor '<' rel_factor | rel_factor '<=' rel_factor |
		  rel_factor '==' rel_factor | rel_factor '!=' rel_factor
	*/
	struct RelationalExpressionNode : ConditionalExpressionNode {
		RelationalExpressionNode(RelationalFactorNode LHS, std::string op, RelationalFactorNode RHS);

		bool operator==(const Node& other) const override;
	};
}