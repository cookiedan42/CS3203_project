#include "ParserUtil.h"

bool ParserUtil::isEndOfFile(TokenIterator& head, TokenIterator& end) {
	return head == end;
}

void ParserUtil::expect(TokenIterator& head, string str) {
    if (!head.valueIs(str)) throw ParseException(Exception::INVALID_SYNTAX_GRAMMAR);
    head.advance();
}

TokenIterator ParserUtil::findRelationalFactorEndIndex(TokenIterator& start) {
    TokenIterator end = start;
    int openBracketCounter = 0;

    // To find the close bracket that indicate end of relational factor
    while (!end.valueIs(SIMPLE::RIGHT_BRACKET) || openBracketCounter != 0) {
        if (end.valueIs(SIMPLE::LEFT_BRACKET)) openBracketCounter++;

        if (end.valueIs(SIMPLE::RIGHT_BRACKET)) openBracketCounter--;

        end.advance();
    }

    return end;
}
