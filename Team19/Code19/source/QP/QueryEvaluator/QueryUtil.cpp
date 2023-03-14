#include "QueryUtil.h"
#include <algorithm>
#include <iterator>
#include <utility>

const string WILDCARD_KEYWORD = "_";
const char STR_DELIM = '"';
const char ATTR_REF_DELIM = '.';
const std::vector<string> WITH_ATTRNAME = { "procName", "varName", "value", "stmt#" };

RelationshipResults QueryUtil::filterEqualPairs(RelationshipResults pairs) 
{
	RelationshipResults filteredPairs;
	for (auto& [value1, value2] : pairs)
	{
		if (value1 == value2) {
			filteredPairs.insert({ value1, value2 });
		}
	}
	return filteredPairs;
}

ArgTypePairToArgsTypeMap QueryUtil::argTypePairToArgsTypeMap = {
		{ { ArgType::EMPTY, ArgType::EMPTY }, ArgsType::EMPTY },
		{ { ArgType::SYN, ArgType::SYN }, ArgsType::SYN_SYN },
		{ { ArgType::SYN, ArgType::WILDCARD }, ArgsType::SYN_WILDCARD },
		{ { ArgType::SYN, ArgType::STMTNO }, ArgsType::SYN_STMTNO },
		{ { ArgType::SYN, ArgType::QUOTE }, ArgsType::SYN_QUOTE },
		{ { ArgType::SYN, ArgType::SUBQUOTE }, ArgsType::SYN_SUBQUOTE },
		{ { ArgType::WILDCARD, ArgType::SYN }, ArgsType::WILDCARD_SYN },
		{ { ArgType::WILDCARD, ArgType::WILDCARD }, ArgsType::WILDCARD_WILDCARD },
		{ { ArgType::WILDCARD, ArgType::STMTNO }, ArgsType::WILDCARD_STMTNO },
		{ { ArgType::WILDCARD, ArgType::QUOTE }, ArgsType::WILDCARD_QUOTE },
		{ { ArgType::WILDCARD, ArgType::SUBQUOTE }, ArgsType::WILDCARD_SUBQUOTE },
		{ { ArgType::STMTNO, ArgType::SYN }, ArgsType::STMTNO_SYN },
		{ { ArgType::STMTNO, ArgType::WILDCARD }, ArgsType::STMTNO_WILDCARD },
		{ { ArgType::STMTNO, ArgType::STMTNO }, ArgsType::STMTNO_STMTNO },
		{ { ArgType::STMTNO, ArgType::QUOTE }, ArgsType::STMTNO_QUOTE },
		{ { ArgType::QUOTE, ArgType::SYN }, ArgsType::QUOTE_SYN },
		{ { ArgType::QUOTE, ArgType::WILDCARD }, ArgsType::QUOTE_WILDCARD },
		{ { ArgType::QUOTE, ArgType::QUOTE }, ArgsType::QUOTE_QUOTE },
		{ { ArgType::QUOTE, ArgType::SUBQUOTE }, ArgsType::QUOTE_SUBQUOTE },
		{ { ArgType::INTEGER, ArgType::INTEGER }, ArgsType::INTEGER_INTEGER },
		{ { ArgType::INTEGER, ArgType::ATTR_REF_INT }, ArgsType::INTEGER_REFINT },
		{ { ArgType::ATTR_REF_INT, ArgType::INTEGER }, ArgsType::REFINT_INTEGER },
		{ { ArgType::ATTR_REF_INT, ArgType::ATTR_REF_INT }, ArgsType::REFINT_REFINT },
		{ { ArgType::QUOTE, ArgType::ATTR_REF_NAME }, ArgsType::QUOTE_REFNAME },
		{ { ArgType::ATTR_REF_NAME, ArgType::QUOTE }, ArgsType::REFNAME_QUOTE },
		{ { ArgType::ATTR_REF_NAME, ArgType::ATTR_REF_NAME }, ArgsType::REFNAME_REFNAME },
};

