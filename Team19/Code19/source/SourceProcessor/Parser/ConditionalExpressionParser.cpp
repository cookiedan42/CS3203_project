#include "ConditionalExpressionParser.h"

ConditionalExpressionParser::ConditionalExpressionParser(TokenIterator head) : head(head) {}

const TokenIterator& ConditionalExpressionParser::getIterator() const {
    return head;
}

unique_ptr<ConditionalExpressionNode> ConditionalExpressionParser::parseConditionalExpression() {
    TokenIterator current = head;
    try { // RelationalExpression
        auto relationalExpression = parseRelationalExpression();
        return relationalExpression;
    }
    catch (ParseException e) {
        head = current;
    } // reset head and continue

    if (head.valueIs(SIMPLE::NOT)) { // NOT operator
        auto notCondition = parseNotConditionalExpression();
        return notCondition;
    }

    // AND or OR operator
    auto andOrCondition = parseAndOrConditionalExpression();
    return andOrCondition;
}

unique_ptr<NotConditionalExpressionNode> ConditionalExpressionParser::parseNotConditionalExpression() {
    ParserUtil::expect(head, SIMPLE::NOT);
    ParserUtil::expect(head, SIMPLE::LEFT_BRACKET);

    auto conditionalExpression = parseConditionalExpression();

    ParserUtil::expect(head, SIMPLE::RIGHT_BRACKET);

    return make_unique<NotConditionalExpressionNode>(move(conditionalExpression));
}

unique_ptr<ConditionalExpressionNode> ConditionalExpressionParser::parseAndOrConditionalExpression() {
    ParserUtil::expect(head, SIMPLE::LEFT_BRACKET);

    auto LHS = parseConditionalExpression();

    ParserUtil::expect(head, SIMPLE::RIGHT_BRACKET);

    string op = head.getValue();
    head.advance();

    ParserUtil::expect(head, SIMPLE::LEFT_BRACKET);

    auto RHS = parseConditionalExpression();

    ParserUtil::expect(head, SIMPLE::RIGHT_BRACKET);

    if (op == SIMPLE::AND) {
        return make_unique<AndConditionalExpressionNode>(move(LHS), move(RHS));
    }
    else {
        return make_unique<OrConditionalExpressionNode>(move(LHS), move(RHS));
    }
}

unique_ptr<RelationalExpressionNode> ConditionalExpressionParser::parseRelationalExpression() {
    TokenIterator startLHS = head;
    while (!head.isComparator()) head.advance();
    TokenIterator endLHS = head;

    auto LHS = parseRelationalFactor(startLHS, endLHS);

    string op = head.getValue();
    head.advance();

    TokenIterator startRHS = head;
    TokenIterator endRHS = ParserUtil::findRelationalFactorEndIndex(startRHS);

    auto RHS = parseRelationalFactor(startRHS, endRHS);

    head = endRHS;
    return make_unique<RelationalExpressionNode>(move(LHS), op, move(RHS));
}

RelationalFactorNode ConditionalExpressionParser::parseRelationalFactor(TokenIterator start, TokenIterator end) {
    ExpressionParser expressionParser = ExpressionParser(start, end);
    RelationalFactorNode relationalFactorNode = expressionParser.parse();

    return relationalFactorNode;
}
