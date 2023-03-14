#pragma once

#include "../Storage.h"

class IfPatternStorage : public Storage {
public:
	IfPatternStorage();

	void insertPattern(Ptn::IfPtn);

	/**
	* Gets all if pattern
	* return    : set of pair<string stmtNo, varName>
	*/
	PatternResults getAllIfPtn();

	/**
	* Gets all if pattern where varname is included in the if conditionals
	* param arg : string that describes the variable name
	* return    : set of pair<string stmtNo, varName>
	*/
	PatternResults getIfByArg1(patternArg);


private:
	stmtNoPtnRefPairSet allIfStmtNoPtnPair;
	ptnKeyStmtNoVecMap ifVarStmtNoVecOneToManyMap;

	void helperAddToMap(ptnKeyStmtNoVecMap&, mapKey, mapVal);
};