#include "SelectParser.h"

SelectObject SelectParser::parse(std::vector<Token> selectArg, std::vector<Declaration> declarations) {
	TokenIterator head = TokenIterator::cbegin(selectArg);
	TokenIterator end = TokenIterator::cend(selectArg);

	if (head.valueIs(TUPLE_START_DELIMITER)) {
		// tuple must start with '<' and end with '>'
		end.back(); // going to '>'
		if (!end.valueIs(TUPLE_END_DELIMITER)) { throw QuerySyntaxException(""); }
		head.advance();
		std::vector<Token> tupleTokens = TokenIterator::copySubVector(head, end);
		std::vector<SynAttr> synAttrs = parseTuple(tupleTokens, declarations);
		std::vector<std::pair<Declaration, ArgType>> selectPairs = {};
		for (const auto& synAttrObj : synAttrs) {
			selectPairs.push_back(make_pair(synAttrObj.synonym, synAttrObj.attributeType));
		}
		return SelectObject(selectPairs);
	}

	if (head.valueIs(BOOL_KEYWORD) &&
		!QueryUtil::getFromDeclarations(BOOL_KEYWORD, declarations)) {
		// if BOOLEAN is definitely not a declaration
		return SelectObject();
	}

	std::vector<Token> tokens = TokenIterator::copySubVector(head, end);
	string selectElemStr = QueryUtil::stringFromTokens(tokens);
	struct SynAttr synAttr = parseDecOrSynAttr(selectElemStr, declarations);
	return SelectObject({ make_pair(synAttr.synonym, synAttr.attributeType) });
}

std::vector<SynAttr> SelectParser::parseTuple(std::vector<Token> tupleArgs, std::vector<Declaration> declarations)
{
	std::vector<SynAttr> result = {};
	string selectTupleStr = QueryUtil::stringFromTokens(tupleArgs);
	std::vector<string> tupleList = getListFromStr(selectTupleStr, SELECT_DEC_DELIM);
	for (const auto& element : tupleList) {
		result.push_back(parseDecOrSynAttr(element, declarations));
	}
	return result;
}

SynAttr SelectParser::parseDecOrSynAttr(std::string arg, std::vector<Declaration> declarations)
{
	bool isAttributeType = arg.find(ATTR_DELIM) != -1;
	if (isAttributeType) {
		if (!QueryUtil::isValidAttrRef(arg, declarations)) { throw QuerySyntaxException(""); }
		AttrRef attr = QueryUtil::getAttrRefFromStr(arg, declarations);
		if (!QueryUtil::isValidSemanticAttrRef(attr)) { throw QuerySyntaxException(""); }
		return getSynAttrFromAttrRef(attr);
	}
	else {
		optional<Declaration> syn = QueryUtil::getFromDeclarations(arg, declarations);
		if (!syn) { throw QuerySemanticException(""); }
		struct SynAttr synAtt = { syn.value(), ArgType::SYN };
		return synAtt;
	}
}

SynAttr SelectParser::getSynAttrFromAttrRef(AttrRef attrRef)
{
	if (attrRef.isNameType()) {
		struct SynAttr result = { attrRef.synonym, ArgType::ATTR_REF_NAME };
		return result;
	}
	if (attrRef.isIntType()) {
		struct SynAttr result = { attrRef.synonym, ArgType::ATTR_REF_INT };
		return result;
	}
	throw QuerySyntaxException("");
}

std::vector<string> SelectParser::getListFromStr(std::string arg, char delim)
{
	std::vector<string> result = {};
	std::string token;
	std::stringstream argSs = stringstream(arg);
	while (std::getline(argSs, token, delim)) {
		result.push_back(token);
	}
	return result;
}
