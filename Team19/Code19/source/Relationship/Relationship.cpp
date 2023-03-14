#include "Relationship.h"

Relationship::Relationship(shared_ptr<Entity> arg1, shared_ptr<Entity> arg2) {
    this->arg1 = move(arg1);
    this->arg2 = move(arg2);
    this->relationshipType = DEFAULT_RS;
}

const shared_ptr<Entity>& Relationship::getArg1() const {
    return arg1;
}

const shared_ptr<Entity>& Relationship::getArg2() const {
    return arg2;
}

RelationshipType Relationship::getRelationshipType() const {
    return relationshipType;
}

bool Relationship::operator==(const Relationship& other) const {
    if (this == &other) {
        return true;
    }
    else if (typeid(this) != typeid(other)) {
        return false;
    }
    return *this->getArg1() == *other.getArg1() &&
        *this->getArg2() == *other.getArg2() &&
        this->getRelationshipType() == other.getRelationshipType();
}

Rel::Follows::Follows(Ent::Statement arg1, Ent::Statement arg2) :
    Relationship(make_shared<Ent::Statement>(arg1), make_shared<Ent::Statement>(arg2)) {
    this->relationshipType = FOLLOWS;
}

Rel::Parent::Parent(Ent::Statement arg1, Ent::Statement arg2) :
    Relationship(make_shared<Ent::Statement>(arg1), make_shared<Ent::Statement>(arg2)) {
    this->relationshipType = PARENT;
}

Rel::Modifies::Modifies(Ent::Procedure arg1, Ent::Variable arg2) :
    Relationship(make_shared<Ent::Procedure>(arg1), make_shared<Ent::Variable>(arg2)) {
    this->arg1Type = Arg1Type::PROCEDURE;
    this->relationshipType = MODIFIES;
}

Rel::Modifies::Modifies(Ent::Statement arg1, Ent::Variable arg2) :
    Relationship(make_shared<Ent::Statement>(arg1), make_shared<Ent::Variable>(arg2)) {
    this->arg1Type = Arg1Type::STATEMENT;
    this->relationshipType = MODIFIES;
}

Rel::Modifies::Modifies(shared_ptr<Ent::Procedure> arg1, shared_ptr<Ent::Variable> arg2) :
    Relationship(move(arg1), move(arg2)) {
    this->arg1Type = Arg1Type::PROCEDURE;
    this->relationshipType = MODIFIES;
}

Rel::Modifies::Modifies(shared_ptr<Ent::Statement> arg1, shared_ptr<Ent::Variable> arg2) :
    Relationship(move(arg1), move(arg2)) {
    this->arg1Type = Arg1Type::STATEMENT;
    this->relationshipType = MODIFIES;
}

Arg1Type Rel::Modifies::getArg1Type() const {
    return arg1Type;
}

Rel::Uses::Uses(Ent::Procedure arg1, Ent::Variable arg2) :
    Relationship(make_shared<Ent::Procedure>(arg1), make_shared<Ent::Variable>(arg2)) {
    this->arg1Type = Arg1Type::PROCEDURE;
    this->relationshipType = USES;
}

Rel::Uses::Uses(Ent::Statement arg1, Ent::Variable arg2) :
    Relationship(make_shared<Ent::Statement>(arg1), make_shared<Ent::Variable>(arg2)) {
    this->arg1Type = Arg1Type::STATEMENT;
    this->relationshipType = USES;
}

Rel::Uses::Uses(shared_ptr<Ent::Procedure> arg1, shared_ptr<Ent::Variable> arg2) :
    Relationship(move(arg1), move(arg2)) {
    this->arg1Type = Arg1Type::PROCEDURE;
    this->relationshipType = USES;
}

Rel::Uses::Uses(shared_ptr<Ent::Statement> arg1, shared_ptr<Ent::Variable> arg2) :
    Relationship(move(arg1), move(arg2)) {
    this->arg1Type = Arg1Type::STATEMENT;
    this->relationshipType = USES;
}

Arg1Type Rel::Uses::getArg1Type() const {
    return arg1Type;
}

Rel::Calls::Calls(Ent::Procedure arg1, Ent::Procedure arg2) :
    Relationship(make_shared<Ent::Procedure>(arg1), make_shared<Ent::Procedure>(arg2)) {
    this->relationshipType = CALLS;
}