ArgType QueryUtil::getArgTypeFromArgument(Argument arg)
{
	if ((std::holds_alternative<Declaration>(arg.argumentVariant))) {
		return ArgType::SYN;
	}
	else if ((std::holds_alternative<StmtNo>(arg.argumentVariant))) {
		return ArgType::STMTNO;
	} 
	else if ((std::holds_alternative<StringExp>(arg.argumentVariant))) {
		return ArgType::QUOTE;
	}
	else if ((std::holds_alternative<SubExp>(arg.argumentVariant))) {
		return ArgType::SUBQUOTE;
	}
	else if ((std::holds_alternative<WildCard>(arg.argumentVariant))) {
		return ArgType::WILDCARD;
	}
	else if ((std::holds_alternative<SynAttr>(arg.argumentVariant))) {
		return std::get<SynAttr>(arg.argumentVariant).attributeType;
	}
	else if ((std::holds_alternative<Integer>(arg.argumentVariant))) {
		return ArgType::INTEGER;
	}
	else {
		return ArgType::EMPTY;
	}
}

ArgsType QueryUtil::getArgumentsTypesFromMap(Argument arg1, Argument arg2) 
{
	ArgType arg1Type = getArgTypeFromArgument(arg1);
	ArgType arg2Type = getArgTypeFromArgument(arg2);

	if (arg1Type == ArgType::EMPTY || arg2Type == ArgType::EMPTY) {
		throw invalid_argument("Arguments are invalid.");
	}
	else {
		auto argTypePair = std::make_pair(arg1Type, arg2Type);
		auto& argsType = argTypePairToArgsTypeMap[argTypePair];

		return argsType;
	}

}

/* Checks if the design entity of the declaration is a type of statement. */
bool QueryUtil::isStatementType(Declaration dec) 
{
	string type = dec.synType->type;
	return type == DE::Assign().type || type == DE::Call().type || type == DE::If().type ||
		type == DE::Print().type || type == DE::Read().type || type == DE::While().type;
}

/* Checks if the declarations contain the same synonym. */
bool QueryUtil::isSameSynonym(Argument arg1, Argument arg2)
{
	Declaration dec1 = std::get<Declaration>(arg1.argumentVariant);
	Declaration dec2 = std::get<Declaration>(arg2.argumentVariant);
	return dec1 == dec2;
}

/**
* Remove all whitespaces in string input.
*/
ArgString QueryUtil::removeAllWhitespaces(ArgString arg)
{
	arg.erase(remove_if(arg.begin(), arg.end(), isspace), arg.end());
	return arg;
}

/**
* Remove quotation marks from quote argument.
*/
ArgString QueryUtil::removeQuotes(ArgString s) {
	ArgString result = s;
	if (s.size() >= 2) {
		result = s.substr(1, s.size() - 2);
	}
	return result;
}

/**
* Remove quotation marks from sub-expression quote argument.
*/
ArgString QueryUtil::removeSubQuotes(ArgString s) {
	ArgString result = s;
	if (s.size() >= 4) {
		result = s.substr(2, s.size() - 4);
	}
	return result;
}

/**
* Convert set of strings to list of strings.
*/
std::list<std::string> QueryUtil::set_to_list(std::unordered_set<std::string> set) {
	list<string> res;
	list<string>::iterator it = res.begin();
	for (string const& k : set) {
		res.insert(it, k);
	}
	return res;
}

/**
* Check if input argument is an INTEGER: 0 | NZDIGIT ( DIGIT )*
*/
bool QueryUtil::isInteger(ArgString& s) {
	return ValidatorUtil::isInteger(s);
}

/**
* Check if input argument is a quote.
*/
bool QueryUtil::isQuote(ArgString& s) {
	// check "x"
	return s[0] == '\"' && s[s.size() - 1] == '\"';
	
}

/**
* Check if input argument is a sub-expression quote.
*/
bool QueryUtil::isSubQuote(ArgString& s) {
	// check _"x"_
	return s[0] == '_' && s[1] == '\"' && s[s.size() - 2] == '\"' && s[s.size() - 1] == '_';
}

