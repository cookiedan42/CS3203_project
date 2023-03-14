#include "DesignExtractorUtil.h"

CalledProcedure::CalledProcedure(Ent::Procedure procedure, stmtNo_t stmtNo) :
    procedure(procedure), stmtNo(stmtNo) {}

Ent::Procedure CalledProcedure::getProcedure() const {
    return procedure;
}

stmtNo_t CalledProcedure::getStmtNo() const {
    return stmtNo;
}

bool CalledProcedure::operator==(const CalledProcedure& other) const {
    if (this == &other) return true;

    if (typeid(this) != typeid(other)) return false;

    return this->getProcedure() == other.getProcedure() && this->getStmtNo() == other.getStmtNo();
}

void DesignExtractorUtil::generateAllProcedureCalls(ProcedureCallMap& procedureCallMap) {
    ProcedureCallMap tempPMap = procedureCallMap;
    for (const auto& [procedure, callees] : tempPMap) {
        stack<CalledProcedure> calledStack;
        for (const auto& calledProcedure : callees) {
            calledStack.push(calledProcedure);
        }

        while (!calledStack.empty()) {
            CalledProcedure calledProcedure = calledStack.top();
            calledStack.pop();

            unordered_set<CalledProcedure> current = procedureCallMap[procedure];
            current.insert(calledProcedure);
            procedureCallMap[procedure] = current;

            stmtNo_t initialCallStmtNo = calledProcedure.getStmtNo();

            auto it = tempPMap.find(calledProcedure.getProcedure());
            if (it == tempPMap.end()) continue; // No indirect call

            for (const auto& indirect : it->second) {
                CalledProcedure c{ indirect.getProcedure(), initialCallStmtNo };
                calledStack.push(c);
            }
        }
    }
}

void DesignExtractorUtil::generateAllStatementCalls(ProcedureCallMap& procedureCallMap, StatementCallMap& statementCallMap) {
    StatementCallMap tempSMap = statementCallMap;
    for (const auto& [statement, callees] : tempSMap) {
        stack<CalledProcedure> calledStack;
        for (const auto& calledProcedure : callees) {
            calledStack.push(calledProcedure);
        }

        while (!calledStack.empty()) {
            CalledProcedure calledProcedure = calledStack.top();
            calledStack.pop();

            unordered_set<CalledProcedure> current = statementCallMap[statement];
            current.insert(calledProcedure);
            statementCallMap[statement] = current;

            stmtNo_t initialCallStmtNo = calledProcedure.getStmtNo();

            auto it = procedureCallMap.find(calledProcedure.getProcedure());
            if (it == procedureCallMap.end()) continue; // No indirect call

            for (const auto& indirect : it->second) {
                CalledProcedure c{ indirect.getProcedure(), initialCallStmtNo };
                calledStack.push(c);
            }
        }
    }
}

void DesignExtractorUtil::insertIntoProcedureMap(const Ent::Procedure& key, const CalledProcedure& element, ProcedureCallMap& map) {
    if (map.count(key)) {
        unordered_set<CalledProcedure> current = map[key];
        current.insert(element);
        map[key] = current;
    }
    else {
        unordered_set<CalledProcedure> current;
        current.insert(element);
        map[key] = current;
    }
}

void DesignExtractorUtil::insertIntoStatementMap(const Ent::Statement& key, const CalledProcedure& element, StatementCallMap& map) {
    if (map.count(key)) {
        unordered_set<CalledProcedure> current = map[key];
        current.insert(element);
        map[key] = current;
    }
    else {
        unordered_set<CalledProcedure> current;
        current.insert(element);
        map[key] = current;
    }
}

ProcedureCallPairs DesignExtractorUtil::convertMapToPairs(ProcedureCallMap map) {
    ProcedureCallPairs pairs;

    for (const auto& [caller, callee] : map) {
        for (const auto& calledProcedure : callee) {
            pairs.push_back({ caller, calledProcedure });
        }
    }

    return pairs;
}

StatementCallPairs DesignExtractorUtil::convertMapToPairs(StatementCallMap map) {
    StatementCallPairs pairs;

    for (const auto& [caller, callee] : map) {
        for (const auto& calledProcedure : callee) {
            pairs.push_back({ caller, calledProcedure });
        }
    }

    return pairs;
}

