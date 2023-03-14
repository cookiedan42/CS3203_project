#pragma once
#include "./BaseNode.h"
#include <memory>
// Nodes representing the RHS of an assign Statement

namespace SP {
	struct BinaryOperatorNode;
	struct ConstantNode;
	struct VariableNode;
	using ExpressionNode = std::variant<
		std::unique_ptr<ConstantNode>,
		std::unique_ptr<VariableNode>,
		std::unique_ptr<BinaryOperatorNode>
	>;

	/*
	const_value: INTEGER
*/
	struct ConstantNode : Node {
	private:
		const std::string value;
	public:
		ConstantNode(std::string value);

		bool operator==(const Node& other) const override;
		const std::string& getValue() const;
	};

	/*
		var_name, proc_name: NAME
	*/
	struct VariableNode : Node {
	private:
		const std::string name;
	public:
		VariableNode(std::string name);

		bool operator==(const Node& other) const override;
		const std::string& getName() const;
	};

	struct BinaryOperatorNode : Node {
	private:
		std::string op;
		ExpressionNode LHS;
		ExpressionNode RHS;
	public:
		BinaryOperatorNode(std::string op, ExpressionNode LHS, ExpressionNode RHS);

		bool operator==(const Node& other) const override;
		const std::string& getOp() const;
		const ExpressionNode& getLHS() const;
		const ExpressionNode& getRHS() const;
	};
}