/**
* Check if input argument is a wildcard expression
*/
bool QueryUtil::isWildcard(ArgString& s) {
	// check if _
	return s[0] == '_' && s.size() == 1;
}

/**
* Check if input argument is a valid name.
*/
bool QueryUtil::isName(string name) {
	return ValidatorUtil::isName(name);
}

/**
* Get the type of input argument.
*/
ArgType QueryUtil::getArgType(ArgString arg)
{
	ArgType arg_type;
	if (arg == "_") {
		arg_type = ArgType::WILDCARD;
	}
	else if (isInteger(arg)) {
		arg_type = ArgType::STMTNO;
	}
	else if (isQuote(arg)) {
		arg_type = ArgType::QUOTE;
	}
	else if (isSubQuote(arg)) {
		arg_type = ArgType::SUBQUOTE;
	}
	else {
		arg_type = ArgType::SYN;
	}
	return arg_type;
}

/**
* Get the type of input argument.
*/
ArgType QueryUtil::getArgTypeForParser(ArgString arg, std::vector<Declaration> declarations)
{
	ArgType arg_type = getArgType(arg);
	if (arg_type == ArgType::SYN) {
		optional<Declaration> optDec = getFromDeclarations(arg, declarations);
		if (optDec.has_value()) { return ArgType::SYN; }
		return ArgType::EMPTY;
	}
	return arg_type;
}


/**
* Get the Argument developed from the string
*/
Argument QueryUtil::getArgFromStr(ArgString arg, std::vector<Declaration> declarations)
{
	struct Argument result;
	ArgType arg_type = getArgTypeForParser(arg, declarations);
	if (arg_type == ArgType::WILDCARD) {
		result = { WildCard() };
	} 
	else if (arg_type == ArgType::STMTNO) {
		result = { StmtNo(arg) };
	}
	else if (arg_type == ArgType::QUOTE) {
		arg = removeQuotes(arg);
		result = { StringExp(arg) };
	}
	else if (arg_type == ArgType::SUBQUOTE) {
		arg = removeSubQuotes(arg);
		result = { SubExp(arg) };
	}
	else if (arg_type == ArgType::SYN) {
		result = { getFromDeclarations(arg, declarations).value() };
	}
	return result;
}


/**
* Join tables of synonyms together, taking intersection of common synonyms.
*/
Table QueryUtil::joinTable(Table& table1, Table& table2) {
	std::vector<std::string> header1 = table1.getTableHeader();
	std::vector<std::string> header2 = table2.getTableHeader();
	
	std::vector<std::string> newHeader(header1);
	std::copy(header2.begin(), header2.end(), std::back_inserter(newHeader));

	std::vector<std::vector<std::string>> newItems;

	//Cross product to form big table
	for (auto& i : table1.getTableRows()) {
		for (auto& j : table2.getTableRows()) {
			std::vector<std::string> newItem(i);
			std::copy(j.begin(), j.end(), std::back_inserter(newItem));
			newItems.push_back(newItem);
		}
	}

	//Initialise big table with new header and items
	Table filteredTable(newHeader, newItems);

	std::pair<int, int> commonKeyIndices = filteredTable.getFirstCommonKeys();

	while (commonKeyIndices != std::pair<int,int>()) {
		filteredTable = filteredTable.filterTable(commonKeyIndices.first, commonKeyIndices.second);

		//get next common key indices
		commonKeyIndices = filteredTable.getFirstCommonKeys();
	}
	return filteredTable;
}

/**
* Find intersection of two sets, and creates table based on intersection
*/
Table QueryUtil::createTableOnEqualValues(ArgString arg1, ArgString arg2,
	SynonymResults values1, SynonymResults values2)
{
	Table table;

	if (arg1 == arg2) 
	{
		table = createTable(arg1, values1);
	}
	else 
	{
		vector<string> headers;
		headers.push_back(arg1);
		headers.push_back(arg2);

		vector<vector<string>> items;
		// get the intersection of the two sets
		for (string const& value : values2) 
		{
			if (values1.find(value) != values1.end()) 
			{
				vector<string> item;
				// add item for both args
				item.push_back(value);
				item.push_back(value);
				// add row into table
				items.push_back(item);
			};
		}

		table = Table(headers, items);
	}

	return table;

}