Ent::Statement DesignExtractorUtil::extractStatementEntity(const unique_ptr<SP::ReadStatementNode>& stmt) {
    return Ent::ReadStatement(stmt->getStatementNo(), stmt->getVariable()->getName());
}
Ent::Statement DesignExtractorUtil::extractStatementEntity(const unique_ptr<SP::PrintStatementNode>& stmt) {
    return Ent::PrintStatement(stmt->getStatementNo(), stmt->getVariable()->getName());
}
Ent::Statement DesignExtractorUtil::extractStatementEntity(const unique_ptr<SP::AssignStatementNode>& stmt) {
    return Ent::AssignStatement(stmt->getStatementNo());
}
Ent::Statement DesignExtractorUtil::extractStatementEntity(const unique_ptr<SP::WhileStatementNode>& stmt) {
    return Ent::WhileStatement(stmt->getStatementNo());
}
Ent::Statement DesignExtractorUtil::extractStatementEntity(const unique_ptr<SP::IfStatementNode>& stmt) {
    return Ent::IfStatement(stmt->getStatementNo());
}
Ent::Statement DesignExtractorUtil::extractStatementEntity(const unique_ptr<SP::CallStatementNode>& stmt) {
    return Ent::CallStatement(stmt->getStatementNo(), stmt->getProcName());
}

void DesignExtractorUtil::extractConditionVariables(
    vector<Ent::Variable>& variables, const unique_ptr<SP::ConditionalExpressionNode>& condition) {
    if (condition->getOp() == SIMPLE::NOT) {
        extractConditionVariables(variables, condition->getCondLHS());
    }
    else if (condition->getOp() == SIMPLE::AND || condition->getOp() == SIMPLE::OR) {
        extractConditionVariables(variables, condition->getCondLHS());
        extractConditionVariables(variables, condition->getCondRHS());
    }
    else {
        extractExpressionVariables(variables, condition->getRelLHS());
        extractExpressionVariables(variables, condition->getRelRHS());
    }
}

void DesignExtractorUtil::extractExpressionVariables(
    vector<Ent::Variable>& variables, const SP::ExpressionNode& expression) {
    visit([&](const auto& expr) {
        extractRelationalFactorVariables(variables, expr);
        }, expression);
}

void DesignExtractorUtil::extractRelationalFactorVariables(
    vector<Ent::Variable>& variables, const unique_ptr<SP::BinaryOperatorNode>& bnOp) {
    visit([&](const auto& factor) {
        extractRelationalFactorVariables(variables, factor);
        }, bnOp->getLHS());
    visit([&](const auto& factor) {
        extractRelationalFactorVariables(variables, factor);
        }, bnOp->getRHS());
}

void DesignExtractorUtil::extractRelationalFactorVariables(
    vector<Ent::Variable>& variables, const unique_ptr<SP::VariableNode>& variable) {
    Ent::Variable var{ variable->getName() };
    variables.push_back(var);
}

void DesignExtractorUtil::extractRelationalFactorVariables(
    vector<Ent::Variable>& variables, const unique_ptr<SP::ConstantNode>& constant) {
    return;
}

string DesignExtractorUtil::getPreOrderTraversal(const SP::ExpressionNode& expression) {
    string preOrder = visit([](auto& expr) {
        return traverseExpression(expr, true);
        }, expression);

    return preOrder;
}

string DesignExtractorUtil::getInOrderTraversal(const SP::ExpressionNode& expression) {
    string inOrder = visit([](auto& expr) {
        return traverseExpression(expr);
        }, expression);

    return inOrder;
}

string DesignExtractorUtil::traverseExpression(const unique_ptr<SP::BinaryOperatorNode>& bnOp, bool isPreOrder) {
    string LHS = visit([&](auto& expr) {
        return traverseExpression(expr, isPreOrder);
        }, bnOp->getLHS());

    string RHS = visit([&](auto& expr) {
        return traverseExpression(expr, isPreOrder);
        }, bnOp->getRHS());

    return isPreOrder ?
        SIMPLE::LEFT_BRACKET + bnOp->getOp() + LHS + RHS + SIMPLE::RIGHT_BRACKET :
        SIMPLE::LEFT_BRACKET + LHS + bnOp->getOp() + RHS + SIMPLE::RIGHT_BRACKET;
}

string DesignExtractorUtil::traverseExpression(const unique_ptr<SP::VariableNode>& variable, bool isPreOrder) {
    return SIMPLE::LEFT_BRACKET + variable->getName() + SIMPLE::RIGHT_BRACKET;
}

string DesignExtractorUtil::traverseExpression(const unique_ptr<SP::ConstantNode>& constant, bool isPreOrder) {
    return SIMPLE::LEFT_BRACKET + constant->getValue() + SIMPLE::RIGHT_BRACKET;
}
