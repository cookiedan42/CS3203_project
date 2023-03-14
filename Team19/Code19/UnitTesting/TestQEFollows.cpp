#include "stdafx.h"
#include "../source/QP/QueryObject/SuchThatClause.h"
#include "../source/QP/QueryEvaluator/FollowsEvaluator.h"
#include "../source/QP/QueryEvaluator/FollowsTEvaluator.h" 
#include "../source/QP/QueryEvaluator/QueryUtil.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertFollowsRequest.h"
#include <codecvt>
#include <locale>
#include <string>


namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<>
			static std::wstring ToString(const enum ArgsType& type)
			{
				switch (type) {
				case ArgsType::STMTNO_STMTNO: return L"1";
				case ArgsType::STMTNO_SYN: return L"2";
				case ArgsType::STMTNO_WILDCARD: return L"3";
				case ArgsType::STMTNO_QUOTE: return L"4";
				case ArgsType::SYN_STMTNO: return L"5";
				case ArgsType::SYN_SYN: return L"6";
				case ArgsType::SYN_WILDCARD: return L"7";
				case ArgsType::SYN_QUOTE: return L"8";
				case ArgsType::WILDCARD_STMTNO: return L"9";
				case ArgsType::WILDCARD_SYN: return L"10";
				case ArgsType::WILDCARD_WILDCARD: return L"11";
				}

				return std::to_wstring(static_cast<int>(type));
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting

{
	TEST_CLASS(Test_QE_Follows_FollowsT)
	{
	public:

		PKB* initialisePKB() {
			PKB* pkb = new PKB();
			InsertFacade insertFacade = InsertFacade(*pkb);

			Statement s1(1, "print");
			Statement s2(2, "while");
			Statement s3(3, "if");
			Statement s4(4, "assignment");
			Statement s5(5, "assignment");

			// please note that order of insertion matters for checking
			Follows f1(s1, s2);
			Follows f2(s2, s3);
			Follows f3(s3, s5);
			Follows f4(s4, s5);
			Follows f5(s1, s5);

			vector<Rel::Follows> followsList{};

			followsList.push_back(f1);
			followsList.push_back(f2);
			followsList.push_back(f3);
			followsList.push_back(f4);
			followsList.push_back(f5);

			for (Follows a : followsList) {
				auto followsRequest = std::make_shared<InsertFollowsRequest>(a);
				insertFacade.insert(followsRequest);
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
		// Tests for follows relationship
		TEST_METHOD(Test_FollowsEvaluator_evaluateClauseWithNoSyn)
		{
			/*
			* Test method: e.evaluateClauseWithNoSyn
			* To test for Follows relationship with no common synonym with others.
			* Should return boolean.
			*/
			struct Argument stmtno1 = { StmtNo("1") };
			struct Argument stmtno2 = { StmtNo("2") };
			struct Argument stmtno5 = { StmtNo("5") };
			struct Argument stmtSyn1 = { Declaration(make_shared<DE::Statement>(), "synName") };
			struct Argument stmtSyn2 = { Declaration(make_shared<DE::Statement>(), "s") };
			struct Argument stmtSyn3 = { Declaration(make_shared<DE::Statement>(), "s1") };
			struct Argument stmtSyn4 = { Declaration(make_shared<DE::Statement>(), "s2") };
			struct Argument wildcard = { WildCard() };

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);

			SuchThatClause st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno1, stmtno2));
			FollowsEvaluator e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			bool r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno2, stmtno1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno1, wildcard));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno5, wildcard));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno1, stmtSyn1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno5, stmtSyn1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(wildcard, stmtno1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(wildcard, stmtno2));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn1, stmtno1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn2, stmtno2));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn3, stmtSyn4));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn3, wildcard));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(wildcard, stmtSyn4));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::FollowsRel>(wildcard, wildcard));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);
		}

		TEST_METHOD(Test_FollowsEvaluator_getRelationPairs)
		{
			/*
			* Test method: e.getRelationPairs
			* To test for Follows relationship pairs.
			* Should return vector<pair<string, string>>
			*/
			struct Argument stmtno1 = { StmtNo("1") };
			struct Argument stmtno2 = { StmtNo("2") };
			struct Argument stmtno5 = { StmtNo("5") };
			struct Argument stmtSyn1 = { Declaration(make_shared<DE::Statement>(), "synName") };
			struct Argument stmtSyn2 = { Declaration(make_shared<DE::Statement>(), "s") };
			struct Argument stmtSyn3 = { Declaration(make_shared<DE::Statement>(), "s1") };
			struct Argument stmtSyn4 = { Declaration(make_shared<DE::Statement>(), "s2") };
			struct Argument stmtSyn5 = { Declaration(make_shared<DE::Statement>(), "random") };
			struct Argument stmtSyn6 = { Declaration(make_shared<DE::Statement>(), "r1") };
			struct Argument stmtSyn7 = { Declaration(make_shared<DE::Statement>(), "2r") };
			struct Argument wildcard = { WildCard() };

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			// Follows pairs include{ {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"} };
			vector<vector<string>> expected;
			RelationshipResults r;
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;


			expected = { {"1","2"} };
			SuchThatClause st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno1, stmtno2));
			FollowsEvaluator e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesStarRelationExists("1", "2", r));

			expected = { };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno2, stmtno1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"1","2"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno1, wildcard));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesStarRelationExists("1", "2", r));
			Assert::IsTrue(doesStarRelationExists("1", "5", r));


			expected = { };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno5, stmtSyn1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { };
			st = SuchThatClause(make_shared<DA::FollowsRel>(wildcard, stmtno1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"3","5"}, {"1", "5"}, {"4", "5"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(wildcard, stmtno5));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesStarRelationExists("3", "5", r));
			Assert::IsTrue(doesStarRelationExists("1", "5", r));
			Assert::IsTrue(doesStarRelationExists("4", "5", r));

			expected = { };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn5, stmtno1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"3","5"}, {"1", "5"}, {"4", "5"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn5, stmtno5));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesStarRelationExists("3", "5", r));
			Assert::IsTrue(doesStarRelationExists("1", "5", r));
			Assert::IsTrue(doesStarRelationExists("4", "5", r));

			expected = { {"1","2"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn6, stmtSyn7));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesStarRelationExists("1", "2", r));
			Assert::IsTrue(doesStarRelationExists("2", "3", r));
			Assert::IsTrue(doesStarRelationExists("3", "5", r));
			Assert::IsTrue(doesStarRelationExists("1", "5", r));
			Assert::IsTrue(doesStarRelationExists("4", "5", r));

			expected = { {"1","2"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn6, wildcard));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesStarRelationExists("1", "2", r));
			Assert::IsTrue(doesStarRelationExists("2", "3", r));
			Assert::IsTrue(doesStarRelationExists("3", "5", r));
			Assert::IsTrue(doesStarRelationExists("1", "5", r));
			Assert::IsTrue(doesStarRelationExists("4", "5", r));

			expected = { {"1","2"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(wildcard, stmtSyn7));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesStarRelationExists("1", "2", r));
			Assert::IsTrue(doesStarRelationExists("2", "3", r));
			Assert::IsTrue(doesStarRelationExists("3", "5", r));
			Assert::IsTrue(doesStarRelationExists("1", "5", r));
			Assert::IsTrue(doesStarRelationExists("4", "5", r));

			expected = { {"1","2"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","5"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(wildcard, wildcard));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesStarRelationExists("1", "2", r));
			Assert::IsTrue(doesStarRelationExists("2", "3", r));
			Assert::IsTrue(doesStarRelationExists("3", "5", r));
			Assert::IsTrue(doesStarRelationExists("1", "5", r));
			Assert::IsTrue(doesStarRelationExists("4", "5", r));

		}

		TEST_METHOD(Test_FollowsEvaluator_evaluateClauseWithSyn)
		{
			/*
			* Test method: e.evaluateClauseWithSyn
			* To test for Follows relationship result table.
			* Should return unordered_map<string, vector<string>>
			*/
			struct Argument stmtno1 = { StmtNo("1") };
			struct Argument stmtno2 = { StmtNo("2") };
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
			std::string header1 = std::get<Declaration>(stmtSyn1.argumentVariant).synName;
			std::string header2 = std::get<Declaration>(stmtSyn2.argumentVariant).synName;
			std::string header3 = std::get<Declaration>(stmtSyn3.argumentVariant).synName;
			std::string header4 = std::get<Declaration>(stmtSyn4.argumentVariant).synName;
			std::string header5 = std::get<Declaration>(stmtSyn5.argumentVariant).synName;
			std::string header6 = std::get<Declaration>(stmtSyn6.argumentVariant).synName;
			std::string header7 = std::get<Declaration>(stmtSyn7.argumentVariant).synName;
			std::string header8 = std::get<Declaration>(stmtSyn8.argumentVariant).synName;

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			// Follows pairs include{ {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"} };
			vector<vector<string>> expected;
			Table r;

			expected = { { "2" }, {"5"} };
			SuchThatClause st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno1, stmtSyn1));
			FollowsEvaluator e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({header1}, r.getTableHeader()));
			vector<vector<string>> rows = r.getTableRows();
			Assert::AreEqual(expected.size(), rows.size());
			Assert::IsTrue(checkRowsEqualExpected(rows, expected));

			expected = { };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtno5, stmtSyn1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({header1}, r.getTableHeader()));
			rows = r.getTableRows();
			Assert::AreEqual(expected.size(), rows.size());
	

			expected = { };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn8, stmtno1));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({header8}, r.getTableHeader()));
			rows = r.getTableRows();
			Assert::AreEqual(expected.size(), rows.size());	


			expected = { { "3"}, {"4"}, {"1"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn5, stmtno5));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header5 }, r.getTableHeader()));
			rows = r.getTableRows();
			Assert::AreEqual(expected.size(), rows.size());
			Assert::IsTrue(checkRowsEqualExpected(rows, expected));

			expected = { {"1","2"}, {"2","3"}, {"3","5"}, {"1","5"}, {"4","5"}};
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn6, stmtSyn7));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header6, header7 }, r.getTableHeader()));
			rows = r.getTableRows();
			Assert::AreEqual(expected.size(), rows.size());
			Assert::IsTrue(checkRowsEqualExpected(rows, expected));

			expected = { { "1"}, {"2"}, {"3"}, {"1"}, {"4"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(stmtSyn6, wildcard));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header6 }, r.getTableHeader()));
			rows = r.getTableRows();
			Assert::AreEqual(expected.size(), rows.size());
			Assert::IsTrue(checkRowsEqualExpected(rows, expected));

			expected = { {"2"}, {"3"}, {"5"}, {"5"}, {"5"} };
			st = SuchThatClause(make_shared<DA::FollowsRel>(wildcard, stmtSyn7));
			e = FollowsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header7 }, r.getTableHeader()));
			rows = r.getTableRows();
			Assert::AreEqual(expected.size(), rows.size());
			Assert::IsTrue(checkRowsEqualExpected(rows, expected));
		}


		// Tests for transitive follows relationship Follows*
		TEST_METHOD(Test_FollowsTEvaluator_getRelationPairs_evaluateClauseWithNoSyn)
		{
			/*
			* Test method: e.evaluateClauseWithNoSyn and getRelationPairs
			* To test for Follows relationship with no common synonym with others.
			* Should return boolean.
			*/

			// Follows* pairs include{ {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","3"}, {"2", "5"} };
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
			SuchThatClause st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtno1, stmtno3));
			FollowsTEvaluator e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = { {"1","2"} };
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtno1, stmtno2));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), false);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtno2, stmtno1));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			expected = { {"1","2"}, {"1", "5"}, {"1", "3"} };
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtno1, wildcard));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("4", "5", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtno5, wildcard));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			expected = { {"1","2"}, {"1", "5"}, {"1", "3"} };
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtno1, stmtSyn1));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "5", r), true);
			Assert::AreEqual(doesStarRelationExists("4", "5", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtno5, stmtSyn1));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::FollowsTRel>(wildcard, stmtno1));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			expected = { {"1", "3"}, {"2", "3"}};
			st = SuchThatClause(make_shared<DA::FollowsTRel>(wildcard, stmtno3));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), true);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), false);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = {};
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtSyn1, stmtno1));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);


			expected = { {"1", "2"} };
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtSyn2, stmtno2));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::AreEqual(doesStarRelationExists("1", "3", r), false);
			Assert::AreEqual(doesStarRelationExists("1", "2", r), true);
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			expected = { {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","3"}, {"2", "5"} };
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtSyn3, stmtSyn4));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
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
			st = SuchThatClause(make_shared<DA::FollowsTRel>(stmtSyn3, wildcard));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
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
			st = SuchThatClause(make_shared<DA::FollowsTRel>(wildcard, stmtSyn4));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
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
			st = SuchThatClause(make_shared<DA::FollowsTRel>(wildcard, wildcard));
			e = FollowsTEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
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

		//TEST_METHOD(Test_FollowsTEvaluator_evaluateClauseWithSyn)
		//{
		//	/*
		//	* Test method: e.evaluateClauseWithSyn
		//	* To test for Follows* relationship result table.
		//	* Should return unordered_map<string, vector<string>>
		//	*/

		//	PKBFacade* pkb = initialisePKB();
		//	// Follows* pairs include{ {"1","2"}, {"1","5"}, {"2","3"}, {"3","5"}, {"4","5"}, {"1","3"}, {"2", "5"} };
		//	vector<vector<string>> expected;
		//	Table r;


		//	expected = {};
		//	SuchThatClause st = SuchThatClause(make_shared<DA::FollowsTRel>("1", "3"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({}, r.getTableHeader()));
		//	vector<vector<string>> rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}

		//	expected = {};
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("1", "2"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({}, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}

		//	expected = {};
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("2", "1"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({}, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}

		//	expected = {};
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("1", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({}, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}
		//	expected = {};
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("5", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({}, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}
		//	expected = { {"5"}, {"2"}, {"3"} };
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("1", "synName"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({ "synName" }, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}

		//	expected = {};
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("5", "synName"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({ "synName" }, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}

		//	expected = {};
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("randomSynName", "1"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({ "randomSynName" }, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}

		//	expected = {};
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("_", "1"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({}, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}
		//	expected = {};
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("_", "5"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({}, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}

		//	/*expected = { {"2"}, {"3"}, {"1"}, {"4"} };
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("random", "5"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({ "random" }, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}*/

		//	expected = { {"1", "2"}, {"1", "5"}, {"2", "3"}, {"3", "5"}, {"4", "5"}, {"1", "3"}, {"2", "5"} };
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("r1", "2r"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({ "r1", "2r" }, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	/*for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}*/

		//	expected = { {"1"}, {"1"} ,{"2"} ,{"3"} ,{"4"} ,{"1"} ,{"2"} };
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("r1", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({ "r1" }, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}

		//	expected = { {"1"}, {"1"} ,{"2"} ,{"3"} ,{"4"} ,{"1"} ,{"2"} };
		//	st = SuchThatClause(make_shared<DA::FollowsTRel>("_", "_"));
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::IsTrue(isEqualVec({ "r1" }, r.getTableHeader()));
		//	rows = r.getTableRows();
		//	Assert::AreEqual(expected.size(), rows.size());
		//	for (size_t i = 0; i < rows.size(); i++) {
		//		Assert::IsTrue(isEqualVec(expected[i], rows[i]));
		//	}
		//}
	};
}