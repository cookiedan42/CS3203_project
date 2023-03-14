#include "./ConditionalExpressionNode.h"

namespace SP {
	ConditionalExpressionNode::ConditionalExpressionNode(std::string op) : op(op) {}

	ConditionalExpressionNode::ConditionalExpressionNode(std::string op,
		std::unique_ptr<ConditionalExpressionNode> condLHS) :
		op(op), condLHS(std::move(condLHS)) {}

	ConditionalExpressionNode::ConditionalExpressionNode(std::string op,
		std::unique_ptr<ConditionalExpressionNode> condLHS, std::unique_ptr<ConditionalExpressionNode> condRHS) :
		op(op), condLHS(std::move(condLHS)), condRHS(std::move(condRHS)) {}

	ConditionalExpressionNode::ConditionalExpressionNode(std::string op,
		RelationalFactorNode relLHS, RelationalFactorNode relRHS) :
		op(op), relLHS(std::move(relLHS)), relRHS(std::move(relRHS)) {}

	bool ConditionalExpressionNode::operator==(const Node& other) const {
		auto otherNode = dynamic_cast<const ConditionalExpressionNode*>(&other);
		if (this->getOp() == SIMPLE::NOT) {
			return otherNode && this->getOp() == otherNode->getOp() &&
				*this->getCondLHS() == *otherNode->getCondLHS();
		}
		else if (this->getOp() == SIMPLE::AND || this->getOp() == SIMPLE::OR) {
			return otherNode && this->getOp() == otherNode->getOp() &&
				*this->getCondLHS() == *otherNode->getCondLHS() && *this->getCondRHS() == *otherNode->getCondRHS();
		}
		else {
			return otherNode && this->getOp() == otherNode->getOp() &&
				visit(compareRelationalFactor, this->getRelLHS(), otherNode->getRelLHS()) &&
				visit(compareRelationalFactor, this->getRelRHS(), otherNode->getRelRHS());
		}
	}

	const std::string& ConditionalExpressionNode::getOp() const {
		return op;
	}

	const std::unique_ptr<ConditionalExpressionNode>& ConditionalExpressionNode::getCondLHS() const {
		return condLHS;
	}

	const std::unique_ptr<ConditionalExpressionNode>& ConditionalExpressionNode::getCondRHS() const {
		return condRHS;
	}

	const RelationalFactorNode& ConditionalExpressionNode::getRelLHS() const {
		return relLHS;
	}

	const RelationalFactorNode& ConditionalExpressionNode::getRelRHS() const {
		return relRHS;
	}

	NotConditionalExpressionNode::NotConditionalExpressionNode(std::unique_ptr<ConditionalExpressionNode> LHS) :
		ConditionalExpressionNode(SIMPLE::NOT, std::move(LHS)) {}

	bool NotConditionalExpressionNode::operator==(const Node& other) const {
		auto otherNode = dynamic_cast<const NotConditionalExpressionNode*>(&other);
		return otherNode && this->getOp() == otherNode->getOp() && *this->getCondLHS() == *otherNode->getCondLHS();
	}

	AndConditionalExpressionNode::AndConditionalExpressionNode(
		std::unique_ptr<ConditionalExpressionNode> LHS, std::unique_ptr<ConditionalExpressionNode> RHS) :
		ConditionalExpressionNode(SIMPLE::AND, std::move(LHS), std::move(RHS)) {}

	bool AndConditionalExpressionNode::operator==(const Node& other) const {
		auto otherNode = dynamic_cast<const AndConditionalExpressionNode*>(&other);
		return otherNode && this->getOp() == otherNode->getOp() &&
			*this->getCondLHS() == *otherNode->getCondLHS() && *this->getCondRHS() == *otherNode->getCondRHS();
	}

	OrConditionalExpressionNode::OrConditionalExpressionNode(
		std::unique_ptr<ConditionalExpressionNode> LHS, std::unique_ptr<ConditionalExpressionNode> RHS) :
		ConditionalExpressionNode(SIMPLE::OR, std::move(LHS), std::move(RHS)) {}

	bool OrConditionalExpressionNode::operator==(const Node& other) const {
		auto otherNode = dynamic_cast<const OrConditionalExpressionNode*>(&other);
		return otherNode && this->getOp() == otherNode->getOp() &&
			*this->getCondLHS() == *otherNode->getCondLHS() && *this->getCondRHS() == *otherNode->getCondRHS();
	}

	RelationalExpressionNode::RelationalExpressionNode(
		RelationalFactorNode LHS, std::string op, RelationalFactorNode RHS) :
		ConditionalExpressionNode(op, std::move(LHS), std::move(RHS)) {}

	bool RelationalExpressionNode::operator==(const Node& other) const {
		auto otherNode = dynamic_cast<const RelationalExpressionNode*>(&other);
		return otherNode && this->getOp() == otherNode->getOp() &&
			visit(compareRelationalFactor, this->getRelLHS(), otherNode->getRelLHS()) &&
			visit(compareRelationalFactor, this->getRelRHS(), otherNode->getRelRHS());
	}
}