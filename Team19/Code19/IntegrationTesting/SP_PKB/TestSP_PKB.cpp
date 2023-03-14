#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Request/GetRequests/GetPatternRequests/GetAssignPatternRequest.h"
#include "../source/Request/GetRequests/GetPatternRequests/GetIfPatternRequest.h"
#include "../source/Request/GetRequests/GetPatternRequests/GetWhilePatternRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetAssignStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetCallStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetConstantEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetIfStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetPrintStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetProcedureEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetReadStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetVariableEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetWhileStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetCallsRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetFollowsRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetModifiesRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetParentRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetUsesRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetNextRequest.h"
#include "../source/SourceProcessor/SPManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// test that predicted can be extracted on the other side
using singleSet = std::unordered_set<std::string>;
using pairVec = std::unordered_set<std::pair<std::string, std::string>, stringPairHash>;
using pairSet = std::unordered_set<std::string>;
using patternSet = std::unordered_set<std::pair<std::string, std::string>, stringPairHash>;

pairSet makeSet(pairVec inp) {
	pairSet out{};
	for each (std::pair < std::string, std::string> p in inp) {
		out.insert({ p.first + " " + p.second });
	}
	return out;
}

PKB makePKB() {
	auto pkb = PKB();
	auto insertFacade = InsertFacade(pkb);
	auto source =
		"procedure p {			"
		"	while(a>b){			" // 1
		"		if(a<b)then{	" // 2
		"			read a;		" // 3
		"		} else {		"
		"			print c;	" // 4
		"			inside = c ;" // 5
		"		}				"
		"		call q;			" // 6
		"	}					"
		"	RHS = 1+d ;			" // 7
		"	print print;		" // 8
		"}"
		"procedure q {			"
		"	e = e;				" // 9
		"	call r;				" // 10
		"}"
		"procedure r {			"
		"	f = f;				" // 11
		"}";
	SPManager::construct(source).populatePKB(insertFacade);
	return pkb;
}

namespace IntegrationTesting {
	TEST_CLASS(Test_SP_PKB) {
public:
	TEST_METHOD(Test_Constant) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetConstantEntityRequest>();
		singleSet outConstant = getFacade.get(request);
		singleSet correctConstant{ "1" };
		Assert::IsTrue(correctConstant == outConstant);
	}

