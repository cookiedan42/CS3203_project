#include "StatementParser.h"

StatementParser::StatementParser(TokenIterator head, int statementNo) :
    head(head), statementNo(statementNo) {}

const TokenIterator& StatementParser::getIterator() const {
    return head;
}

const int& StatementParser::getStatementNo() const {
    return statementNo;
}

vector<StatementNode> StatementParser::parseStatementList() {
    vector<StatementNode> statements{};

    while (!head.valueIs(SIMPLE::RIGHT_CURLY)) {
        auto statement = parseStatement();

        statements.push_back(move(statement));
    }

    if (statements.empty()) throw ParseException(Exception::EMPTY_STATEMENT_LIST);

    return statements;
}

StatementNode StatementParser::parseStatement() {
    if (head.valueIs(SIMPLE::EQUALS, 1)) {
        auto assignStatement = parseAssignStatement();
        return assignStatement;
    }

    if (head.valueIs(SIMPLE::READ)) {
        auto readStatement = parseReadStatement();
        return readStatement;
    }

    if (head.valueIs(SIMPLE::PRINT)) {
        auto printStatement = parsePrintStatement();
        return printStatement;
    }

    if (head.valueIs(SIMPLE::CALL)) {
        auto callStatement = parseCallStatement();
        return callStatement;
    }

    if (head.valueIs(SIMPLE::WHILE)) {
        auto whileStatement = parseWhileStatement();
        return whileStatement;
    }

    if (head.valueIs(SIMPLE::IF)) {
        auto ifStatement = parseIfStatement();
        return ifStatement;
    }

    // Shouldn't reach here if Validator is working properly
    throw ParseException(Exception::INVALID_SYNTAX_GRAMMAR);
}

unique_ptr<AssignStatementNode> StatementParser::parseAssignStatement() {
    auto variable = TerminalParser::parseVariableName(head);

    ParserUtil::expect(head, SIMPLE::EQUALS);

    TokenIterator start = head;
    head.advanceTo(SIMPLE::EOL);
    TokenIterator end = head;

    ExpressionParser expressionParser = ExpressionParser(start, end);
    ExpressionNode expressionNode = expressionParser.parse();

    ParserUtil::expect(head, SIMPLE::EOL);

    statementNo++;

    return make_unique<AssignStatementNode>(statementNo,
        move(variable), move(expressionNode));
}

unique_ptr<ReadStatementNode> StatementParser::parseReadStatement() {
    ParserUtil::expect(head, SIMPLE::READ);

    auto variable = TerminalParser::parseVariableName(head);

    ParserUtil::expect(head, SIMPLE::EOL);

    statementNo++;

    return make_unique<ReadStatementNode>(statementNo, move(variable));
}

unique_ptr<PrintStatementNode> StatementParser::parsePrintStatement() {
    ParserUtil::expect(head, SIMPLE::PRINT);

    auto variable = TerminalParser::parseVariableName(head);

    ParserUtil::expect(head, SIMPLE::EOL);

    statementNo++;

    return make_unique<PrintStatementNode>(statementNo, move(variable));
}

unique_ptr<CallStatementNode> StatementParser::parseCallStatement() {
    ParserUtil::expect(head, SIMPLE::CALL);

    string procName = head.getValue();
    head.advance();

    ParserUtil::expect(head, SIMPLE::EOL);

    statementNo++;

    return make_unique<CallStatementNode>(statementNo, procName);
}

unique_ptr<WhileStatementNode> StatementParser::parseWhileStatement() {
    ParserUtil::expect(head, SIMPLE::WHILE);

    statementNo++;
    int whileStatementNo = this->statementNo;

    ParserUtil::expect(head, SIMPLE::LEFT_BRACKET);

    auto conditionalExpressionParser = ConditionalExpressionParser(head);
    auto conditionalExpression = conditionalExpressionParser.parseConditionalExpression();
    head = conditionalExpressionParser.getIterator(); // Point to end of cond expr

    ParserUtil::expect(head, SIMPLE::RIGHT_BRACKET);
    ParserUtil::expect(head, SIMPLE::LEFT_CURLY);

    auto statements = parseStatementList();

    ParserUtil::expect(head, SIMPLE::RIGHT_CURLY);

    return make_unique<WhileStatementNode>(whileStatementNo,
        move(conditionalExpression), move(statements));
}

unique_ptr<IfStatementNode> StatementParser::parseIfStatement() {
    ParserUtil::expect(head, SIMPLE::IF);

    statementNo++;
    int ifStatementNo = this->statementNo;

    ParserUtil::expect(head, SIMPLE::LEFT_BRACKET);

    auto conditionalExpressionParser = ConditionalExpressionParser(head);
    auto conditionalExpression = conditionalExpressionParser.parseConditionalExpression();
    head = conditionalExpressionParser.getIterator(); // Point to end of cond expr

    ParserUtil::expect(head, SIMPLE::RIGHT_BRACKET);
    ParserUtil::expect(head, SIMPLE::THEN);
    ParserUtil::expect(head, SIMPLE::LEFT_CURLY);

    auto thenStatements = parseStatementList();

    ParserUtil::expect(head, SIMPLE::RIGHT_CURLY);
    ParserUtil::expect(head, SIMPLE::ELSE);
    ParserUtil::expect(head, SIMPLE::LEFT_CURLY);

    auto elseStatements = parseStatementList();

    ParserUtil::expect(head, SIMPLE::RIGHT_CURLY);

    return make_unique<IfStatementNode>(ifStatementNo,
        move(conditionalExpression), move(thenStatements), move(elseStatements));
}
