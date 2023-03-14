#include "./QuerySplitter.h"

void QuerySplitter::validateHasSelect(const std::string& arg)
{
	auto selectPos = arg.find(SELECT_KEYWORD);
	if ((selectPos == -1) || (selectPos == 0)) { throw QuerySyntaxException(""); }
}

void QuerySplitter::parse() {
	extractDeclarations();
	extractSelect();
	extractClauses();
	// if anything empty --> throw
}

// declaration : design-entity synonym (',' synonym)* ';'
void QuerySplitter::extractDeclarations() {
	while (!head.valueIs(SELECT_KEYWORD)) {
		// abstract into getdeclaration
		// head and head.advance_to(;)
		auto declType = head.getValue();
		// catch invalid design-entities being called in declarations
		if (std::find(DE_TYPES.cbegin(), DE_TYPES.cend(), declType) == DE_TYPES.cend()) { throw QuerySyntaxException(""); }
		head.advance();
		while (!head.valueIs(DEC_DELIMITER)) {
			// alternate between value and ,
			if (!head.isName()) { throw QuerySyntaxException(""); }
			declarations.push_back(make_pair(head.getValue(), declType));
			head.advance();

			// either , or end of block
			// if , --> then loop and check next child of declaration
			if (head.valueIs(DEC_MULT_VAR_DELIMITER)) { head.advance(); continue; }
			// if ; --> loop and check next declaration type
			if (head.valueIs(DEC_DELIMITER)) { head.advance(); break; }
			throw QuerySyntaxException("");
		}
	}
	// all populated, head is pointing at "Select"
}

// 'Select' (elem | elem.attrRef | '<' elem(',' elem) * '>' | 'BOOLEAN')
void QuerySplitter::extractSelect() {
	if (!head.valueIs(SELECT_KEYWORD)) { throw QuerySyntaxException(""); }
	head.advance();
	// when Select is missing a declaration, but no clauses are behind
	if (head == end) { throw QuerySyntaxException(""); }

	if (head.valueIs(TUPLE_START_DELIMITER)) { // tuple type
		std::vector<Token> tuple = extractTuple();
		selectArg.insert(cend(selectArg), cbegin(tuple), cend(tuple));
	}
	else if (isAttrRef(head)) { // attrRef
		std::vector<Token> attrRef = extractAttrRef();
		selectArg.insert(cend(selectArg), cbegin(attrRef), cend(attrRef));
	}
	else if (head.valueIs(BOOL_KEYWORD)) { // boolean
		selectArg.push_back(head.getToken());
		head.advance();
	}
	else if (head.isName()) { // syn
		selectArg.push_back(head.getToken());
		head.advance();
	}
	else {
		throw QuerySyntaxException("");
	}
	// head is pointing to start of clauses
}

void QuerySplitter::extractClauses() {
	while (head != end) {
		if (head.valueIs(SUCH_THAT_FIRST_KEYWORD)) {
			head.advance();
			if (!head.valueIs(SUCH_THAT_SCND_KEYWORD)) { throw QuerySyntaxException(""); }
			head.advance();
			extractSuchThatClause();
		}
		else if (head.valueIs(PATTERN_KEYWORD)) {
			head.advance();
			extractPatternClause();
		}
		else if (head.valueIs(WITH_KEYWORD)) {
			head.advance();
			extractWithClause();
		
		}
		else { throw QuerySyntaxException(head.getValue()); }
	}
	// extra text after the terminating;
	if (head != end) { throw QuerySyntaxException("");}
}

// excluding{such that} type(contents)
void QuerySplitter::extractSuchThatClause() {
	// pointing at type
	std::vector<Token> arg {};
	
	// validate clause type against list
	auto clauseType = head.getValue();
	arg.push_back(head.getToken());
	head.advance();
	if (head.valueIs(REL_STAR_DELIM)) { 
		arg.push_back(head.getToken());
		head.advance();
	}

	extractBetweenBrackets(arg);
	suchThatClauses.push_back(arg);
	// pointing at token after close bracket

	if (head == end) { return; }

	// and means next is same type
	if (head.valueIs(AND_KEYWORD)) { 
		head.advance(); 
		extractSuchThatClause();
	}
}

// excluding{'pattern'} syn (pattern contents)
void QuerySplitter::extractPatternClause() {
	// pointing at syn
	std::vector<Token> arg {};
	if (!head.isName()) { throw QuerySyntaxException(""); }
	arg.push_back(head.getToken());
	head.advance();

	extractBetweenBrackets(arg);
	patternClauses.push_back(arg);
	// pointing at token after close bracket
	if (head == end) { return; }
	// and means same type
	if (head.valueIs(AND_KEYWORD)) { 
		head.advance(); 
		extractPatternClause();
	}
}