	TEST_METHOD(Test_Var) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetVariableEntityRequest>();
		singleSet outVar = getFacade.get(request);
		singleSet correctVar{ "a","b","c","d","e","f","print","RHS","inside" };
		Assert::IsTrue(correctVar == outVar);
	}

	TEST_METHOD(Test_Proc) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetProcedureEntityRequest>();
		singleSet outProc = getFacade.get(request);
		singleSet correctProc{ "p", "q", "r" };
		Assert::IsTrue(correctProc == outProc);
	}

	TEST_METHOD(Test_Stmt) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetStatementEntityRequest>(EntityMode::STATEMENT);
		singleSet outStmt = getFacade.get(request);
		singleSet correctStmt{ "1","2","3","4","5","6","7", "8", "9", "10", "11" };
		Assert::IsTrue(correctStmt == outStmt);
	}

	TEST_METHOD(Test_Read) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetReadStatementEntityRequest>(EntityMode::READ);
		singleSet outRead = getFacade.get(request);
		singleSet correctRead{ "3" };
		Assert::IsTrue(correctRead == outRead);
	}

	TEST_METHOD(Test_Print) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetPrintStatementEntityRequest>(EntityMode::PRINT);
		singleSet outPrint = getFacade.get(request);
		singleSet correctPrint{ "4", "8" };
		Assert::IsTrue(correctPrint == outPrint);
	}

	TEST_METHOD(Test_Call) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetCallStatementEntityRequest>(EntityMode::CALL);
		singleSet outCall = getFacade.get(request);
		singleSet correctCall{ "6", "10" };
		Assert::IsTrue(correctCall == outCall);
	}

	TEST_METHOD(Test_While) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetWhileStatementEntityRequest>();
		singleSet outWhile = getFacade.get(request);
		singleSet correctWhile{ "1" };
		Assert::IsTrue(correctWhile == outWhile);
	}

	TEST_METHOD(Test_If) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetIfStatementEntityRequest>();
		singleSet outIf = getFacade.get(request);
		singleSet correctIf{ "2" };
		Assert::IsTrue(correctIf == outIf);
	}

	TEST_METHOD(Test_Assign) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetAssignStatementEntityRequest>();
		singleSet outAssign = getFacade.get(request);
		singleSet correctAssign{ "5", "7", "9", "11" };
		Assert::IsTrue(correctAssign == outAssign);
	}

	TEST_METHOD(Test_Follows) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL);
		auto outFollows = makeSet(getFacade.get(request));
		pairSet correctFollows = makeSet({ {"1","7"}, {"2","6"}, {"4","5"}, {"7","8"}, {"9","10"} });
		Assert::IsTrue(correctFollows == outFollows);
	}

	TEST_METHOD(Test_FollowsStar) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetFollowsRequest>(Mode::ALL, STAR);
		auto outFollowsStar = makeSet(getFacade.get(request));
		pairSet correctFollowsStar = makeSet({
			{"1","7"}, {"2","6"}, {"4","5"}, {"7","8"}, {"9", "10"},
			{"1","8"},
			});
		Assert::IsTrue(correctFollowsStar == outFollowsStar);
	}

	TEST_METHOD(Test_Parent) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetParentRequest>(Mode::ALL);
		auto outParent = makeSet(getFacade.get(request));
		pairSet correctParent = makeSet({ {"1","2"},{"1","6"}, {"2","3"},{"2","4"},{"2","5"} });
		Assert::IsTrue(correctParent == outParent);
	}

	TEST_METHOD(Test_ParentStar) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetParentRequest>(Mode::ALL, STAR);
		auto outParentStar = makeSet(getFacade.get(request));
		pairSet correctParentStar = makeSet({
			{"1","2"},{"1","6"},{"2","3"},{"2","4"},{"2","5"},
			{"1","3" },{"1","4" },{"1","5" },
			});
		Assert::IsTrue(correctParentStar == outParentStar);
	}

	TEST_METHOD(Test_Uses) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetUsesRequest>(Mode::ALL);
		auto outUses = makeSet(getFacade.get(request));
		pairSet correctUses = makeSet({
			{"1","a"},{"1","b"},
			{"1","c"}, // nested uses
			{"1","e"},{"1","f"}, // indirect uses
			{"2","a"},{"2","b"},
			{"2","c"}, // nested uses
			{"4","c"},{"5","c"},
			{"6","e"},{"6","f"}, // call uses
			{"7","d"},
			{"8","print"},
			{"9","e"},
			{"10","f"}, // call uses
			{"11","f"},
			{"p", "a"}, // procedure uses
			{"p", "b"},
			{"p", "c"},
			{"p", "d"},
			{"p","e"},{"p","f"}, // indirect uses
			{"p", "print"},
			{"q","e"},{"q","f"},
			{"r","f"},
			});
		Assert::IsTrue(correctUses == outUses);
	}

	TEST_METHOD(Test_Modifies) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetModifiesRequest>(Mode::ALL);
		auto outModifies = makeSet(getFacade.get(request));
		pairSet correctModifies = makeSet({
			//direct
			{"5","inside"},{"7","RHS"},
			{"3","a"},
			{"6","e"},{"6","f"}, // call modifies
			{"9","e"},
			{"10","f"}, // call modifies
			{"11","f"},
			//nested
			{"1","inside"},{"2","inside"},
			{"1","a"},{"2","a"},
			{"1","e"},{"1","f"}, // indirect modifies
			// procedure
			{"p", "a"},
			{"p", "inside"},
			{"p", "RHS"},
			{"p","e"},{"p","f"}, // indirect modifies
			{"q","e"},{"q","f"},
			{"r","f"},
			});
		Assert::IsTrue(correctModifies == outModifies);
	}

	TEST_METHOD(Test_WhilePattern) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetWhilePatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
		patternSet outPattern = getFacade.get(request);
		patternSet correctPattern = { {"1", "a"}, {"1", "b"} };
		Assert::IsTrue(correctPattern == outPattern);
	}

	TEST_METHOD(Test_IfPattern) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetIfPatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
		patternSet outPattern = getFacade.get(request);
		patternSet correctPattern = { {"2", "a"}, {"2", "b"} };
		Assert::IsTrue(correctPattern == outPattern);
	}

	TEST_METHOD(Test_AssignPattern) {
		auto pkb = makePKB();
		auto getFacade = GetFacade(pkb);
		auto request = std::make_shared<GetAssignPatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
		patternSet outPattern = getFacade.get(request);
		patternSet correctPattern = { {"5", "inside"}, {"7", "RHS"}, {"9","e"}, {"11", "f"} };
		Assert::IsTrue(correctPattern == outPattern);
	}
	};
}