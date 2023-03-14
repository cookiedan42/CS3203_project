#include "Parser.h"

unique_ptr<ProgramNode> Parser::parseProgram() {
    vector<unique_ptr<ProcedureNode>> procedures {};

    while (!ParserUtil::isEndOfFile(head, end)) {

        auto procedure = parseProcedure();

        procedures.push_back(move(procedure));
    }

    if (procedures.empty()) throw ParseException(Exception::EMPTY_SOURCE);

    return make_unique<ProgramNode>(move(procedures));
}

unique_ptr<ProcedureNode> Parser::parseProcedure() {
    ParserUtil::expect(head, SIMPLE::PROCEDURE);

    string procName = head.getValue();
    head.advance();

    ParserUtil::expect(head, SIMPLE::LEFT_CURLY);

    auto statementParser = StatementParser(head, statementNo);
    auto statements = statementParser.parseStatementList();
    head = statementParser.getIterator();
    statementNo = statementParser.getStatementNo();

    ParserUtil::expect(head, SIMPLE::RIGHT_CURLY);

    return make_unique<ProcedureNode>(procName, move(statements));
}

Parser::Parser(vector<Token> tokens) : tokens(tokens), statementNo(0) {}

unique_ptr<ProgramNode> Parser::parseTokens() {
    try {
        auto AST = parseProgram();
        return AST;
    } catch (ParseException e) {
        return nullptr;
    }
}

