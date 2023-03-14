#include "./ExpressionNode.h"

namespace SP {
	ConstantNode::ConstantNode(std::string value) : value(value) {}

	bool ConstantNode::operator==(const Node& other) const {
		auto otherConstant = dynamic_cast<const ConstantNode*>(&other);
		return otherConstant && this->value == otherConstant->value;
	}

	const std::string& ConstantNode::getValue() const {
		return value;
	}

	VariableNode::VariableNode(std::string name) : name(name) {}

	bool VariableNode::operator==(const Node& other) const {
		auto otherVariable = dynamic_cast<const VariableNode*>(&other);
		return otherVariable && this->name == otherVariable->name;
	}

	const std::string& VariableNode::getName() const {
		return name;
	}

	BinaryOperatorNode::BinaryOperatorNode(std::string op,
		ExpressionNode LHS, ExpressionNode RHS) : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

	bool BinaryOperatorNode::operator==(const Node& other) const {
		auto otherBnOpNode = dynamic_cast<const BinaryOperatorNode*>(&other);
		return otherBnOpNode &&
			this->op == otherBnOpNode->op &&
			visit(compareExpression, this->LHS, otherBnOpNode->LHS) &&
			visit(compareExpression, this->RHS, otherBnOpNode->RHS);
	}

	const std::string& BinaryOperatorNode::getOp() const {
		return op;
	}
	const ExpressionNode& BinaryOperatorNode::getLHS() const {
		return LHS;
	}
	const ExpressionNode& BinaryOperatorNode::getRHS() const {
		return RHS;
	}
};
