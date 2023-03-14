#include "Pattern.h"

Pattern::Pattern(stmtNoRef stmtNo, patternType type) : stmtNo(stmtNo), type(type) {}

stmtNoRef Pattern::getStmtNo() const {
    return this->stmtNo;
}

patternType Pattern::getType() const {
    return this->type;
}

bool Pattern::operator!=(const Pattern& other) const {
    return !(*this == other);
}

Ptn::AssignPtn::AssignPtn(int stmtNo, lhsRef LHS, rhsRef preOrderRHS, rhsRef inOrderRHS) :
    Pattern(std::to_string(stmtNo), ASSIGNMENTTYPE),
    LHS(LHS), preOrderRHS(preOrderRHS), inOrderRHS(inOrderRHS), stmtNoLhsPr({ std::to_string(stmtNo), LHS }) {}

lhsRef Ptn::AssignPtn::getLhs() const {
    return this->LHS;
}

stmtNoLhsPair Ptn::AssignPtn::getStmtNoLhsPair() const {
    return this->stmtNoLhsPr;
}

rhsRef Ptn::AssignPtn::getInOrderRHS() const {
    return this->inOrderRHS;
}

rhsRef Ptn::AssignPtn::getPreOrderRHS() const {
    return this->preOrderRHS;
}

bool Ptn::AssignPtn::operator==(const Pattern& other) const {
    auto casted = dynamic_cast<const Ptn::AssignPtn*>(&other);

    return casted &&
        this->getStmtNo() == casted->getStmtNo() &&
        this->getType() == casted->getType() &&
        this->getLhs() == casted->getLhs() &&
        this->getStmtNoLhsPair() == casted->getStmtNoLhsPair() &&
        this->getPreOrderRHS() == casted->getPreOrderRHS() &&
        this->getInOrderRHS() == casted->getInOrderRHS();
}

Ptn::IfPtn::IfPtn(int stmtNo, std::vector<Ent::Variable> conditional) :
    Pattern(std::to_string(stmtNo), IFTYPE) {
    for (Ent::Variable var : conditional) { // inserts into a set. This is to ensure no duplicates
        this->varSet.insert(var.getName());
    }
}

condVarList Ptn::IfPtn::getVarSet() const {
    return this->varSet;
}

bool Ptn::IfPtn::operator==(const Pattern& other) const {
    auto casted = dynamic_cast<const Ptn::IfPtn*>(&other);

    return casted &&
        this->getStmtNo() == casted->getStmtNo() &&
        this->getType() == casted->getType() &&
        this->getVarSet() == casted->getVarSet();
}

Ptn::WhilePtn::WhilePtn(int stmtNo, std::vector<Ent::Variable> conditional) :
    Pattern(std::to_string(stmtNo), WHILETYPE) {
    for (Ent::Variable var : conditional) { // inserts into a set. This is to ensure no duplicates
        this->varSet.insert(var.getName());
    }
}

condVarList Ptn::WhilePtn::getVarSet() const {
    return this->varSet;
}

bool Ptn::WhilePtn::operator==(const Pattern& other) const {
    auto casted = dynamic_cast<const Ptn::WhilePtn*>(&other);

    return casted &&
        this->getStmtNo() == casted->getStmtNo() &&
        this->getType() == casted->getType() &&
        this->getVarSet() == casted->getVarSet();
}
