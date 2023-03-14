#include "ModifiesExtractor.h"

void ModifiesExtractor::extractStatementListModifies(const Ent::Procedure p, const vector<StatementNode> &statements) {
    for (const auto& statement : statements) {
		visit([&](const auto& stmt) {
			extractStatementModifies(p, stmt);
			}, statement);
    }
}

void ModifiesExtractor::extractStatementModifies(const Ent::Procedure p, const unique_ptr<ReadStatementNode> &read) {
    Ent::ReadStatement re{ read->getStatementNo(), read->getVariable()->getName() };
    Ent::Variable var{ read->getVariable()->getName() };
    Rel::Modifies modifies{ re, var };
    modifiesRelationships.push_back(move(modifies));

    addNestedModifiesToVector(p, var);
}

void ModifiesExtractor::extractStatementModifies(const Ent::Procedure p, const unique_ptr<PrintStatementNode> &print) {
    return; // Print statement does not modify any variable
}

void ModifiesExtractor::extractStatementModifies(const Ent::Procedure p, const unique_ptr<AssignStatementNode> &assign) {
    Ent::AssignStatement assignment{ assign->getStatementNo() };
    Ent::Variable var{ assign->getLHS()->getName() };

    Rel::Modifies modifies{ assignment, var };
    modifiesRelationships.push_back(move(modifies));

    addNestedModifiesToVector(p, var);
}

void ModifiesExtractor::extractStatementModifies(const Ent::Procedure p, const unique_ptr<WhileStatementNode> &whiles) {
    Ent::WhileStatement w{ whiles->getStatementNo() };
    statementStack.push_back(w);

    extractStatementListModifies(p, whiles->getStatements());

    statementStack.pop_back();
}

void ModifiesExtractor::extractStatementModifies(const Ent::Procedure p, const unique_ptr<IfStatementNode> &ifs) {
    Ent::IfStatement i{ ifs->getStatementNo() };
    statementStack.push_back(i);

    extractStatementListModifies(p, ifs->getThenStatements());
    extractStatementListModifies(p, ifs->getElseStatements());

    statementStack.pop_back();
}

void ModifiesExtractor::extractStatementModifies(const Ent::Procedure p, const unique_ptr<CallStatementNode> &call) {
    Ent::Procedure called{ call->getProcName()};
    CalledProcedure calledProcedure{ called, call->getStatementNo() };
    
    DesignExtractorUtil::insertIntoProcedureMap(p, calledProcedure, procedureCallMap);

    for (const auto& stmt : statementStack) {
        DesignExtractorUtil::insertIntoStatementMap(stmt, calledProcedure, statementCallMap);
    }
}

void ModifiesExtractor::extractIndirectProcedureModifies() {
    auto procedureCallPairs = DesignExtractorUtil::convertMapToPairs(procedureCallMap);

    for (const auto& [caller, calledProcedure] : procedureCallPairs) {
        stmtNo_t calledProcedureStmtNo = calledProcedure.getStmtNo();
        for (const auto& rel : procedureModifiesRelationships) {
            if (*rel.getArg1() != calledProcedure.getProcedure()) continue;

            auto p = make_shared<Ent::Procedure>(caller);
            auto var = dynamic_pointer_cast<Ent::Variable>(rel.getArg2());
            Rel::Modifies procedureModifies{ p, var };
            modifiesRelationships.push_back(move(procedureModifies));

            auto callStatement = make_shared<Ent::CallStatement>(calledProcedureStmtNo, calledProcedure.getProcedure().getName());
            Rel::Modifies callModifies{ callStatement, var };
            modifiesRelationships.push_back(move(callModifies));
        }
    }
}

void ModifiesExtractor::extractIndirectStatementModifies() {
    auto statementCallPairs = DesignExtractorUtil::convertMapToPairs(statementCallMap);

    for (const auto& [statement, calledProcedure] : statementCallPairs) {
        for (const auto& rel : procedureModifiesRelationships) {
            if (*rel.getArg1() != calledProcedure.getProcedure()) continue;

            auto stmt = make_shared<Ent::Statement>(statement);
            auto var = dynamic_pointer_cast<Ent::Variable>(rel.getArg2());
            Rel::Modifies stmtModifies{ stmt, var };
            modifiesRelationships.push_back(move(stmtModifies));
        }
    }
}

void ModifiesExtractor::addNestedModifiesToVector(const Ent::Procedure p, const Ent::Variable var) {
    for (const auto& statement : statementStack) {
        Rel::Modifies nestedModifies{ statement, var };
        modifiesRelationships.push_back(move(nestedModifies));
    }

    Rel::Modifies procedureModifies{ p, var };
    modifiesRelationships.push_back(procedureModifies);
    procedureModifiesRelationships.push_back(procedureModifies);
}

ModifiesExtractor::ModifiesExtractor() {}

vector<Rel::Modifies> ModifiesExtractor::extractModifies(const unique_ptr<ProgramNode> &AST) {
	for (const auto& procedure : AST->getProcedures()) {
        Ent::Procedure p{ procedure->getName()};
        extractStatementListModifies(p, procedure->getStatements());
    }
    
    DesignExtractorUtil::generateAllStatementCalls(procedureCallMap, statementCallMap);
    DesignExtractorUtil::generateAllProcedureCalls(procedureCallMap);

    extractIndirectProcedureModifies();
    extractIndirectStatementModifies();

    return modifiesRelationships;
}