#include "Entity.h"

Entity::Entity(std::string name, std::string entityType) {
    this->name = name;
    this->entityType = entityType;
}

std::string Entity::getName() const {
    return this->name;
}

std::string Entity::getEntityType() const {
    return this->entityType;
}

bool Entity::operator!=(const Entity& other) const {
    return !(*this == other);
}

Ent::Constant::Constant(std::string name) : Entity(name, const_entityType) {}

bool Ent::Constant::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::Constant*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType();
}

Ent::Variable::Variable(std::string name) : Entity(name, var_entityType) {}

bool Ent::Variable::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::Variable*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType();
}

Ent::Procedure::Procedure(std::string name) : Entity(name, proc_entityType) {}

bool Ent::Procedure::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::Procedure*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType();
}

Ent::Statement::Statement(int stmtNo, std::string stmtType) :
    Entity(stmtType, stmt_entityType), stmtNo(stmtNo) {}

int Ent::Statement::getStmtNo() const {
    return this->stmtNo;
}

std::string Ent::Statement::getOptionalArg() const{
    return this->optionalArg;
}

bool Ent::Statement::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::Statement*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType() &&
        this->getStmtNo() == casted->getStmtNo();
}

Ent::ReadStatement::ReadStatement(int stmtNo, std::string varName) : Statement(stmtNo, read_stmtType) {
    this->optionalArg = varName;
}

Ent::PrintStatement::PrintStatement(int stmtNo, std::string varName) : Statement(stmtNo, print_stmtType) {
    this->optionalArg = varName;
}

Ent::AssignStatement::AssignStatement(int stmtNo) : Statement(stmtNo, assign_stmtType) {}

Ent::CallStatement::CallStatement(int stmtNo, std::string procName) : Statement(stmtNo, call_stmtType) {
    this->optionalArg = procName;
}

Ent::WhileStatement::WhileStatement(int stmtNo) : Statement(stmtNo, while_stmtType) {}

Ent::IfStatement::IfStatement(int stmtNo) : Statement(stmtNo, if_stmtType) {}

bool Ent::ReadStatement::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::ReadStatement*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType() &&
        this->getStmtNo() == casted->getStmtNo() &&
        this->optionalArg == casted->optionalArg;
}

bool Ent::PrintStatement::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::PrintStatement*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType() &&
        this->getStmtNo() == casted->getStmtNo() &&
        this->optionalArg == casted->optionalArg;
}

bool Ent::AssignStatement::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::AssignStatement*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType() &&
        this->getStmtNo() == casted->getStmtNo();
}

bool Ent::CallStatement::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::CallStatement*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType() &&
        this->getStmtNo() == casted->getStmtNo() &&
        this->optionalArg == casted->optionalArg;
}
bool Ent::WhileStatement::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::WhileStatement*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType() &&
        this->getStmtNo() == casted->getStmtNo();
}
bool Ent::IfStatement::operator==(const Entity& other) const {
    auto casted = dynamic_cast<const Ent::IfStatement*>(&other);

    return casted && this->getName() == casted->getName() &&
        this->getEntityType() == casted->getEntityType() &&
        this->getStmtNo() == casted->getStmtNo();
}