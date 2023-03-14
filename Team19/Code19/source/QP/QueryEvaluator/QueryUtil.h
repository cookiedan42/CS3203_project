#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include <optional>
#include "../../TypeAlias/TypeAlias.h"
#include "../QueryObject/Argument.h"
#include "../QueryObject/Declaration.h"
#include "../QueryParser/AttrRef.h"
#include "../QueryExceptions/QuerySyntaxException.h"
#include "../Table/Table.h"
#include "../../SourceProcessor/Validator/ValidatorUtil.h"
#include "../../SourceProcessor/Validator/ExpValidator.h"
#include "../../SourceProcessor/Tokenizer/TokenIterator.h"
#include "../../SourceProcessor/Tokenizer/SPTokenizer.h"

using namespace std;

class QueryUtil {
public:
	
	// Methods used by query evaluators
	static ArgTypePairToArgsTypeMap argTypePairToArgsTypeMap;
	static ArgType getArgTypeFromArgument(Argument arg);
	static ArgsType getArgumentsTypesFromMap(Argument arg1, Argument arg2);
	static ArgString removeQuotes(ArgString s);
	static ArgString removeSubQuotes(ArgString s);
	static Table joinTable(Table& table1, Table& table2);
	static Table createTableOnEqualValues(ArgString arg1, ArgString arg2, SynonymResults values1, SynonymResults values2);
	static Table createFilteredTableWithValue(ArgString arg, SynonymResults values, string filterValue);
	static Table createTableArg1(ArgString arg1, PKBResults result);
	static Table createTableArg2(ArgString arg2, PKBResults result);
	static Table createTableArg1Arg2(ArgString arg1, ArgString arg2, PKBResults result);
	static Table createTable(ArgString arg, SynonymResults items);
	static Table createTableArg1ForStatementType(ArgString arg1, RelationshipResults result, SynonymResults statementResults);
	static Table createTableArg2ForStatementType(ArgString arg2, RelationshipResults result, SynonymResults statementResults);
	static Table createTableArg1Arg2ForStatementType(ArgString arg1, ArgString arg2, RelationshipResults result, 
		SynonymResults arg1Results, SynonymResults arg2Results, bool isStmtTypeArg1, bool isStmtTypeArg2);
	static bool isSameSynonym(Argument arg1, Argument arg2);
	static bool isStatementType(Declaration dec);
	static RelationshipResults filterEqualPairs(RelationshipResults pairs);
	static std::list<std::string> set_to_list(std::unordered_set<std::string> set);
		

	// Methods used by query parser
	static ArgType getArgType(ArgString arg);
	static ArgString removeAllWhitespaces(ArgString arg);
	static optional<Declaration> getFromDeclarations(string varname, std::vector<Declaration> declarations);
	static ArgString stringFromTokens(std::vector<Token> tokens);
	static ArgType getArgTypeForParser(ArgString arg, std::vector<Declaration> declarations);
	static Argument getArgFromStr(ArgString arg, std::vector<Declaration> declarations);

	// QueryParser uses to run grammar checks
	static bool isValidEntRef(string entStr, std::vector<Declaration> declarations);
	static bool isValidStmtRef(string stmtStr, std::vector<Declaration> declarations);
	static bool isValidWithRef(string refStr, std::vector<Declaration> declarations);
	static bool isValidAttrRef(string attrRefStr, std::vector<Declaration> declarations);
	static bool isValidExprSpec(string exprStr);
	static bool isValidExp(string exprStr);
	static bool isName(string name);
	static bool isInteger(ArgString& s);
	static bool isQuote(ArgString& s);
	static bool isSubQuote(ArgString& s);
	static bool isWildcard(ArgString& s);

	// QueryParser uses for handling Attributes
	static AttrRef getAttrRefFromStr(std::string attrrefStr, std::vector<Declaration> declarations);
	static bool isValidSemanticAttrRef(AttrRef attrRef);
};
