#include "stdafx.h"
#include "../source/QP/QueryObject/SuchThatClause.h"
#include "../source/QP/QueryEvaluator/UsesEvaluator.h"
#include "../source/QP/QueryEvaluator/QueryUtil.h"
#include "../source/QP/QueryObject/DesignAbstractionFactory.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertUsesRequest.h"
#include <codecvt>
#include <locale>
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting

{
	TEST_CLASS(Test_QE_Uses)
	{
	public:

		PKB* initialisePKB() {
			PKB* pkb = new PKB();
			InsertFacade insertFacade = InsertFacade(*pkb);

			Statement s3(3, "print");
			Statement s4(4, "assign");
			Statement s5(5, "assign");
			Statement s6(6, "print");

			Variable v1( "x1");
			Variable v2( "y");
			Variable v3( "x2");
			Variable v4( "x3");
			Variable v5( "z");
			Variable v6( "y");

			// please note that order of insertion matters for checking
			Uses r1(s3, v5);
			Uses r2(s4, v1);
			Uses r3(s4, v3);
			Uses r4(s4, v4);
			Uses r5(s5, v2);
			Uses r6(s6, v6);

			vector<Rel::Uses> usesList{};

			usesList.push_back(r1);
			usesList.push_back(r2);
			usesList.push_back(r3);
			usesList.push_back(r4);
			usesList.push_back(r5);
			usesList.push_back(r6);

			for (Uses a : usesList) {
				auto usesRequest = std::make_shared<InsertUsesRequest>(a);
				insertFacade.insert(usesRequest);
			}

			return pkb;
		}

		bool doesPairExists(std::string arg1, std::string arg2, RelationshipResults results) {
			std::pair<std::string, std::string> item = std::make_pair(arg1, arg2);

			if (find(results.begin(), results.end(), item) != results.end()) {
				return true;
			}
			return false;
		}

		bool isEqualVec(std::vector<std::string> v1, std::vector<std::string> v2) {
			bool res = true;
			for (size_t i = 0; i < v1.size(); i++) {
				if (v1[i] != v2[i]) {
					res = false;
					break;
				}
			}
			return res;
		}

		bool checkRowsEqualExpected(vector<vector<string>> rows, vector<vector<string>> expected)
		{
			bool res = true;
			for (auto& row : rows)
			{
				if (checkContainsRow(row, expected)) {
					continue;
				}
				else {
					res = false;
					break;
				}
			}
			return res;
		}

		bool checkContainsRow(vector<string> row, vector<vector<string>> expected)
		{
			bool res = false;
			for (auto& expect : expected)
			{
				if (isEqualVec(row, expect)) {
					res = true;
					break;
				}
			}
			return res;
		}
		TEST_METHOD(Test_UsesEvaluator_getRelationPairs_evaluateClauseWithNoSyn)
		{
			/*
			* Test method: e.evaluateClauseWithNoSyn
			* To test for Uses relationship with no common synonym with others.
			* Should return boolean.
			*/

			// Uses pairs include{ {"3","z"}, {"4","x1"}, {"4","x2"}, {"4","x3"}, {"5","y"}, {"6", "y"} };
			struct Argument stmtno1 = { StmtNo("1") };
			struct Argument stmtno2 = { StmtNo("2") };
			struct Argument stmtno3 = { StmtNo("3") };
			struct Argument stmtno4 = { StmtNo("4") };
			struct Argument stmtno5 = { StmtNo("5") };
			struct Argument stmtno6 = { StmtNo("6") };
			struct Argument varname_x = { StringExp("x") };
			struct Argument varname_y = { StringExp("y") };
			struct Argument varname_z = { StringExp("z") };
			struct Argument varname_x1 = { StringExp("x1") };
			struct Argument varname_x2 = { StringExp("x2") };
			struct Argument varname_x3 = { StringExp("x3") };
			struct Argument stmtSyn1 = { Declaration(make_shared<DE::Statement>(), "s1") };
			struct Argument stmtSyn2 = { Declaration(make_shared<DE::Statement>(), "s2") };
			struct Argument varSyn1 = { Declaration(make_shared<DE::Variable>(), "y") };
			struct Argument wildcard = { WildCard() };

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			std::vector<std::pair<std::string, std::string>> expected;
			RelationshipResults r; // result for getRelationPairs
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;
			bool res; // result for evaluateClauseWithNoSyn

			// STMNO_QUOTE
			expected = { {"3", "z"} };
			SuchThatClause st = SuchThatClause(make_shared<DA::UsesRel>(stmtno3, varname_z));
			UsesEvaluator e = UsesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("3", "z", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// STMTNO_QUOTE
			expected = { };
			st = SuchThatClause(make_shared<DA::UsesRel>(stmtno3, varname_x));
			e = UsesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			// STMTNO_SYN
			expected = { {"4", "x1"}, {"4", "x2"}, {"4", "x3"} };
			st = SuchThatClause(make_shared<DA::UsesRel>(stmtno4, stmtSyn1)); // z is a synonym, should return all variables used by s4
			e = UsesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("4", "x1", r));
			Assert::IsTrue(doesPairExists("4", "x2", r));
			Assert::IsTrue(doesPairExists("4", "x3", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// STMTNO_WILDCARD
			expected = { {"4", "x1"}, {"4", "x2"}, {"4", "x3"} };
			st = SuchThatClause(make_shared<DA::UsesRel>(stmtno4, wildcard)); // should return all variables used by s4
			e = UsesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("4", "x1", r));
			Assert::IsTrue(doesPairExists("4", "x2", r));
			Assert::IsTrue(doesPairExists("4", "x3", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);


			// SYN_QUOTE
			expected = { {"5", "y"}, {"6", "y"} };
			st = SuchThatClause(make_shared<DA::UsesRel>(stmtSyn1, varname_y)); // z is a synonym, should return all using variable "y"
			e = UsesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("5", "y", r));
			Assert::IsTrue(doesPairExists("6", "y", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// SYN_SYN
			expected = { {"3","z"}, {"4","x2"}, {"4","x1"}, {"4","x3"}, {"5","y"}, {"6", "y"} };
			st = SuchThatClause(make_shared<DA::UsesRel>(stmtSyn1, varSyn1)); // z and y both are a synonym, should return all Uses
			e = UsesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("4", "x1", r));
			Assert::IsTrue(doesPairExists("4", "x2", r));
			Assert::IsTrue(doesPairExists("4", "x3", r));
			Assert::IsTrue(doesPairExists("5", "y", r));
			Assert::IsTrue(doesPairExists("6", "y", r));
			Assert::IsTrue(doesPairExists("3", "z", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// SYN_WILDCARD
			expected = { {"3","z"}, {"4","x2"}, {"4","x1"}, {"4","x3"}, {"5","y"}, {"6", "y"} };
			st = SuchThatClause(make_shared<DA::UsesRel>(stmtSyn1, wildcard)); // z is synonym, should return all Uses
			e = UsesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("4", "x1", r));
			Assert::IsTrue(doesPairExists("4", "x2", r));
			Assert::IsTrue(doesPairExists("4", "x3", r));
			Assert::IsTrue(doesPairExists("5", "y", r));
			Assert::IsTrue(doesPairExists("6", "y", r));
			Assert::IsTrue(doesPairExists("3", "z", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

		}
		
			//TEST_METHOD(Test_UsesEvaluator_evaluateClauseWithSyn)
			//{
			//	/*
			//	* Test method: ParentEvaluator::evaluateClauseWithSyn
			//	* To test for Parent relationship result table.
			//	* Should return unordered_map<string, vector<string>>
			//	*/

			//	PKBFacade* pkb = initialisePKB();
			//	// Uses pairs include{ {"3","z"}, {"4","x1"}, {"4","x2"}, {"4","x3"}, {"5","y"}, {"6", "y"} };
			//	std::unordered_map<std::string, std::vector<std::string>> expected;
			//	std::unordered_map<std::string, std::vector<std::string>> r;
			//	std::pair<std::string, std::vector<std::string>> rPair;


			//	expected = { };
			//	SuchThatClause st = SuchThatClause(make_shared<DA::UsesRel>("3", "\"x\"")); // get by arg1 and arg2
			//	r = e.evaluateClauseWithSyn(st, &getFacade);
			//	Assert::AreEqual(expected.empty(), r.empty());

			//	expected = { };
			//	st = SuchThatClause("Parent", "3", "\"z\""));
			//	// there is result, but noCommSyn, mistake that it came here, returns empty table, but can handle in different way
			//	r = e.evaluateClauseWithSyn(st, &getFacade);
			//	Assert::AreEqual(expected.empty(), r.empty());


			//	expected = { { "z", {"x1", "x2", "x3" } } };
			//	st = SuchThatClause("Parent", "4", "z")); // theres syn, should have table with z as header
			//	r = e.evaluateClauseWithSyn(st, &getFacade);
			//	Assert::AreEqual(expected.size(), r.size());
			//	if (r.find("z") != r.end()) {
			//		Assert::AreEqual(expected["z"].size(), r["z"].size());
			//		for (size_t i = 0; i < expected["z"].size(); i++) {
			//			std::string r1 = r["z"][i];
			//			std::string e1 = expected["z"][i];
			//			Assert::AreEqual(r1, e1);
			//		}
			//	}
			//	else {
			//		Assert::Fail();
			//	}

			//	expected = { };
			//	st = SuchThatClause(make_shared<DA::UsesRel>("4", "_")); // no comm syn, no table
			//	r = e.evaluateClauseWithSyn(st, &getFacade);
			//	Assert::AreEqual(expected.empty(), r.empty());

			//	expected = { {"z", { "5", "6" } } };
			//	st = SuchThatClause(make_shared<DA::UsesRel>("z", "\"y\"")); // syn z, table with z as header
			//	r = e.evaluateClauseWithSyn(st, &getFacade);
			//	Assert::AreEqual(expected.size(), r.size());
			//	if (r.find("z") != r.end()) {
			//		Assert::AreEqual(expected["z"].size(), r["z"].size());
			//		for (size_t i = 0; i < expected["z"].size(); i++) {
			//			std::string r1 = r["z"][i];
			//			std::string e1 = expected["z"][i];
			//			Assert::AreEqual(r1, e1);
			//		}
			//	}
			//	else {
			//		Assert::Fail();
			//	}

			//	expected = { {"z", { "3", "4", "4", "4", "5", "6" } },
			//				{ "y", { "z", "x1", "x2", "x3", "y", "y"}}};
			//	st = SuchThatClause(make_shared<DA::UsesRel>("z", "y")); // syn z and syn y, table with z and y as header, all pairs
			//	r = e.evaluateClauseWithSyn(st, &getFacade);
			//	Assert::AreEqual(expected.size(), r.size());
			//	if (r.find("z") != r.end()) {
			//		Assert::AreEqual(expected["z"].size(), r["z"].size());
			//		for (size_t i = 0; i < expected["z"].size(); i++) {
			//			std::string r1 = r["z"][i];
			//			std::string e1 = expected["z"][i];
			//			Assert::AreEqual(r1, e1);
			//		}
			//	}
			//	else {
			//		Assert::Fail();
			//	}
			//	if (r.find("y") != r.end()) {
			//		Assert::AreEqual(expected["y"].size(), r["y"].size());
			//		for (size_t i = 0; i < expected["y"].size(); i++) {
			//			std::string r1 = r["y"][i];
			//			std::string e1 = expected["y"][i];
			//			Assert::AreEqual(r1, e1);
			//		}
			//	}
			//	else {
			//		Assert::Fail();
			//	}

			//	expected = { {"z", { "3", "4", "4", "4", "5", "6" } } };
			//	st = SuchThatClause(make_shared<DA::UsesRel>("z", "_")); // syn z and syn y, table with z and y as header, all pairs
			//	r = e.evaluateClauseWithSyn(st, &getFacade);
			//	Assert::AreEqual(expected.size(), r.size());
			//	if (r.find("z") != r.end()) {
			//		Assert::AreEqual(expected["z"].size(), r["z"].size());
			//		for (size_t i = 0; i < expected["z"].size(); i++) {
			//			std::string r1 = r["z"][i];
			//			std::string e1 = expected["z"][i];
			//			Assert::AreEqual(r1, e1);
			//		}
			//	}
			//	else {
			//		Assert::Fail();
			//	}
			//}
	};
}