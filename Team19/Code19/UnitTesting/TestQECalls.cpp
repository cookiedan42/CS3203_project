#include "stdafx.h"
#include "../source/QP/QueryObject/SuchThatClause.h"
#include "../source/QP/QueryEvaluator/CallsEvaluator.h"
#include "../source/QP/QueryEvaluator/QueryUtil.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertCallsRequest.h"
#include <codecvt>
#include <locale>
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTesting

{
	TEST_CLASS(Test_QE_Calls_CallsT)
	{
	public:

		PKB* initialisePKB() {
			PKB* pkb = new PKB();
			InsertFacade insertFacade = InsertFacade(*pkb);
			Procedure p1("proc_name1");
			Procedure p2("proc_name2");
			Procedure p3("proc_name3");

			// please note that order of insertion matters for checking
			Calls c1(p1, p2);
			Calls c2(p2, p3);

			vector<Rel::Calls> callsList{};

			callsList.push_back(c1);
			callsList.push_back(c2);

			for (Calls a : callsList) {
				auto callsRequest = std::make_shared<InsertCallsRequest>(a);
				insertFacade.insert(callsRequest);
			}

			//after all insertion then generate star data
			insertFacade.generateSecondaryData();

			return pkb;
		}

		bool doesStarRelationExists(std::string proc_name_1, std::string proc_name_2, std::vector<std::pair<std::string, std::string>> vec) {
			std::pair<std::string, std::string> item = std::make_pair(proc_name_1, proc_name_2);

			if (find(vec.begin(), vec.end(), item) != vec.end()) {
				return true;
			}
			return false;
		}

		bool isElementInSet(std::string e, std::vector<std::string> s) {
			return find(s.begin(), s.end(), e) != s.end();
		}

		bool isEqualVec(std::vector<std::string> v1, std::vector<std::string> v2) {
			std::sort(v1.begin(), v1.end());
			std::sort(v2.begin(), v2.end());
			return v1 == v2;
		}

		// Tests for Calls relationship
		TEST_METHOD(Test_CallsEvaluator_evaluateClauseWithNoSyn)
		{
			/*
			* Test method: e.evaluateClauseWithNoSyn
			* To test for Calls relationship with no common synonym with others.
			* Should return boolean.
			*/

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);

			struct Argument proc_name_1 = { StringExp("proc_name1") };
			struct Argument proc_name_2 = { StringExp("proc_name2") };
			struct Argument proc_name_3 = { StringExp("proc_name3") };
			struct Argument proc_syn_1 = { Declaration(make_shared<DE::Procedure>(), "synName") };
			struct Argument proc_syn_2 = { Declaration(make_shared<DE::Procedure>(), "s") };
			struct Argument wildcard = { WildCard() };

			SuchThatClause st = SuchThatClause(make_shared<DA::CallsRel>(proc_name_1, proc_name_2));
			CallsEvaluator e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			bool r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(proc_name_2, proc_name_1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(proc_name_1, wildcard));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(proc_name_3, wildcard));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(proc_name_1, proc_syn_1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(wildcard, proc_name_1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(wildcard, proc_name_2));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(proc_syn_1, proc_name_1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(proc_syn_2, proc_name_2));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(proc_syn_1, proc_syn_2));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(proc_syn_1, wildcard));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(wildcard, proc_syn_2));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::CallsRel>(wildcard, wildcard));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);
		}

		TEST_METHOD(Test_CallsEvaluator_getRelationPairs)
		{
			/*
			* Test method: e.getRelationPairs
			* To test for Calls relationship pairs.
			* Should return vector<pair<string, string>>
			*/

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			struct Argument procName1 = { StringExp("proc_name1") };
			struct Argument procName2 = { StringExp("proc_name2") };
			struct Argument procName3 = { StringExp("proc_name3") };
			struct Argument procSyn1 = { Declaration(make_shared<DE::Procedure>(), "synName") };
			struct Argument procSyn2 = { Declaration(make_shared<DE::Procedure>(), "random") };
			struct Argument procSyn3 = { Declaration(make_shared<DE::Procedure>(), "r1") };
			struct Argument procSyn4 = { Declaration(make_shared<DE::Procedure>(), "2r") };
			struct Argument wildcard = { WildCard() };

			// Calls pairs include{ {"proc_name1","proc_name2"}, {"proc_name2","proc_name3"} };
			std::vector<std::pair<std::string, std::string>> expected;
			RelationshipResults r;
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;
			int expectedSize;


			expected = { {"proc_name1","proc_name2"} };
			SuchThatClause st = SuchThatClause(make_shared<DA::CallsRel>(procName1, procName2));
			CallsEvaluator e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (size_t i = 0; i < expected.size(); i++) {
				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { };
			st = SuchThatClause(make_shared<DA::CallsRel>(procName2, procName1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			expectedSize = expected.size();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"proc_name1","proc_name2"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(procName1, wildcard));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (size_t i = 0; i < expected.size(); i++) {
				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { };
			st = SuchThatClause(make_shared<DA::CallsRel>(procName3, wildcard));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			expectedSize = expected.size();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"proc_name1","proc_name2"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(procName1, procSyn1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (size_t i = 0; i < expected.size(); i++) {
				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { };
			st = SuchThatClause(make_shared<DA::CallsRel>(procName3, procSyn1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			expectedSize = expected.size();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { };
			st = SuchThatClause(make_shared<DA::CallsRel>(wildcard, procName1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			expectedSize = expected.size();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"proc_name1","proc_name2"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(wildcard, procName2));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (size_t i = 0; i < expected.size(); i++) {
				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { };
			st = SuchThatClause(make_shared<DA::CallsRel>(procSyn2, procName1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			expectedSize = expected.size();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"proc_name1","proc_name2"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(procSyn2, procName2));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (size_t i = 0; i < expected.size(); i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { {"proc_name1","proc_name2"}, {"proc_name2","proc_name3"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(procSyn3, procSyn4));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (size_t i = 0; i < expected.size(); i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { {"proc_name1","proc_name2"}, {"proc_name2","proc_name3"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(procSyn3, wildcard));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (size_t i = 0; i < expected.size(); i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { {"proc_name1","proc_name2"}, {"proc_name2","proc_name3"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(wildcard, procSyn4));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (size_t i = 0; i < expected.size(); i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { {"proc_name1","proc_name2"}, {"proc_name2","proc_name3"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(wildcard, wildcard));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (size_t i = 0; i < expected.size(); i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

		}

		TEST_METHOD(Test_CallsEvaluator_evaluateClauseWithSyn)
		{
			/*
			* Test method: e.evaluateClauseWithSyn
			* To test for Calls relationship result table.
			* Should return unordered_map<string, vector<string>>
			*/
			struct Argument procName1 = { StringExp("proc_name1") };
			struct Argument procName2 = { StringExp("proc_name2") };
			struct Argument procName3 = { StringExp("proc_name3") };
			struct Argument procSyn1 = { Declaration(make_shared<DE::Procedure>(), "synName") };
			struct Argument procSyn2 = { Declaration(make_shared<DE::Procedure>(), "random") };
			struct Argument procSyn3 = { Declaration(make_shared<DE::Procedure>(), "r1") };
			struct Argument procSyn4 = { Declaration(make_shared<DE::Procedure>(), "2r") };
			struct Argument procSyn5 = { Declaration(make_shared<DE::Procedure>(), "p") };
			struct Argument procSyn6 = { Declaration(make_shared<DE::Procedure>(), "randomSynName") };
			struct Argument wildcard = { WildCard() };

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			// Calls pairs include{ {"proc_name1","proc_name2"}, {"proc_name2","proc_name3"} };
			vector<vector<string>> expected;
			int expectedSize;
			int rowsSize;
			Table r;

			expected = { { "proc_name3"} };
			SuchThatClause st = SuchThatClause(make_shared<DA::CallsRel>(procName2, procSyn5));
			CallsEvaluator e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			expectedSize = expected.size();
			Assert::IsTrue(isEqualVec({ "p" }, r.getTableHeader()));
			vector<vector<string>> rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			int max = rows.size();
			for (int i = 0; i < max; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { };
			st = SuchThatClause(make_shared<DA::CallsRel>(procName3, procSyn5));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			expectedSize = expected.size();
			Assert::IsTrue(isEqualVec({ "p" }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			max = rows.size();
			for (int i = 0; i < max; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { };
			st = SuchThatClause(make_shared<DA::CallsRel>(procSyn6, procName1));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			expectedSize = expected.size();
			Assert::IsTrue(isEqualVec({ std::get<Declaration>(procSyn6.argumentVariant).synName }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			max = rows.size();
			for (int i = 0; i < max; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { {"proc_name2"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(procSyn2, procName3));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ std::get<Declaration>(procSyn2.argumentVariant).synName }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			max = rows.size();
			for (int i = 0; i < max; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { {"proc_name1","proc_name2"}, {"proc_name2","proc_name3"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(procSyn3, procSyn4));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ std::get<Declaration>(procSyn3.argumentVariant).synName, std::get<Declaration>(procSyn4.argumentVariant).synName }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			max = rows.size();
			for (int i = 0; i < max; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { {"proc_name1"}, {"proc_name2"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(procSyn3, wildcard));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ std::get<Declaration>(procSyn3.argumentVariant).synName }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			max = rows.size();
			for (int i = 0; i < max; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { {"proc_name2"}, {"proc_name3"} };
			st = SuchThatClause(make_shared<DA::CallsRel>(wildcard, procSyn4));
			e = CallsEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ std::get<Declaration>(procSyn4.argumentVariant).synName }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			max = rows.size();
			for (int i = 0; i < max; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}
		}
	};

}
