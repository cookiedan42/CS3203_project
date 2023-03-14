#pragma once
#include <string>
#include <memory>
#include "../AST/AST.h"
#include "../Tokenizer/TokenIterator.h"

using namespace std;
using namespace SP;

class TerminalParser {
public:
	static unique_ptr<VariableNode> parseVariableName(TokenIterator& head);

	static unique_ptr<ConstantNode> parseConstantValue(TokenIterator& head);
};