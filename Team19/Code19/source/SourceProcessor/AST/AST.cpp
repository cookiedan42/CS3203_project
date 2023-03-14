#include "AST.h"

namespace SP {
	ProcedureNode::ProcedureNode(std::string name, std::vector<StatementNode> statements) :
		name(name), statements(std::move(statements)) {}

	bool ProcedureNode::operator==(const Node& other) const {
		auto otherProcedure = dynamic_cast<const ProcedureNode*>(&other);
		return otherProcedure &&
			this->name == otherProcedure->name &&
			equal(this->statements.begin(), this->statements.end(),
				otherProcedure->statements.begin(), compareStatements);
	}

	const std::string& ProcedureNode::getName() const {
		return name;
	}
	const std::vector<StatementNode>& ProcedureNode::getStatements() const {
		return statements;
	}

	ProgramNode::ProgramNode(std::vector<std::unique_ptr<ProcedureNode>> procedures) : procedures(std::move(procedures)) {}

	bool ProgramNode::operator==(const Node& other) const {
		auto otherProgram = dynamic_cast<const ProgramNode*>(&other);
		return otherProgram &&
			equal(this->procedures.begin(), this->procedures.end(), otherProgram->procedures.begin(),
				[](const auto& thisProcedure, const auto& otherProcedure) {
					return *thisProcedure == *otherProcedure;
				});
	}
	const std::vector<std::unique_ptr<ProcedureNode>>& ProgramNode::getProcedures() const {
		return procedures;
	}
}