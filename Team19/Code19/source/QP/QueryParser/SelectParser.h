#pragma once
#include <vector>
#include <sstream>
#include "../../SourceProcessor/Tokenizer/TokenIterator.h"
#include "../../SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../QueryObject/Declaration.h"
#include "../QueryObject/SelectObject.h"
#include "../QueryExceptions/QuerySemanticException.h"
#include "../QueryExceptions/QuerySyntaxException.h"
#include "../QueryEvaluator/QueryUtil.h"
#include "../../Definitions/Definitions.h"

struct SelectParser {
public:
	static SelectObject parse(std::vector<Token> selectArg, std::vector<Declaration> declarations);
private:
	static std::vector<SynAttr> parseTuple(std::vector<Token> tupleArgs, std::vector<Declaration> declarations);
	static SynAttr parseDecOrSynAttr(std::string arg, std::vector<Declaration> declarations);
	static SynAttr getSynAttrFromAttrRef(AttrRef attrRef);
	static std::vector<string> getListFromStr(std::string arg, char delim);
};