#include "stdafx.h"
#include "../source/QP/QueryObject/SuchThatClause.h"
#include "../source/QP/QueryEvaluator/ParentEvaluator.h"
#include "../source/QP/QueryEvaluator/ParentTEvaluator.h"
#include "../source/QP/QueryEvaluator/QueryUtil.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertParentRequest.h"
#include <codecvt>
#include <locale>
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting

{
	TEST_CLASS(Test_QE_Parent)
	{
	public:

		PKB* initialisePKB() {
			PKB* pkb = new PKB();
			InsertFacade insertFacade = InsertFacade(*pkb);

			Statement s1(1, "print");
			Statement s2(2, "while");
			Statement s3(3, "if");
			Statement s4(4, "assign");
			Statement s5(5, "assign");

			// please note that order of insertion matters for checking
			Parent f1(s1, s2);
			Parent f2(s2, s3);
			Parent f3(s3, s5);
			Parent f4(s4, s5);
			Parent f5(s1, s5);

			vector<Rel::Parent> parentList{};

			parentList.push_back(f1);
			parentList.push_back(f2);
			parentList.push_back(f3);
			parentList.push_back(f4);
			parentList.push_back(f5);

			for (Parent a : parentList) {
				auto parentRequest = std::make_shared<InsertParentRequest>(a);
				insertFacade.insert(parentRequest);
			}


			//after all insertion then generate star data
			insertFacade.generateSecondaryData();

			return pkb;
		}

		bool doesStarRelationExists(std::string arg1, std::string arg2, RelationshipResults results) {
			std::pair<std::string, std::string> item = std::make_pair(arg1, arg2);

			if (find(results.begin(), results.end(), item) != results.end()) {
				return true;
			}
			return false;
		}

		bool doesPairExists(std::string arg1, std::string arg2, RelationshipResults results) {
			std::pair<std::string, std::string> item = std::make_pair(arg1, arg2);

			if (find(results.begin(), results.end(), item) != results.end()) {
				return true;
			}
			return false;
		}

		bool isElementInSet(std::string e, std::vector<std::string> s) {
			return find(s.begin(), s.end(), e) != s.end();
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

		// Tests parent relationship
		TEST_METHOD(Test_ParentEvaluator_evaluateClauseWithNoSyn)
		{
			/*
			* Test method: e.evaluateClauseWithNoSyn
			* To test for Parent relationship with no common synonym with others.
			* Should return boolean.
			*/
			struct Argument stmtno1 = { StmtNo("1") };
			struct Argument stmtno2 = { StmtNo("2") };
			struct Argument stmtno3 = { StmtNo("3") };
			struct Argument stmtno4 = { StmtNo("4") };
			struct Argument stmtno5 = { StmtNo("5") };
			struct Argument stmtSyn1 = { Declaration(make_shared<DE::Statement>(), "synName") };
			struct Argument stmtSyn2 = { Declaration(make_shared<DE::Statement>(), "s") };
			struct Argument stmtSyn3 = { Declaration(make_shared<DE::Statement>(), "s1") };
			struct Argument stmtSyn4 = { Declaration(make_shared<DE::Statement>(), "s2") };
			struct Argument stmtSyn5 = { Declaration(make_shared<DE::Statement>(), "random") };
			struct Argument stmtSyn6 = { Declaration(make_shared<DE::Statement>(), "r1") };
			struct Argument stmtSyn7 = { Declaration(make_shared<DE::Statement>(), "2r") };
			struct Argument stmtSyn8 = { Declaration(make_shared<DE::Statement>(), "randomSynName") };
			struct Argument wildcard = { WildCard() };

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);

			SuchThatClause st = SuchThatClause(make_shared<DA::ParentRel>(stmtno1, stmtno2));
			ParentEvaluator e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			bool r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno2, stmtno1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno1, wildcard));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno5, wildcard));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno1, stmtSyn1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno5, stmtSyn1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(wildcard, stmtno1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(wildcard, stmtno2));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(stmtSyn1, stmtno1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(stmtSyn2, stmtno2));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(stmtSyn3, stmtSyn4));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(stmtSyn3, wildcard));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(wildcard, stmtSyn4));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::ParentRel>(wildcard, wildcard));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);
		}

		TEST_METHOD(Test_ParentEvaluator_getRelationPairs)
		{
			/*
			* Test method: e.getRelationPairs
			* To test for Parent relationship pairs.
			* Should return vector<pair<string, string>>
			*/
			struct Argument stmtno1 = { StmtNo("1") };
			struct Argument stmtno2 = { StmtNo("2") };
			struct Argument stmtno3 = { StmtNo("3") };
			struct Argument stmtno4 = { StmtNo("4") };
			struct Argument stmtno5 = { StmtNo("5") };
			struct Argument stmtSyn1 = { Declaration(make_shared<DE::Statement>(), "synName") };
			struct Argument stmtSyn2 = { Declaration(make_shared<DE::Statement>(), "s") };
			struct Argument stmtSyn3 = { Declaration(make_shared<DE::Statement>(), "s1") };
			struct Argument stmtSyn4 = { Declaration(make_shared<DE::Statement>(), "s2") };
			struct Argument stmtSyn5 = { Declaration(make_shared<DE::Statement>(), "random") };
			struct Argument stmtSyn6 = { Declaration(make_shared<DE::Statement>(), "r1") };
			struct Argument stmtSyn7 = { Declaration(make_shared<DE::Statement>(), "2r") };
			struct Argument stmtSyn8 = { Declaration(make_shared<DE::Statement>(), "randomSynName") };
			struct Argument wildcard = { WildCard() };

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			// Parent pairs include{ {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"} };
			std::vector<std::pair<std::string, std::string>> expected;
			RelationshipResults r;
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;


			expected = { {"1","2"} };
			SuchThatClause st = SuchThatClause(make_shared<DA::ParentRel>(stmtno1, stmtno2));
			ParentEvaluator e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("1", "2", r));

			expected = { };
			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno2, stmtno1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"1","2"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno1, wildcard));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("1", "2", r));
			Assert::IsTrue(doesPairExists("1", "5", r));

			expected = { };
			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno5, wildcard));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"1","2"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno1, stmtSyn1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("1", "2", r));
			Assert::IsTrue(doesPairExists("1", "5", r));

			expected = { };
			st = SuchThatClause(make_shared<DA::ParentRel>(stmtno5, stmtSyn1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { };
			st = SuchThatClause(make_shared<DA::ParentRel>(wildcard, stmtno1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"3","5"}, {"1", "5"}, {"4", "5"} };
			st = SuchThatClause(make_shared<DA::ParentRel>(wildcard, stmtno5));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("3", "5", r));
			Assert::IsTrue(doesPairExists("1", "5", r));
			Assert::IsTrue(doesPairExists("4", "5", r));

			expected = { };
			st = SuchThatClause(make_shared<DA::ParentRel>(stmtSyn2, stmtno1));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"3","5"}, {"1", "5"}, {"4", "5"} };
			st = SuchThatClause(make_shared<DA::ParentRel>(stmtSyn2, stmtno5));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("3", "5", r));
			Assert::IsTrue(doesPairExists("1", "5", r));
			Assert::IsTrue(doesPairExists("4", "5", r));

			expected = { {"1","2"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::ParentRel>(stmtSyn6, stmtSyn7));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("1", "2", r));
			Assert::IsTrue(doesPairExists("2", "3", r));
			Assert::IsTrue(doesPairExists("3", "5", r));
			Assert::IsTrue(doesPairExists("1", "5", r));
			Assert::IsTrue(doesPairExists("4", "5", r));

			expected = { {"1","2"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::ParentRel>(stmtSyn6, wildcard));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("1", "2", r));
			Assert::IsTrue(doesPairExists("2", "3", r));
			Assert::IsTrue(doesPairExists("3", "5", r));
			Assert::IsTrue(doesPairExists("1", "5", r));
			Assert::IsTrue(doesPairExists("4", "5", r));

			expected = { {"1","2"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::ParentRel>(wildcard, stmtSyn7));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("1", "2", r));
			Assert::IsTrue(doesPairExists("2", "3", r));
			Assert::IsTrue(doesPairExists("3", "5", r));
			Assert::IsTrue(doesPairExists("1", "5", r));
			Assert::IsTrue(doesPairExists("4", "5", r));

			expected = { {"1","2"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::ParentRel>(wildcard, wildcard));
			e = ParentEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("1", "2", r));
			Assert::IsTrue(doesPairExists("2", "3", r));
			Assert::IsTrue(doesPairExists("3", "5", r));
			Assert::IsTrue(doesPairExists("1", "5", r));
			Assert::IsTrue(doesPairExists("4", "5", r));

		}

		//TEST_METHOD(Test_ParentEvaluator_evaluateClauseWithSyn)
		//{
		//	/*
		//	* Test method: e.evaluateClauseWithSyn
		//	* To test for Parent relationship result table.
		//	* Should return unordered_map<string, vector<string>>
		//	*/

		//	PKBFacade* pkb = initialisePKB();
		//	// Parent pairs include{ {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"} };
		//	std::unordered_map<std::string, std::vector<std::string>> expected;
		//	std::unordered_map<std::string, std::vector<std::string>> r;
		//	std::pair<std::string, std::vector<std::string>> rPair;


		//	expected = { };
		//	SuchThatClause st = SuchThatClause(make_shared<DA::ParentRel>("1", "2"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("2", "1"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());


		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("1", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("5", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());


		//	expected = { { "synName", { "2", "5" } }, };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("1", "synName"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("synName") != r.end()) {
		//		for (size_t i = 0; i < expected["synName"].size(); i++) {
		//			std::string r1 = r["synName"][i];
		//			std::string e1 = expected["synName"][i];
		//			Assert::AreEqual(r1, e1);
		//		}
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("5", "synName"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(expected.empty());
		//	Assert::IsFalse(r.empty());
		//	Assert::IsTrue(r["synName"].empty());
		//	/*Assert::AreEqual(expected.empty(), r.empty());*/

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("randomSynName", "1"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(expected.empty());
		//	Assert::IsFalse(r.empty());
		//	Assert::IsTrue(r["randomSynName"].empty());

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("_", "1"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("_", "5"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());


		//	expected = { { "random", { "3", "4", "1" } }, };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("random", "5"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("random") != r.end()) {
		//		for (size_t i = 0; i < expected["random"].size(); i++) {
		//			std::string r1 = r["random"][i];
		//			std::string e1 = expected["random"][i];
		//			Assert::AreEqual(r1, e1);
		//		}
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { { "r1", { "1", "1", "2", "3", "4" } }, {"2r", {"2", "5", "3", "5", "5"} } };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("r1", "2r"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("r1") != r.end()) {
		//		for (size_t i = 0; i < expected["r1"].size(); i++) {
		//			std::string r1 = r["r1"][i];
		//			std::string e1 = expected["r1"][i];
		//			Assert::AreEqual(r1, e1);
		//		}
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//	if (r.find("2r") != r.end()) {
		//		for (size_t i = 0; i < expected["2r"].size(); i++) {
		//			std::string r1 = r["2r"][i];
		//			std::string e1 = expected["2r"][i];
		//			Assert::AreEqual(r1, e1);
		//		}
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { { "r1", { "1", "1", "2", "3", "4" } } };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("r1", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("r1") != r.end()) {
		//		for (size_t i = 0; i < expected["r1"].size(); i++) {
		//			std::string r1 = r["r1"][i];
		//			std::string e1 = expected["r1"][i];
		//			Assert::AreEqual(r1, e1);
		//		}
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { {"2r", {"2", "5", "3", "5", "5"} } };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("_", "2r"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("2r") != r.end()) {
		//		Assert::AreEqual(expected["2r"].size(), r["2r"].size());
		//		for (size_t i = 0; i < expected["2r"].size(); i++) {
		//			std::string r1 = r["2r"][i];
		//			std::string e1 = expected["2r"][i];
		//			Assert::AreEqual(r1, e1);
		//		}
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentRel>("_", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//}


		// Tests for transitive parent relationship Parent*
		TEST_METHOD(Test_ParentTEvaluator_getRelationPairs_evaluateClauseWithNoSyn)
		{
			/*
			* Test method: e.evaluateClauseWithNoSyn and getRelationPairs
			* To test for Parent relationship with no common synonym with others.
			* Should return boolean.
			*/

			// Parent* pairs include{ {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","3"}, {"2", "5"} };
			struct Argument stmtno1 = { StmtNo("1") };
			struct Argument stmtno2 = { StmtNo("2") };
			struct Argument stmtno3 = { StmtNo("3") };
			struct Argument stmtno4 = { StmtNo("4") };
			struct Argument stmtno5 = { StmtNo("5") };
			struct Argument stmtSyn1 = { Declaration(make_shared<DE::Statement>(), "synName") };
			struct Argument stmtSyn2 = { Declaration(make_shared<DE::Statement>(), "s") };
			struct Argument stmtSyn3 = { Declaration(make_shared<DE::Statement>(), "s1") };
			struct Argument stmtSyn4 = { Declaration(make_shared<DE::Statement>(), "s2") };
			struct Argument stmtSyn5 = { Declaration(make_shared<DE::Statement>(), "random") };
			struct Argument stmtSyn6 = { Declaration(make_shared<DE::Statement>(), "r1") };
			struct Argument stmtSyn7 = { Declaration(make_shared<DE::Statement>(), "2r") };
			struct Argument stmtSyn8 = { Declaration(make_shared<DE::Statement>(), "randomSynName") };
			struct Argument wildcard = { WildCard() };

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			std::vector<std::pair<std::string, std::string>> expected;
			RelationshipResults r; // result from getRelationPairs
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;
			bool res; // result from evaluateClauseWithNoSyn


			expected = { {"1","3"} };
			SuchThatClause st = SuchThatClause(make_shared<DA::ParentTRel>(stmtno1, stmtno3));
			ParentTEvaluator e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = { {"1","2"} };
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtno1, stmtno2));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), false);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtno2, stmtno1));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			expected = { {"1","2"}, {"1", "5"}, {"1", "3"} };
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtno1, wildcard));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("4", "5", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtno5, wildcard));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			expected = { {"1","2"}, {"1", "5"}, {"1", "3"} };
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtno1, stmtSyn1));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("4", "5", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtno5, stmtSyn1));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::ParentTRel>(wildcard, stmtno1));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			expected = { {"1", "3"}, {"2", "3"} };
			st = SuchThatClause(make_shared<DA::ParentTRel>(wildcard, stmtno3));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtSyn1, stmtno1));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);


			expected = { {"1", "2"} };
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtSyn2, stmtno2));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), false);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = { {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","3"}, {"2", "5"} };
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtSyn3, stmtSyn4));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("2", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("3", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("4", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("2", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "4", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = { {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","3"}, {"2", "5"} };
			st = SuchThatClause(make_shared<DA::ParentTRel>(stmtSyn3, wildcard));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("2", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("3", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("4", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("2", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "4", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = { {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","3"}, {"2", "5"} };
			st = SuchThatClause(make_shared<DA::ParentTRel>(wildcard, stmtSyn4));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("2", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("3", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("4", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("2", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "4", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = { {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","3"}, {"2", "5"} };
			st = SuchThatClause(make_shared<DA::ParentTRel>(wildcard, wildcard));
			e = ParentTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("2", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("3", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("4", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("2", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "4", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

		}

		//TEST_METHOD(Test_ParentTEvaluator_evaluateClauseWithSyn)
		//{
		//	/*
		//	* Test method: e.evaluateClauseWithSyn
		//	* To test for Parent* relationship result table.
		//	* Should return unordered_map<string, vector<string>>
		//	*/

		//	PKBFacade* pkb = initialisePKB();
		//	// Parent* pairs include{ {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","3"}, {"2", "5"} };
		//	std::unordered_map<std::string, std::vector<std::string>> expected;
		//	std::unordered_map<std::string, std::vector<std::string>> r;
		//	std::pair<std::string, std::vector<std::string>> rPair;


		//	expected = { };
		//	SuchThatClause st = SuchThatClause(make_shared<DA::ParentTRel>("1", "3")); // has result but no comm syn
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("1", "2")); // no result no comm syn
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("2", "1")); // no result
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());


		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("1", "_")); // has result no comm syn
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("5", "_")); // no result no comm syn
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());


		//	expected = { { "synName", { "5", "2", "3"}}, };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("1", "synName")); // comm syn synName
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("synName") != r.end()) {
		//		for (size_t i = 0; i < expected["synName"].size(); i++) {
		//			std::string r1 = r["synName"][i];
		//			std::string e1 = expected["synName"][i];
		//			Assert::AreEqual(r1, e1);
		//		}
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("5", "synName"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(expected.empty());
		//	Assert::IsFalse(r.empty());
		//	Assert::IsTrue(r["synName"].empty());
		//	/*Assert::AreEqual(expected.empty(), r.empty());*/

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("randomSynName", "1"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(expected.empty());
		//	Assert::IsFalse(r.empty());
		//	Assert::IsTrue(r["randomSynName"].empty());

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("_", "1"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("_", "5"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());


		//	expected = { { "random", { "2", "3", "1", "4"}}, };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("random", "5"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("random") != r.end()) {
		//		for (size_t i = 0; i < expected["random"].size(); i++) {
		//			Assert::IsTrue(isElementInSet(r["random"][i], expected["random"]));
		//		}
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { { "r1", { "1", "1", "2", "3", "4", "1", "2"}}, {"2r", {"2", "5", "3", "5", "5", "3", "5"}} };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("r1", "2r"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());

		//	if (r.find("r1") != r.end()) {

		//		Assert::AreEqual(isEqualVec(expected["r1"], r["r1"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}
		//	if (r.find("2r") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["2r"], r["2r"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { { "r1", { "1", "1", "2", "3", "4", "1", "2"} } };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("r1", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("r1") != r.end()) {
		//		Assert::AreEqual(isEqualVec(expected["r1"], r["r1"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { {"2r", {"2", "5", "3", "5", "5", "3", "5"} } };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("_", "2r"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.size(), r.size());
		//	if (r.find("2r") != r.end()) {
		//		Assert::AreEqual(expected["2r"].size(), r["2r"].size());
		//		Assert::AreEqual(isEqualVec(expected["2r"], r["2r"]), true);
		//	}
		//	else {
		//		Assert::Fail();
		//	}

		//	expected = { };
		//	st = SuchThatClause(make_shared<DA::ParentTRel>("_", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//}
	};
}