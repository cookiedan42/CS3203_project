#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "../../SourceProcessor/Tokenizer/TokenIterator.h"
#include "../../SourceProcessor/Tokenizer/Token.h"
#include "../QuerySplitter/QuerySplitter.h"
#include "../QueryObject/Declaration.h"
#include "../QueryObject/DesignEntity.h"
#include "../QueryObject/DesignEntityFactory.h"
#include "../QueryExceptions/QuerySyntaxException.h"

struct DeclarationsParser {
public:
	static std::vector<Declaration> parse(std::vector<QuerySplitter::dec_pair> declarations);
private:
	static shared_ptr<DesignEntity> validateSynType(std::string synTypeStr);
};