#include "UsesExtractor.h"

void UsesExtractor::extractStatementListUses(const Ent::Procedure p, const vector<StatementNode> &statements) {
    for (const auto& statement : statements) {
		visit([&](auto& stmt) {
			extractStatementUses(p, stmt);
			}, statement);
    }
}

void UsesExtractor::extractStatementUses(const Ent::Procedure p, const unique_ptr<ReadStatementNode> &read) {
    return; // Read statement does not use any variable
}

void UsesExtractor::extractStatementUses(const Ent::Procedure p, const unique_ptr<PrintStatementNode> &print) {
    Ent::PrintStatement pn{ print->getStatementNo(), print->getVariable()->getName() };
    Ent::Variable var{ print->getVariable()->getName() };

    Rel::Uses uses{ pn, var };
    usesRelationships.push_back(move(uses));

    addNestedUsesToVector(p, var);
}

void UsesExtractor::extractStatementUses(const Ent::Procedure p, const unique_ptr<AssignStatementNode> &assign) {
    Ent::AssignStatement assignment{ assign->getStatementNo() };

    vector<Ent::Variable> expressionVariables;
    DesignExtractorUtil::extractExpressionVariables(expressionVariables, assign->getRHS());
    for (const auto& var : expressionVariables) {
        Rel::Uses uses{ assignment, var };
        usesRelationships.push_back(move(uses));

        addNestedUsesToVector(p, var);
    }
}

void UsesExtractor::extractStatementUses(const Ent::Procedure p, const unique_ptr<WhileStatementNode> &whiles) {
    Ent::WhileStatement w{ whiles->getStatementNo() };
    statementStack.push_back(w);

    vector<Ent::Variable> controlVariables;
    DesignExtractorUtil::extractConditionVariables(controlVariables, whiles->getCondition());
    for (const auto& var : controlVariables) addNestedUsesToVector(p, var);

    extractStatementListUses(p, whiles->getStatements());

    statementStack.pop_back();
}

void UsesExtractor::extractStatementUses(const Ent::Procedure p, const unique_ptr<IfStatementNode> &ifs) {
    Ent::IfStatement i{ ifs->getStatementNo() };
    statementStack.push_back(i);

    vector<Ent::Variable> controlVariables;
    DesignExtractorUtil::extractConditionVariables(controlVariables, ifs->getCondition());
    for (const auto& var : controlVariables) addNestedUsesToVector(p, var);

    extractStatementListUses(p, ifs->getThenStatements());
    extractStatementListUses(p, ifs->getElseStatements());

    statementStack.pop_back();
}

void UsesExtractor::extractStatementUses(const Ent::Procedure p, const unique_ptr<CallStatementNode> &call) {
    Ent::Procedure called{ call->getProcName()};
    CalledProcedure calledProcedure{ called, call->getStatementNo()};

    DesignExtractorUtil::insertIntoProcedureMap(p, calledProcedure, procedureCallMap);

    for (const auto& stmt : statementStack) {
        DesignExtractorUtil::insertIntoStatementMap(stmt, calledProcedure, statementCallMap);
    }
}

void UsesExtractor::extractIndirectProcedureUses() {
    auto procedureCallPairs = DesignExtractorUtil::convertMapToPairs(procedureCallMap);
    
    for (const auto& [caller, calledProcedure] : procedureCallPairs) {
        stmtNo_t calledProcedureStmtNo = calledProcedure.getStmtNo();
        for (const auto& rel : procedureUsesRelationships) {
            if (*rel.getArg1() != calledProcedure.getProcedure()) continue;

            auto p = make_shared<Ent::Procedure>(caller);
            auto var = dynamic_pointer_cast<Ent::Variable>(rel.getArg2());
            Rel::Uses procedureUses{ p, var };
            usesRelationships.push_back(move(procedureUses));

            auto callStatement = make_shared<Ent::CallStatement>(calledProcedureStmtNo, calledProcedure.getProcedure().getName());
            Rel::Uses callUses{ callStatement, var };
            usesRelationships.push_back(move(callUses));
        }
    }
}

void UsesExtractor::extractIndirectStatementUses() {
    auto statementCallPairs = DesignExtractorUtil::convertMapToPairs(statementCallMap);

    for (const auto& [statement, calledProcedure] : statementCallPairs) {
        for (const auto& rel : procedureUsesRelationships) {
            if (*rel.getArg1() != calledProcedure.getProcedure()) continue;

            auto stmt = make_shared<Ent::Statement>(statement);
            auto var = dynamic_pointer_cast<Ent::Variable>(rel.getArg2());
            Rel::Uses stmtUses{ stmt, var };
            usesRelationships.push_back(move(stmtUses));
        }
    }
}

void UsesExtractor::addNestedUsesToVector(const Ent::Procedure p, const Ent::Variable var) {
    for (const auto& statement : statementStack) {
        Rel::Uses nestedUses{ statement, var };
        usesRelationships.push_back(move(nestedUses));
    }

    Rel::Uses procedureUses{ p, var };
    usesRelationships.push_back(procedureUses);
    procedureUsesRelationships.push_back(procedureUses);
}

UsesExtractor::UsesExtractor() {}

vector<Rel::Uses> UsesExtractor::extractUses(const unique_ptr<ProgramNode> &AST) {
	for (const auto& procedure : AST->getProcedures()) {
        Ent::Procedure p{ procedure->getName()};
        extractStatementListUses(p, procedure->getStatements());
	}

    DesignExtractorUtil::generateAllStatementCalls(procedureCallMap, statementCallMap);
    DesignExtractorUtil::generateAllProcedureCalls(procedureCallMap);

    extractIndirectProcedureUses();
	extractIndirectStatementUses();

    return usesRelationships;
}