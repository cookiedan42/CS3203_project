#include "DeclarationsParser.h"

std::vector<Declaration> DeclarationsParser::parse(std::vector<QuerySplitter::dec_pair> declarations) {
	std::vector<Declaration> results = {};
	std::vector<std::string> usedSynNames = {};
	for (auto it = declarations.begin(); it != declarations.end(); it++) {
		string synName = it->first;
		string synTypeStr = it->second;
		if (synTypeStr == "" || synName == "") { throw QuerySyntaxException(""); }
		shared_ptr<DesignEntity> synEntType = validateSynType(synTypeStr);
		// catch if synonym name is declared more than once
		if (std::find(usedSynNames.cbegin(), usedSynNames.cend(), synName) != usedSynNames.cend()) {
			throw QuerySemanticException("");
		}
		results.push_back(Declaration(synEntType, synName));
		usedSynNames.push_back(synName);
	}
	return results;
}

//Validate and classify declaration string type
shared_ptr<DesignEntity> DeclarationsParser::validateSynType(std::string synTypeStr) {
	return DesignEntityFactory::createDesignEntity(synTypeStr);
}