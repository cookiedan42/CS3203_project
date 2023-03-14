#include "stdafx.h"
#include "../source/QP/QueryObject/SuchThatClause.h"
#include "../source/QP/QueryEvaluator/PatternAssignEvaluator.h"
#include "../source/QP/QueryEvaluator/PatternIfEvaluator.h"
#include "../source/QP/QueryEvaluator/PatternWhileEvaluator.h"
#include "../source/QP/QueryEvaluator/QueryUtil.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Request/InsertRequests/InsertPatternRequests/InsertAssignPatternRequest.h"
#include "../source/Request/InsertRequests/InsertPatternRequests/InsertIfPatternRequest.h"
#include "../source/Request/InsertRequests/InsertPatternRequests/InsertWhilePatternRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertStatementEntityRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertVariableEntityRequest.h"
#include <codecvt>
#include <locale>
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting

{
	TEST_CLASS(Test_QE_Pattern)
	{
	public:

		PKB initialisePKB() {
			auto pkb = PKB();
			auto insertFacade = InsertFacade(pkb);
			
			AssignStatement s1(1);
			AssignStatement s2(2);
			AssignStatement s3(3);
			AssignStatement s4(4);
			AssignStatement s5(10);
			IfStatement s6(5);
			IfStatement s7(6);
			WhileStatement s8(7);
			WhileStatement s9(8);

			Variable v1( "y");
			Variable v2( "x");
			Variable v3( "z");
			Variable v4( "z");
			Variable v5( "z");
			Variable v6( "y");
			Variable v7( "y");
			Variable v8( "y");
			Variable v9( "x");
			Variable v10( "x");
			Variable v11( "m");
			Variable v12( "m");
			Variable v13("m123");
			Variable v14("za");
			Variable v15("am");

			std::vector<Ent::Statement> stmtList{};
			std::vector<Ent::Variable> varList{};

			stmtList.push_back(s1);
			stmtList.push_back(s2);
			stmtList.push_back(s3);
			stmtList.push_back(s4);
			stmtList.push_back(s5);
			stmtList.push_back(s6);
			stmtList.push_back(s7);
			stmtList.push_back(s8);
			stmtList.push_back(s9);

			varList.push_back(v1);
			varList.push_back(v2);
			varList.push_back(v3);
			varList.push_back(v4);
			varList.push_back(v5);
			varList.push_back(v6);
			varList.push_back(v7);
			varList.push_back(v8);
			varList.push_back(v9);
			varList.push_back(v10);
			varList.push_back(v11);
			varList.push_back(v12);
			varList.push_back(v13);
			varList.push_back(v14);
			varList.push_back(v15);

			for (Variable a : varList) {
				auto requestV = std::make_shared<InsertVariableEntityRequest>(a);
				insertFacade.insert(requestV);
			}

			for (auto a : stmtList) {
				auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
				insertFacade.insert(requestS);
			}

			// insert assign patterns
			std::vector<Ptn::AssignPtn> assignPtnList{};
			std::vector<Ptn::IfPtn> ifPtnList{};
			std::vector<Ptn::WhilePtn> whilePtnList{};
			assignPtnList.push_back(Ptn::AssignPtn(1, "x", "(y)", "(y)")); // x=y
			assignPtnList.push_back(Ptn::AssignPtn(2, "x", "(+(y)(z))", "((y)+(z))"));
			assignPtnList.push_back(Ptn::AssignPtn(3, "x", "(+(+(y)(z))(m))", "(((y)+(z))+(m))"));  // y+z y+z+m
			assignPtnList.push_back(Ptn::AssignPtn(4, "y", "(+(+(y)(z))(m))", "(((y)+(z))+(m))"));  // y+z y+z+m
			assignPtnList.push_back(Ptn::AssignPtn(10, "m123", "(+(+(x)(y))(m123))", "(((x)+(y))+(m123))"));
			assignPtnList.push_back(Ptn::AssignPtn(11, "y", "(+(+(y)(za))(m))", "(((y)+(za))+(m))")); // y + za
			assignPtnList.push_back(Ptn::AssignPtn(12, "y", "(+(+(y)(z))(am))", "(((y)+(z))+(am))")); // y + z

			// insert if patterns
			std::vector<Variable> entList1 = { Variable("a") };
			std::vector<Variable> entList2 = { Variable("a"), Variable("b") };

			ifPtnList.push_back(Ptn::IfPtn(5, entList1)); // { 5, "a" }
			ifPtnList.push_back(Ptn::IfPtn(6, entList2)); // { 6, "a" } { 6, "b" }

			// insert while patterns
			std::vector<Variable> whileEntList1 = { Variable("a") };
			std::vector<Variable> whileEntList2 = { Variable("a"), Variable("b") };

			whilePtnList.push_back(Ptn::WhilePtn(7, whileEntList1));
			whilePtnList.push_back(Ptn::WhilePtn(8, whileEntList2));

			for (Ptn::AssignPtn a : assignPtnList) {
				auto requestPtn = std::make_shared<InsertAssignPatternRequest>(a);
				insertFacade.insert(requestPtn);
			}

			for (Ptn::IfPtn a : ifPtnList) {
				auto requestPtn = std::make_shared<InsertIfPatternRequest>(a);
				insertFacade.insert(requestPtn);
			}

			for (Ptn::WhilePtn a : whilePtnList) {
				auto requestPtn = std::make_shared<InsertWhilePatternRequest>(a);
				insertFacade.insert(requestPtn);
			}

			return pkb;
		}

		bool doesPairExist(std::string arg1, std::string arg2, stmtNoPtnRefPairSet vec) {
			std::pair<std::string, std::string> item = std::make_pair(arg1, arg2);

			if (find(vec.begin(), vec.end(), item) != vec.end()) {
				return true;
			}
			return false;
		}
		bool isEqualVec(std::vector<std::string> v1, std::vector<std::string> v2) {
			std::sort(v1.begin(), v1.end());
			std::sort(v2.begin(), v2.end());
			return v1 == v2;
		}

		TEST_METHOD(Test_PatternAssignEvaluator_getRelationPairs_evaluateNoCommSyn)
		{
			/*
			* Test method: PatternAssignEvaluator::evaluateNoCommSyn
			* To test for Pattern assign relationship with no common synonym with others.
			* Should return boolean.
			*/
			Argument varSyn1 = { Declaration(make_shared<DE::Variable>(),"v") };
			Argument wildcard = { WildCard() };
			Argument complete1 = { StringExp("y+z+m") };
			Argument complete2 = { StringExp("y") };
			Argument sub1 = { SubExp("z") };
			Argument sub2 = { SubExp("y+z") };
			Argument sub3 = { SubExp("m123") };

			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);
			std::vector<std::pair<std::string, std::string>> expected;
			stmtNoPtnRefPairSet r; // result for getRelationPairs
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;
			bool res; // result for evaluateNoCommSyn
			shared_ptr<PatternClause> pt;

			expected = { {"1", "x"}, {"2", "x"}, {"3", "x"}, {"4", "y"}, {"10", "m123"}, {"11", "y"}, {"12", "y"} };
			Declaration de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, varSyn1, wildcard);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("1", "x", r), true);
			Assert::AreEqual(doesPairExist("2", "x", r), true);
			Assert::AreEqual(doesPairExist("3", "x", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), true);
			Assert::AreEqual(doesPairExist("10", "m123", r), true);
			Assert::AreEqual(doesPairExist("11", "y", r), true);
			Assert::AreEqual(doesPairExist("12", "y", r), true);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);


			expected = { {"1", "x"}, {"2", "x"}, {"3", "x"}, {"4", "y"}, {"10", "m123"}, {"11", "y"}, {"12", "y"} };
			de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, wildcard, wildcard);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("1", "x", r), true);
			Assert::AreEqual(doesPairExist("2", "x", r), true);
			Assert::AreEqual(doesPairExist("3", "x", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), true);
			Assert::AreEqual(doesPairExist("10", "m123", r), true);
			Assert::AreEqual(doesPairExist("11", "y", r), true);
			Assert::AreEqual(doesPairExist("12", "y", r), true);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);


			expected = { {"3", "x"}, {"4", "y"} };
			de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, varSyn1, complete1);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("2", "x", r), false);
			Assert::AreEqual(doesPairExist("3", "x", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), true);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);


			expected = { {"2", "x"}, {"3", "x"}, {"4", "y"}, {"12", "y"}};
			de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, wildcard, sub1);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("2", "x", r), true);
			Assert::AreEqual(doesPairExist("3", "x", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), true);
			Assert::AreEqual(doesPairExist("12", "y", r), true);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);

			expected = { {"2", "x"}, {"3", "x"}, {"4", "y"}, {"12", "y"} };
			de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, varSyn1, sub1);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("2", "x", r), true);
			Assert::AreEqual(doesPairExist("3", "x", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), true);
			Assert::AreEqual(doesPairExist("12", "y", r), true);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);


			expected = { {"4", "y"}, {"11", "y"}, {"12", "y"} };
			de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, complete2, wildcard);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("1", "x", r), false);
			Assert::AreEqual(doesPairExist("2", "x", r), false);
			Assert::AreEqual(doesPairExist("4", "y", r), true);
			Assert::AreEqual(doesPairExist("11", "y", r), true);
			Assert::AreEqual(doesPairExist("12", "y", r), true);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);

			expected = { {"4", "y"} };
			de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, complete2, complete1);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("1", "x", r), false);
			Assert::AreEqual(doesPairExist("2", "x", r), false);
			Assert::AreEqual(doesPairExist("4", "y", r), true);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);

			expected = { {"4", "y"}, {"12", "y"} };
			de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, complete2, sub2);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade); // "y+z"
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("2", "x", r), false);
			Assert::AreEqual(doesPairExist("3", "x", r), false);
			Assert::AreEqual(doesPairExist("12", "y", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), true);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);


			expected = { {"1", "x"} };
			de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, wildcard, complete2);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("1", "x", r), true);
			Assert::AreEqual(doesPairExist("3", "x", r), false);
			Assert::AreEqual(doesPairExist("4", "y", r), false);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);

			expected = { {"10", "m123"} };
			de = Declaration(make_shared<DE::Assign>(), "a");
			pt = make_shared<PatternClause>(de, wildcard, sub3);
			r = PatternAssignEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("10", "m123", r), true);
			Assert::AreEqual(doesPairExist("3", "x", r), false);
			Assert::AreEqual(doesPairExist("4", "y", r), false);
			res = PatternAssignEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);
		}

		//TEST_METHOD(Test_PatternAssignEvaluator_evaluateHasCommSyn)
		//{
		//	/*
		//	* Test method: PatternAssignEvaluator::evaluateHasCommSyn
		//	* To test for pattern a (arg1, arg2) result table.
		//	* Should return unordered_map<string, vector<string>>
		//	*/

		//	PKBFacade* pkb = initialisePKB();
		//	// pattern pairs include{ {"1","x"}, {"2","x"}, {"3","x"}, {"4","y"} };
		//	std::unordered_map<std::string, std::vector<std::string>> expected;
		//	std::unordered_map<std::string, std::vector<std::string>> r;
		//	std::pair<std::string, std::vector<std::string>> rPair;

		//	expected = { { "a", { "1", "2", "3", "4"} }, {"v", {"x", "x", "x", "y"} } };
		//	Declaration de = Declaration(make_shared<DE::Assign>(), "a");
		//	PatternClause pt = make_shared<PatternClause>(de, "v", "_");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//	if (r.find("v") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["v"], r["v"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { { "a", { "1"}}, {"v", { "x"} } };
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "v", "\"y\"");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//	if (r.find("v") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["v"], r["v"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { { "a", { "1", "2", "3", "4"} }, {"v", {"x", "x", "x", "y"} } };
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "v", "_\"y\"_");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//	if (r.find("v") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["v"], r["v"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { {"a", { "1" }} };
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "_", "\"y\"");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}


		//	expected = { {"a", {"1", "2", "3", "4"} }, { "v", {"x", "x", "x", "y"} } };
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "v", "_");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//	if (r.find("v") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["v"], r["v"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}


		//	expected = { {"a", {"1", "2", "3", "4"}}};
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "_", "_");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//

		//	expected = { { "a", {"3", "4"} }, {"v", {"x", "y"} } };
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "v", "\"y+z+m\"");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//	if (r.find("v") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["v"], r["v"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}


		//	expected = { {"a", {"2","3","4"}}};
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "_", "_\"z\"_");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { {"a", {"2","3","4"}}, {"v", {"x", "x", "y"}}};
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "v", "_\"z\"_");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//	if (r.find("v") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["v"], r["v"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}


		//	expected = { {"a", {"4"}} };
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "\"y\"", "_");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { {"a", {"4"}} };
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "\"y\"", "\"y+z+m\"");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { {"a", {"2", "3"}} };
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "\"x\"", "_\"y+z\"_");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}


		//	expected = { {"a", {"1"}}};
		//	de = Declaration(make_shared<DE::Assign>(), "a");
		//	pt = make_shared<PatternClause>(de, "_", "\"y\"");
		//	r = PatternAssignEvaluator::evaluateHasCommSyn(pt, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("a") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["a"], r["a"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//}

		TEST_METHOD(Test_PatternIfEvaluator_getRelationPairs_evaluateNoCommSyn)
		{
			/*
			* Test method: PatternIfEvaluator::evaluateNoCommSyn
			* To test for Pattern if relationship with no common synonym with others.
			* Should return boolean.
			*/
			Argument varSyn1 = { Declaration(make_shared<DE::Variable>(),"v") };
			Argument wildcard = { WildCard() };
			Argument varname_a = { StringExp("a") };
			Argument varname_c = { StringExp("c") };

			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);
			std::vector<std::pair<std::string, std::string>> expected;
			stmtNoPtnRefPairSet r; // result for getAllMatches
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;
			bool res; // result for evaluateNoCommSyn
			shared_ptr<PatternClause> pt;

			// WILDCARD_WILDCARD
			expected = { {"5", "a"}, {"6", "a"}, {"6", "b"} };
			Declaration w = Declaration(make_shared<DE::If>(), "w");
			pt = make_shared<PatternClause>(w, wildcard, wildcard);
			r = PatternIfEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("5", "a", r), true);
			Assert::AreEqual(doesPairExist("6", "a", r), true);
			Assert::AreEqual(doesPairExist("6", "b", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), false);
			res = PatternIfEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);

			// SYN_WILDCARD
			expected = { {"5", "a"}, {"6", "a"}, {"6", "b"} };
			w = Declaration(make_shared<DE::If>(), "w");
			pt = make_shared<PatternClause>(w, varSyn1, wildcard);
			r = PatternIfEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("5", "a", r), true);
			Assert::AreEqual(doesPairExist("6", "a", r), true);
			Assert::AreEqual(doesPairExist("6", "b", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), false);
			res = PatternIfEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);


			// Test get all
			auto request = std::make_shared<GetIfPatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
			PatternResults results = getFacade.get(request);
			int sizeAll = results.size();
			Assert::AreEqual(3, sizeAll);

			// Test quoted
			request = std::make_shared <GetIfPatternRequest>(PatternMode::ARG1, "a", EMPTY_ARG);
			results = getFacade.get(request);
			int sizePtn = results.size();
			Assert::AreEqual(2, sizePtn);

			// QUOTE_WILDCARD - positive
			expected = { {"5", "a"}, {"6", "a"} };
			w = Declaration(make_shared<DE::If>(), "w");
			pt = make_shared<PatternClause>(w, varname_a, wildcard);
			r = PatternIfEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("5", "a", r), true);
			Assert::AreEqual(doesPairExist("6", "a", r), true);
			Assert::AreEqual(doesPairExist("6", "b", r), false);
			res = PatternIfEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);

			// QUOTE_WILDCARD - negative
			expected = { };
			w = Declaration(make_shared<DE::If>(), "w");
			pt = make_shared<PatternClause>(w, varname_c, wildcard);
			r = PatternIfEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("5", "a", r), false);
			Assert::AreEqual(doesPairExist("6", "a", r), false);
			Assert::AreEqual(doesPairExist("6", "b", r), false);
			res = PatternIfEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsFalse(res);
		}

		TEST_METHOD(Test_PatternWhileEvaluator_getRelationPairs_evaluateNoCommSyn)
		{
			/*
			* Test method: PatternWhileEvaluator::evaluateNoCommSyn
			* To test for Pattern while relationship with no common synonym with others.
			* Should return boolean.
			*/
			Argument varSyn1 = { Declaration(make_shared<DE::Variable>(),"v") };
			Argument wildcard = { WildCard() };
			Argument varname_a = { StringExp("a") };
			Argument varname_c = { StringExp("c") };

			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);
			std::vector<std::pair<std::string, std::string>> expected;
			stmtNoPtnRefPairSet r; // result for getAllMatches
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;
			bool res; // result for evaluateNoCommSyn
			shared_ptr<PatternClause> pt;

			// WILDCARD_WILDCARD
			expected = { {"7", "a"}, {"8", "a"}, {"8", "b"} };
			Declaration w = Declaration(make_shared<DE::While>(), "w");
			pt = make_shared<PatternClause>(w, wildcard, wildcard);
			r = PatternWhileEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("7", "a", r), true);
			Assert::AreEqual(doesPairExist("8", "a", r), true);
			Assert::AreEqual(doesPairExist("8", "b", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), false);
			res = PatternWhileEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);

			// SYN_WILDCARD
			expected = { {"7", "a"}, {"8", "a"}, {"8", "b"} };
			w = Declaration(make_shared<DE::While>(), "w");
			pt = make_shared<PatternClause>(w, varSyn1, wildcard);
			r = PatternWhileEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("7", "a", r), true);
			Assert::AreEqual(doesPairExist("8", "a", r), true);
			Assert::AreEqual(doesPairExist("8", "b", r), true);
			Assert::AreEqual(doesPairExist("4", "y", r), false);
			res = PatternWhileEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);

			// QUOTE_WILDCARD - positive
			expected = { {"7", "a"}, {"8", "a"} };
			w = Declaration(make_shared<DE::While>(), "w");
			pt = make_shared<PatternClause>(w, varname_a, wildcard);
			r = PatternWhileEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("7", "a", r), true);
			Assert::AreEqual(doesPairExist("8", "a", r), true);
			Assert::AreEqual(doesPairExist("8", "b", r), false);
			res = PatternWhileEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsTrue(res);

			// QUOTE_WILDCARD - negative
			expected = { };
			w = Declaration(make_shared<DE::While>(), "w");
			pt = make_shared<PatternClause>(w, varname_c, wildcard);
			r = PatternWhileEvaluator::getAllMatches(pt, &getFacade);
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesPairExist("7", "a", r), false);
			Assert::AreEqual(doesPairExist("8", "a", r), false);
			Assert::AreEqual(doesPairExist("8", "b", r), false);
			res = PatternWhileEvaluator::evaluateNoCommSyn(pt, &getFacade);
			Assert::IsFalse(res);
		}
	};
}