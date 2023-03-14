#include "./StatementNode.h"


namespace SP {
	ReadStatementNode::ReadStatementNode(int statementNo, std::unique_ptr<VariableNode> variable) :
		StatementBase(statementNo), variable(std::move(variable)) {}

	bool ReadStatementNode::operator==(const Node& other) const {
		auto otherRead = dynamic_cast<const ReadStatementNode*>(&other);
		return otherRead &&
			this->statementNo == otherRead->statementNo &&
			*this->variable == *otherRead->variable;
	}

	const std::unique_ptr<VariableNode>& ReadStatementNode::getVariable() const {
		return variable;
	}

	PrintStatementNode::PrintStatementNode(int statementNo, std::unique_ptr<VariableNode> variable) :
		StatementBase(statementNo), variable(std::move(variable)) {}

	bool PrintStatementNode::operator==(const Node& other) const {
		auto otherPrint = dynamic_cast<const PrintStatementNode*>(&other);
		return otherPrint &&
			this->statementNo == otherPrint->statementNo &&
			*this->variable == *otherPrint->variable;
	}

	const std::unique_ptr<VariableNode>& PrintStatementNode::getVariable() const {
		return variable;
	}

	CallStatementNode::CallStatementNode(int statementNo, std::string procName) :
		StatementBase(statementNo), procName(procName) {}

	bool CallStatementNode::operator==(const Node& other) const {
		auto otherCall = dynamic_cast<const CallStatementNode*>(&other);
		return otherCall &&
			this->statementNo == otherCall->statementNo &&
			this->procName == otherCall->procName;
	}

	const std::string& CallStatementNode::getProcName() const {
		return procName;
	}

	WhileStatementNode::WhileStatementNode(
		int statementNo, std::unique_ptr<ConditionalExpressionNode> condition, std::vector<StatementNode> statements) :
		StatementBase(statementNo), condition(std::move(condition)), statements(std::move(statements)) {}

	bool WhileStatementNode::operator==(const Node& other) const {
		auto otherWhile = dynamic_cast<const WhileStatementNode*>(&other);
		return otherWhile &&
			this->statementNo == otherWhile->statementNo &&
			*this->condition == *otherWhile->condition &&
			equal(this->statements.begin(), this->statements.end(),
				otherWhile->statements.begin(), compareStatements);
	}

	const std::unique_ptr<ConditionalExpressionNode>& WhileStatementNode::getCondition() const {
		return condition;
	}

	const std::vector<StatementNode>& WhileStatementNode::getStatements() const {
		return statements;
	}

	IfStatementNode::IfStatementNode(int statementNo, std::unique_ptr<ConditionalExpressionNode> condition,
		std::vector<StatementNode> thenStatements, std::vector<StatementNode> elseStatements) :
		StatementBase(statementNo), condition(std::move(condition)),
		thenStatements(std::move(thenStatements)), elseStatements(std::move(elseStatements)) {}

	bool IfStatementNode::operator==(const Node& other) const {
		auto otherIf = dynamic_cast<const IfStatementNode*>(&other);
		return otherIf &&
			this->statementNo == otherIf->statementNo &&
			*this->condition == *otherIf->condition &&
			equal(this->thenStatements.begin(), this->thenStatements.end(),
				otherIf->thenStatements.begin(), compareStatements) &&
			equal(this->elseStatements.begin(), this->elseStatements.end(),
				otherIf->elseStatements.begin(), compareStatements);
	}
	const std::unique_ptr<ConditionalExpressionNode>& IfStatementNode::getCondition() const {
		return condition;
	}
	const std::vector<StatementNode>& IfStatementNode::getThenStatements() const {
		return thenStatements;
	}
	const std::vector<StatementNode>& IfStatementNode::getElseStatements() const {
		return elseStatements;
	}


	AssignStatementNode::AssignStatementNode(
		int statementNo, std::unique_ptr<VariableNode> LHS, ExpressionNode RHS) :
		StatementBase(statementNo), LHS(std::move(LHS)), RHS(std::move(RHS)) {}

	bool AssignStatementNode::operator==(const Node& other) const {
		auto otherAssign = dynamic_cast<const AssignStatementNode*>(&other);
		return otherAssign &&
			this->statementNo == otherAssign->statementNo &&
			*this->LHS == *otherAssign->LHS &&
			visit(compareExpression, this->RHS, otherAssign->RHS);
	}

	const std::unique_ptr<VariableNode>& AssignStatementNode::getLHS() const{
		return LHS;
	}

	const ExpressionNode& AssignStatementNode::getRHS() const {
		return RHS;
	}
}