Table QueryUtil::createFilteredTableWithValue(ArgString arg, SynonymResults values, string filterValue)
{
	Table table;
	vector<string> headers;
	headers.push_back(arg);

	vector<vector<string>> items;
	if (values.find(filterValue) != values.end()) 
	{
		vector<string> item;
		item.push_back(filterValue);
		items.push_back(item);
	}

	table = Table(headers, items);
	return table;
}

/**
* Create single column table of synonym values of arg1.
*/
Table QueryUtil::createTableArg1(ArgString arg1, PKBResults result)
{
	vector<string> headers;
	headers.push_back(arg1);
	vector<vector<string>> items;
	for (auto& [res1, res2] : result) {
		vector<std::string> item;
		item.push_back(res1);
		items.push_back(item);
	}
	return Table(headers, items);
}

/**
* Create single column table of synonym values of arg1, with intersection on statement type synonym results.
*/
Table QueryUtil::createTableArg1ForStatementType(ArgString arg1, RelationshipResults result, SynonymResults statementResults)
{
	vector<string> headers;
	headers.push_back(arg1);
	vector<vector<string>> items;
	for (auto& [res1, res2] : result) {
		SynonymResults::iterator it = std::find(statementResults.begin(), statementResults.end(), res1);
		if (it != statementResults.end()) {
			vector<std::string> item;
			item.push_back(res1);
			items.push_back(item);
		}	
	}
	return Table(headers, items);
}


/**
* Create single column table of synonym values of arg2.
*/
Table QueryUtil::createTableArg2(ArgString arg2, PKBResults result)
{
	vector<string> headers;
	headers.push_back(arg2);
	vector<vector<string>> items;
	for (auto& [res1, res2] : result) {
		vector<std::string> item;
		item.push_back(res2);
		items.push_back(item);
	}
	return Table(headers, items);
}

/**
* Create single column table of synonym values of arg2, with intersection on statement type synonym results.
*/
Table QueryUtil::createTableArg2ForStatementType(ArgString arg2, RelationshipResults result, SynonymResults statementResults)
{
	vector<string> headers;
	headers.push_back(arg2);
	vector<vector<string>> items;
	for (auto& [res1, res2] : result) {
		SynonymResults::iterator it = std::find(statementResults.begin(), statementResults.end(), res2);
		if (it != statementResults.end()) {
			vector<std::string> item;
			item.push_back(res2);
			items.push_back(item);
		}
	}
	return Table(headers, items);
}

/**
* Create two column table of synonym values of arg1 and arg2.
*/
Table QueryUtil::createTableArg1Arg2(ArgString arg1, ArgString arg2, PKBResults result)
{
	vector<string> headers;
	headers.push_back(arg1);
	headers.push_back(arg2);
	
	vector<vector<string>> items;
	for (auto& [res1, res2] : result) {
		vector<std::string> item;
		item.push_back(res1);
		item.push_back(res2);
		items.push_back(item);
	}
	return Table(headers, items);
}

