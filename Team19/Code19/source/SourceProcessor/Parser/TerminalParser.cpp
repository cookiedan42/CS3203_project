#include "TerminalParser.h"

unique_ptr<VariableNode> TerminalParser::parseVariableName(TokenIterator& head) {
    string name = head.getValue();
    head.advance();

    return make_unique<VariableNode>(name);
}

unique_ptr<ConstantNode> TerminalParser::parseConstantValue(TokenIterator& head) {
    string value = head.getValue();
    head.advance();

    return make_unique<ConstantNode>(value);
}
