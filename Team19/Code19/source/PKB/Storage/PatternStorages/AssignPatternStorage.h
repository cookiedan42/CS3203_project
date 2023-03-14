#pragma once

#include "../Storage.h"

class AssignPatternStorage : public Storage {
public:
	AssignPatternStorage();

	void insertPattern(Ptn::AssignPtn);

	/**
	* Get all assigntment patterns
	* return	: set of pair<string stmtNo, string Lhs>
	*/
	PatternResults getAllAssign();

	/**
	* Gets all assignment patterns where lhs = arg1
	* Assumption is that there are no spacing in between entities within arg1
	*
	* param arg1: string exactly describing lhs of assignment
	* return	: set of pair<string stmtNo, string Lhs>
	*/
	PatternResults getAssignByArg1(patternArg);

	/**
	* Gets all assignment patterns where rhs is exactly arg 2
	* Assumption is that there are no spacing in between entities within arg2
	*
	* param	arg1: prefix notation of rhs
	* param	arg2: infix notation of rhs
	* return	: set of pair<string stmtNo, string Lhs>
	*/
	PatternResults getAssignByCompleteArg2(patternArg, patternArg);


	/**
	* Gets all assingment patterns where rhs can be found somewhere on the rhs of assignment (based on AST)
	* Assumption is that there are no spacing in between entities within arg1 and arg2
	*
	* param	arg1: prefix notation of rhs
	* param	arg2: infix notation of rhs
	* return	: set of pair<string stmtNo, string Lhs>
	*/
	PatternResults getAssignBySubArg2(patternArg, patternArg);

	/**
	* Gets all assignment patterns where lhs = arg1 and rhs is exactly arg2
	* Assumption is that there are no spacing in between entities within arg1 and arg2
	*
	* param arg1: string exactly describing lhs of assignment
	* param	arg2: prefix notation of rhs
	* param	arg3: infix notation of rhs
	* return	: set of pair<string stmtNo, string Lhs>
	*/
	PatternResults getAssignByArg1CompleteArg2(patternArg, patternArg, patternArg);

	/**
	* Gets all assignment patterns where lhs = arg1 and arg2 can partially be found in the rhs
	* Assumption is that there are no spacing in between entities within arg1
	*
	* param arg1: string exactly describing lhs of assignment
	* param	arg2: prefix notation of rhs
	* param	arg3: infix notation of rhs
	* return	: set of pair<string stmtNo, string Lhs>
	*/
	PatternResults getAssignByArg1SubArg2(patternArg, patternArg, patternArg);

private:

	PatternResults allAssignPair;

	stmtNoKeyPatternValueMap assignStmtNoPatternOneToOneMap;
	ptnKeyStmtNoVecMap assignRhsInOrderStmtNoOneToManyMap;
	ptnKeyStmtNoVecMap assignLhsStmtNoOneToManyMap;

	void helperAddToMap(ptnKeyStmtNoVecMap&, mapKey, mapVal);

};