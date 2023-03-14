#pragma once

#include "../Storage.h"


class WhilePatternStorage : public Storage {
public:
	WhilePatternStorage();

	void insertPattern(Ptn::WhilePtn);

	/**
	* Gets all while pattern
	* return    : set of pair<string stmtNo, varName>
	*/
	PatternResults getAllWhilePtn();

	/**
	* Gets all while pattern where varname is included in the while conditionals
	* param arg : string that describes the variable name
	* return    : set of pair<string stmtNo, varName>
	*/
	PatternResults getWhileByArg1(patternArg);

private:

	stmtNoPtnRefPairSet allWhileStmtNoPtnPair;
	ptnKeyStmtNoVecMap whileVarStmtNoVecOneToManyMap;
	void helperAddToMap(ptnKeyStmtNoVecMap&, mapKey, mapVal);
};