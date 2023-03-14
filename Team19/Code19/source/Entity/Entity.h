#pragma once
#include <string>
#include "../TypeAlias/TypeAlias.h"
#include "../Definitions/Definitions.h"

class Entity {
protected:
    std::string name;
    std::string entityType;

public:
    Entity() = default;
    Entity(std::string name, std::string entityType);

    std::string getName() const;
    std::string getEntityType() const;

    virtual bool operator==(const Entity& other) const = 0;
    bool operator!=(const Entity& other) const;

    ~Entity() = default;
};

namespace Ent {
    struct Constant : public Entity {
        Constant(std::string name);

        bool operator==(const Entity& other) const override;
    };

    struct Variable : public Entity {
        Variable(std::string name);

        bool operator==(const Entity& other) const override;
    };

    struct Procedure : public Entity {
        Procedure(std::string name);

        bool operator==(const Entity& other) const override;
    };

    struct Statement : public Entity {
    protected:
        int stmtNo;
        std::string optionalArg;
    public:
        Statement(int stmtNo, std::string stmtType);

        int getStmtNo() const;
        std::string Statement::getOptionalArg() const;

        virtual bool operator==(const Entity& other) const override;
    };

    struct ReadStatement : Statement {
        ReadStatement(int stmtNo, std::string varName);
        bool operator==(const Entity& other) const override;
    };
    
    struct PrintStatement : Statement {
        PrintStatement(int stmtNo, std::string varName);
        bool operator==(const Entity& other) const override;
    };

    struct AssignStatement : Statement {
        AssignStatement(int stmtNo);
        bool operator==(const Entity& other) const override;
    };

    struct CallStatement : Statement {
        CallStatement(int stmtNo, std::string procName);
        bool operator==(const Entity& other) const override;
    };

    struct WhileStatement : Statement {
        WhileStatement(int stmtNo);
        bool operator==(const Entity& other) const override;
    };

    struct IfStatement : Statement {
        IfStatement(int stmtNo);
        bool operator==(const Entity& other) const override;
    };
}

namespace std {
    template<> struct hash<Ent::Constant> {
        size_t operator()(const Ent::Constant& entity) const {
            size_t nameHash = std::hash<std::string>()(entity.getName()) << 1;
            size_t typeHash = std::hash<std::string>()(entity.getEntityType());
            return nameHash ^ typeHash;
        }
    };
    template<> struct hash<Ent::Variable> {
        size_t operator()(const Ent::Variable& entity) const {
            size_t nameHash = std::hash<std::string>()(entity.getName()) << 1;
            size_t typeHash = std::hash<std::string>()(entity.getEntityType());
            return nameHash ^ typeHash;
        }
    };
    template<> struct hash<Ent::Procedure> {
        size_t operator()(const Ent::Procedure& entity) const {
            size_t nameHash = std::hash<std::string>()(entity.getName()) << 1;
            size_t typeHash = std::hash<std::string>()(entity.getEntityType());
            return nameHash ^ typeHash;
        }
    };
    template<> struct hash<Ent::Statement> {
        size_t operator()(const Ent::Statement& entity) const {
            size_t nameHash = std::hash<std::string>()(entity.getName()) << 1;
            size_t typeHash = std::hash<std::string>()(entity.getEntityType());
            size_t stmtNoHash = std::hash<int>()(entity.getStmtNo());
            return nameHash ^ typeHash ^ stmtNoHash;
        }
    };

}