// excluding{'with'} ref '=' ref
void QuerySplitter::extractWithClause() {
	std::vector<Token> arg {};

	auto frontHalf = extractRef();
	arg.insert(arg.cend(), frontHalf.cbegin(), frontHalf.cend());
	if (!head.valueIs("=")) { throw QuerySyntaxException(""); }
	arg.push_back(head.getToken());
	head.advance();
	auto backHalf = extractRef();
	arg.insert(arg.cend(), backHalf.cbegin(), backHalf.cend());
	withClauses.push_back(arg);
	if (head == end) { return; }
	if (head.valueIs(AND_KEYWORD)) {
		head.advance();
		extractWithClause();
	}
}

// excluding{'Select'} <a,b,c>
std::vector<Token> QuerySplitter::extractTuple() {
	std::vector<Token> arg{};
	arg.push_back(head.getToken());
	head.advance();
	while (!head.valueIs(TUPLE_END_DELIMITER)) {
		// tupleVar: attrRef | syn
		if (isAttrRef(head)) {
			std::vector<Token> attrRef = extractAttrRef();
			arg.insert(cend(arg), cbegin(attrRef), cend(attrRef));
		}
		else if (head.isName()) {
			arg.push_back(head.getToken());
			head.advance();
		}
		else { throw QuerySyntaxException(""); }

		// next val: ',' | '>'
		if (head.valueIs(DEC_MULT_VAR_DELIMITER)) {
			arg.push_back(head.getToken());
			head.advance();
		}
		else if (head.valueIs(TUPLE_END_DELIMITER)) {
			arg.push_back(head.getToken());
			head.advance();
			break;
		}
		else { throw QuerySyntaxException(""); }
	}
	return arg;
}

// ref : '"' IDENT '"' | INTEGER | attrRef
std::vector<Token> QuerySplitter::extractRef() {
	std::vector<Token> arg {};
	// if " --> "NAME"
	if (head.valueIs("\"")) {
		arg.push_back(head.getToken());
		head.advance();

		if (!head.isName()) { throw QuerySyntaxException(""); }
		arg.push_back(head.getToken());
		head.advance();

		if (!head.valueIs("\"")) { throw QuerySyntaxException(""); }
		arg.push_back(head.getToken());
		head.advance();
	} 
	// if INTEGER
	else if (head.isInteger()) {
		arg.push_back(head.getToken());
		head.advance();

	} 
	// if syn.attrname
	else if (head.isName()) {
		auto attrRef = extractAttrRef();
		arg.insert(arg.cend(), attrRef.cbegin(), attrRef.cend());
	}
	else { throw QuerySyntaxException(""); }
	return arg;
}

std::vector<Token> QuerySplitter::extractAttrRef() {
	std::vector<Token> arg{};
	arg.push_back(head.getToken());
	head.advance();

	if (!head.valueIs(ATTR_STR_DELIM)) { throw QuerySyntaxException(""); }
	arg.push_back(head.getToken());
	head.advance();

	// this is either one name or name + stmt
	if (head.valueIs(PROC_NAME_ATTRNAME)) {
		arg.push_back(head.getToken());
		head.advance();
	}
	else if (head.valueIs(VAR_NAME_ATTRNAME)) {
		arg.push_back(head.getToken());
		head.advance();
	}
	else if (head.valueIs(VALUE_ATTRNAME)) {
		arg.push_back(head.getToken());
		head.advance();
	}
	else if (head.valueIs(STMTNO_KEYWORD)) {
		arg.push_back(head.getToken());
		head.advance();
		if (!head.valueIs(STMTNO_HASH)) { throw QuerySyntaxException(""); }
		arg.push_back(head.getToken());
		head.advance();
	}
	return arg;
}

bool QuerySplitter::isAttrRef(TokenIterator currTokeIt) {
	return (currTokeIt.isName()) &&
		(currTokeIt != end - 1) &&
		(currTokeIt.valueIs(ATTR_STR_DELIM, 1));
}

void QuerySplitter::extractBetweenBrackets(std::vector<Token>& arg) {
	if (!head.valueIs(BRACKET_START_DELIMITER)) { throw QuerySyntaxException(""); }
	arg.push_back(head.getToken());
	head.advance();

	int brackets = 1;
	while (brackets > 0) {
		if (head.valueIs(BRACKET_START_DELIMITER)) { brackets++; }
		if (head.valueIs(BRACKET_END_DELIMITER)) { brackets--; }
		arg.push_back(head.getToken());
		head.advance();
	}

}