#pragma once
#include <string>
#include "../Entity/Entity.h"
#include "../TypeAlias/TypeAlias.h"
#include "../Definitions/Definitions.h"
#include <unordered_set>
#include <unordered_map>

class Pattern {
protected:
    stmtNoRef stmtNo;
    patternType type;

public:
    Pattern(stmtNoRef stmtNo, std::string type);

    stmtNoRef getStmtNo() const;
    patternType getType() const;
    virtual bool operator==(const Pattern& other) const = 0;
    bool operator!=(const Pattern& other) const;
};

namespace std {
    template<> struct hash<Pattern> {
        size_t operator()(Pattern const& pattern) const {
            size_t stmtNoHash = std::hash<std::string>()(pattern.getStmtNo());
            size_t nameHash = std::hash<std::string>()(pattern.getType()) << 1;
            return stmtNoHash ^ nameHash;
        }
    };
}

namespace Ptn {
    struct AssignPtn : Pattern {
    private:
        lhsRef LHS;
        stmtNoLhsPair stmtNoLhsPr;
        rhsRef inOrderRHS;
        rhsRef preOrderRHS;

    public:
        AssignPtn(int stmtNo, lhsRef lhs, rhsRef preOrderRHS, rhsRef inOrderRHS);

        lhsRef getLhs() const;
        stmtNoLhsPair getStmtNoLhsPair() const;
        rhsRef getInOrderRHS() const;
        rhsRef getPreOrderRHS() const;

        bool operator==(const Pattern& other) const override;
    };

    struct IfPtn : Pattern {
    private:
        condVarList varSet;

    public:
        IfPtn(int stmtNo, std::vector<Ent::Variable> conditional);

        condVarList getVarSet() const;

        bool operator==(const Pattern& other) const override;
    };

    struct WhilePtn : Pattern {
    private:
        condVarList varSet;

    public:
        WhilePtn(int stmtNo, std::vector<Ent::Variable> conditional);

        condVarList getVarSet() const;

        bool operator==(const Pattern& other) const override;
    };
}