/**
* Create two column table of synonym values of arg1 and arg2, with intersection on statement type synonym results.
*/
Table QueryUtil::createTableArg1Arg2ForStatementType(ArgString arg1, ArgString arg2, RelationshipResults result, 
	SynonymResults arg1Results, SynonymResults arg2Results, bool isStmtTypeArg1, bool isStmtTypeArg2)
{
	vector<string> headers;
	headers.push_back(arg1);
	headers.push_back(arg2);
	vector<vector<string>> items;
	for (auto& [res1, res2] : result) {
		if (isStmtTypeArg1 && isStmtTypeArg2) {
			SynonymResults::iterator it1 = std::find(arg1Results.begin(), arg1Results.end(), res1);
			SynonymResults::iterator it2 = std::find(arg2Results.begin(), arg2Results.end(), res2);
			if (it1 != arg1Results.end() && it2 != arg2Results.end()) {
				vector<std::string> item;
				item.push_back(res1);
				item.push_back(res2);
				items.push_back(item);
			}
		}
		else if (isStmtTypeArg1 && !isStmtTypeArg2) {
			SynonymResults::iterator it1 = std::find(arg1Results.begin(), arg1Results.end(), res1);
			if (it1 != arg1Results.end()) {
				vector<std::string> item;
				item.push_back(res1);
				item.push_back(res2);
				items.push_back(item);
			}
		}
		else if (!isStmtTypeArg1 && isStmtTypeArg2) {
			SynonymResults::iterator it2 = std::find(arg2Results.begin(), arg2Results.end(), res2);
			if (it2 != arg2Results.end()) {
				vector<std::string> item;
				item.push_back(res1);
				item.push_back(res2);
				items.push_back(item);
			}
		}
		
	}
	return Table(headers, items);
}

/**
* Create one column table of synonym values of arg.
* 
* @param arg Column header for synonym values
* @param values Synonym values
* 
* @return Table of column header and values
*/
Table QueryUtil::createTable(ArgString arg, SynonymResults values)
{
	vector<string> headers;
	headers.push_back(arg);

	vector<vector<string>> items;
	for (auto& value : values) {
		vector<std::string> item;
		item.push_back(value);
		items.push_back(item);
	}

	return Table(headers, items);
}

/**
* Returns declaration from varName
*/
optional<Declaration> QueryUtil::getFromDeclarations(string varName, std::vector<Declaration> declarations) {
	std::vector<Declaration>::iterator decIt;
	for (decIt = declarations.begin(); decIt != declarations.end(); decIt++) {
		if (decIt->synName == varName) {
			return *decIt;
		}
	}
	return std::nullopt;
}

/**
* Returns true if str is a valid entity reference: synonym | '_' | '"' IDENT '"'
*/
bool QueryUtil::isValidEntRef(string entStr, std::vector<Declaration> declarations) {
	if (entStr == WILDCARD_KEYWORD) {
		return true;
	}
	if (entStr.at(0) == STR_DELIM && entStr.at(entStr.length() - 1) == STR_DELIM) {
		string ident = entStr.substr(1, entStr.length() - 2);
		if (ValidatorUtil::verifyName(ident)) {
			return true;
		}
		return false;
	}
	optional<Declaration> syn = getFromDeclarations(entStr, declarations);
	if (!syn) { return false; }
	return true;
}

/**
* Returns true if str is a valid entity reference: synonym | '_' | INTEGER
*/
bool QueryUtil::isValidStmtRef(string stmtStr, std::vector<Declaration> declarations) {
	if (stmtStr == WILDCARD_KEYWORD) {
		return true;
	}
	if (ValidatorUtil::verifyInt(stmtStr)) {
		return true;
	}
	optional<Declaration> syn = getFromDeclarations(stmtStr, declarations);
	if (!syn) { return false; }
	return true;
}

/**
* Returns true if str is a valid ref (of with clause): '"' IDENT '"' | INTEGER | attrRef
*/
bool QueryUtil::isValidWithRef(string refStr, std::vector<Declaration> declarations) {
	if (ValidatorUtil::verifyInt(refStr)) {
		return true;
	}
	if (refStr.at(0) == STR_DELIM && refStr.at(refStr.length() - 1) == STR_DELIM) {
		string ident = refStr.substr(1, refStr.length() - 2);
		if (ValidatorUtil::verifyName(ident)) {
			return true;
		}
		return false;
	}
	if (isValidAttrRef(refStr, declarations)) {
		return true;
	}
	return false;
}

