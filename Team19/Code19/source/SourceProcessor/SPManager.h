#pragma once

#include "DesignExtractor/DesignExtractor.h"
#include "Parser/Parser.h"
#include "Tokenizer/Token.h"
#include "Tokenizer/SPTokenizer.h"
#include "Validator/Validator.h"

#include "../PKB/InsertFacade.h"

struct SPManager {	
	SPManager static construct(std::string source);
	void populatePKB(InsertFacade& facade);

	DesignExtractor de; // exposed for unit testing
private:
	SPManager() = delete;
	SPManager(DesignExtractor& de) :
		de(move(de)) { (this->de).extractAST(); };
};