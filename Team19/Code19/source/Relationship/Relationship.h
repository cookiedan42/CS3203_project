#pragma once
#include <string>
#include <memory>
#include "../../source/Entity/Entity.h"
#include "../Definitions/Definitions.h"
#include "../TypeAlias/TypeAlias.h"

using namespace std;
using namespace Ent;

enum class Arg1Type {
    STATEMENT,
    PROCEDURE,
    ANY
};

class Relationship {
protected:
    shared_ptr<Entity> arg1;
    shared_ptr<Entity> arg2;
    RelationshipType relationshipType;

public:
    Relationship() = default;
    Relationship(shared_ptr<Entity> arg1, shared_ptr<Entity> arg2);

    const shared_ptr<Entity>& getArg1() const;
    const shared_ptr<Entity>& getArg2() const;
    RelationshipType getRelationshipType() const;

    bool operator==(const Relationship& other) const;
};

namespace Rel {
    struct Follows : Relationship {
        Follows() = default;
        Follows(Ent::Statement arg1, Ent::Statement arg2);
    };

    struct Parent : Relationship {
        Parent() = default;
        Parent(Ent::Statement arg1, Ent::Statement arg2);
    };

    struct Modifies : Relationship {
    private:
        Arg1Type arg1Type;
    public:
        Modifies() = default;
        Modifies(Ent::Procedure arg1, Ent::Variable arg2);
        Modifies(Ent::Statement arg1, Ent::Variable arg2);
        Modifies(shared_ptr<Ent::Procedure> arg1, shared_ptr<Ent::Variable> arg2);
        Modifies(shared_ptr<Ent::Statement> arg1, shared_ptr<Ent::Variable> arg2);

        Arg1Type getArg1Type() const;
    };

    struct Uses : Relationship {
    private:
        Arg1Type arg1Type;
    public:
        Uses() = default;
        Uses(Ent::Procedure arg1, Ent::Variable arg2);
        Uses(Ent::Statement arg1, Ent::Variable arg2);
        Uses(shared_ptr<Ent::Procedure> arg1, shared_ptr<Ent::Variable> arg2);
        Uses(shared_ptr<Ent::Statement> arg1, shared_ptr<Ent::Variable> arg2);

        Arg1Type getArg1Type() const;
    };

    struct Calls : Relationship {
        Calls() = default;
        Calls(Ent::Procedure arg1, Ent::Procedure arg2);
    };
}