/**
* Returns true if str is a valid attrRef: synonym '.' attrName
*/
bool QueryUtil::isValidAttrRef(string attrRefStr, std::vector<Declaration> declarations) {
	auto attrRefDelimPos = attrRefStr.find(ATTR_REF_DELIM);
	if (attrRefDelimPos == -1) { return false; }
	string decStr = attrRefStr.substr(0, attrRefDelimPos);
	string attrName = attrRefStr.substr(attrRefDelimPos + 1);
	optional<Declaration> syn = getFromDeclarations(decStr, declarations);
	if (!syn) { return false; }
	if (std::find(WITH_ATTRNAME.cbegin(), WITH_ATTRNAME.cend(), attrName) == WITH_ATTRNAME.cend()) { return false; }
	return true;
}

/**
* Returns true if str follows expression-spec :  '"' expr'"' | '_' '"' expr '"' '_' | '_'
*/
bool QueryUtil::isValidExprSpec(string exprStr) {
	if (exprStr == WILDCARD_KEYWORD) {
		return true;
	}
	if ((std::string(1, exprStr[0]) == WILDCARD_KEYWORD) && 
		(std::string(1, exprStr[exprStr.length() - 1]) == WILDCARD_KEYWORD)) {
		string ident = exprStr.substr(1, exprStr.length() - 2);
		return isValidExp(ident);
	}
	return isValidExp(exprStr);
}

/**
* Returns true if str is a valid expression
*/
bool QueryUtil::isValidExp(string exprStr) {
	if (removeAllWhitespaces(exprStr) == "") { return false; }
	if ((exprStr.at(0) != STR_DELIM) || (exprStr.at(exprStr.length() - 1) != STR_DELIM)) {
		return false;
	}
	try {
		auto tokens = TokenSplitter(exprStr).tokenize();
		ExpValidator(TokenIterator::cbegin(tokens)+1, TokenIterator::cend(tokens)-1).validateExpression();
	}
	catch (std::invalid_argument) {
		throw QuerySyntaxException("");
	}
	return true;
}

/**
* Return a string from a token list
*/
ArgString QueryUtil::stringFromTokens(std::vector<Token> tokens) {
	std::string selectArg = "";
	TokenIterator head = TokenIterator::cbegin(tokens);
	TokenIterator end = TokenIterator::cend(tokens);
	while (head != end) {
		selectArg.append(head.getValue());
		head.advance();
	}
	return selectArg;
}

/**
* Returns AttRef object with declaration and attrName string from attrRefStr
*/
AttrRef QueryUtil::getAttrRefFromStr(std::string attrRefStr, std::vector<Declaration> declarations)
{
	auto attrRefDelimPos = attrRefStr.find(ATTR_DELIM);
	if (attrRefDelimPos == -1) { throw QuerySyntaxException(""); }
	string decStr = attrRefStr.substr(0, attrRefDelimPos);
	string attrName = attrRefStr.substr(attrRefDelimPos + 1);
	optional<Declaration> syn = QueryUtil::getFromDeclarations(decStr, declarations);
	if (!syn) { throw QuerySyntaxException(""); }
	struct AttrRef attrRef = { syn.value(), attrName };
	return attrRef;
}

/**
* Returns true if for an AttrRef object, synonym-type and attrName are tied logically correctly
*/
bool QueryUtil::isValidSemanticAttrRef(AttrRef attrRef) {
	string synType = attrRef.synonym.synType->type;
	string attrName = attrRef.attrName;
	if (attrName == PROC_NAME_ATTRNAME) {
		return ((synType == DE::Call().type) ||
			(synType == DE::Procedure().type));
	}
	else if (attrName == VAR_NAME_ATTRNAME) {
		return ((synType == DE::Variable().type) ||
			(synType == DE::Read().type) ||
			(synType == DE::Print().type));
	}
	else if (attrName == VALUE_ATTRNAME) {
		return (synType == DE::Constant().type);
	}
	else if (attrName == STMTNO_ATTRNAME) {
		return ((synType == DE::Statement().type) ||
			(synType == DE::Read().type) ||
			(synType == DE::Print().type) ||
			(synType == DE::Call().type) ||
			(synType == DE::While().type) ||
			(synType == DE::If().type) ||
			(synType == DE::Assign().type));
	}
	return false;
}