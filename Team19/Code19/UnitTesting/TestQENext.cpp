#include "stdafx.h"
#include "../source/QP/QueryObject/SuchThatClause.h"
#include "../source/QP/QueryEvaluator/NextEvaluator.h"
#include "../source/QP/QueryEvaluator/QueryUtil.h"
#include "../source/PKB/Storage/RelationshipStorages/NextStorage.h"
#include "../source/PKB/Storage/RelationshipStorage.h"
#include "../source/Relationship/Relationship.h"
#include "../source/Entity/Entity.h"
#include "../source/TypeAlias/TypeAlias.h"
#include "../source/Cfg/Cfg.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/CfgExtractor.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include <codecvt>
#include <locale>
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTesting

{
	TEST_CLASS(Test_QE_Next_NextT)
	{
	public:
		std::vector<Cfg> makeCfg(std::string inProg) {
			auto tokens = TokenSplitter(inProg).tokenize();
			auto AST = Parser(tokens).parseTokens();
			return  CfgExtractor().extractCfg(AST);
		}

		std::vector<Cfg> simpleProgram() {
			auto inProgram =
				"procedure proc {"
				"	read a;"	    // 1
				"   read b;"	    // 2
				"	read c;"		// 3
				"}";
			return makeCfg(inProgram);
		}

		PKB* initialisePKB() {
			PKB* pkb = new PKB();
			InsertFacade insertFacade = InsertFacade(*pkb);

			std::vector<Cfg> cfgs = simpleProgram();

			for (const auto& cfg : cfgs) {
				insertFacade.insertCFG(cfg);
			}

			insertFacade.generateSecondaryData(); // Next(1,2), Next(2,3)

			return pkb;
		}

		bool doesStarRelationExists(std::string arg1, std::string arg2, std::vector<std::pair<std::string, std::string>> vec) {
			std::pair<std::string, std::string> item = std::make_pair(arg1, arg2);

			if (find(vec.begin(), vec.end(), item) != vec.end()) {
				return true;
			}
			return false;
		}

		bool doesPairExist(std::string arg1, std::string arg2, stmtNoPtnRefPairSet vec) {
			std::pair<std::string, std::string> item = std::make_pair(arg1, arg2);

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

		// Tests for Next relationship
		TEST_METHOD(Test_NextEvaluator_evaluateClauseWithNoSyn)
		{
			/*
			* Test method: e.evaluateClauseWithNoSyn
			* To test for Next relationship with no common synonym with others.
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

			SuchThatClause st = SuchThatClause(make_shared<DA::NextRel>(stmtno1, stmtno2));
			NextEvaluator e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			bool r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::NextRel>(stmtno2, stmtno1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::NextRel>(stmtno1, wildcard));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::NextRel>(stmtno3, wildcard));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::NextRel>(stmtno1, stmtSyn1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::NextRel>(wildcard, stmtno1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::NextRel>(wildcard, stmtno2));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn1, stmtno1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(r);

			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn2, stmtno2));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn3, stmtSyn4));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn3, wildcard));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::NextRel>(wildcard, stmtSyn4));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);

			st = SuchThatClause(make_shared<DA::NextRel>(wildcard, wildcard));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(r);
		}

		TEST_METHOD(Test_NextEvaluator_getRelationPairs)
		{
			/*
			* Test method: e.getRelationPairs
			* To test for Next relationship pairs.
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
			// Next pairs include{ {"1","2"}, {"2","3"} };
			std::vector<std::pair<std::string, std::string>> expected;
			RelationshipResults r;
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;
			int expectedSize;


			expected = { {"1","2"} };
			expectedSize = expected.size();
			SuchThatClause st = SuchThatClause(make_shared<DA::NextRel>(stmtno1, stmtno2));
			NextEvaluator e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (int i = 0; i < expectedSize; i++) {
				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtno2, stmtno1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"1","2"} };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtno1, wildcard));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (int i = 0; i < expectedSize; i++) {
				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtno3, wildcard));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"1","2"} };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtno1, stmtSyn1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (int i = 0; i < expectedSize; i++) {
				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtno3, stmtSyn1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(wildcard, stmtno1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"1","2"} };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(wildcard, stmtno2));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (int i = 0; i < expectedSize; i++) {
				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn5, stmtno1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());


			expected = { {"1","2"} };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn5, stmtno2));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (int i = 0; i < expectedSize; i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { {"1","2"}, {"2","3"} };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn3, stmtSyn4));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (int i = 0; i < expectedSize; i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { {"1","2"}, {"2","3"} };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn3, wildcard));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (int i = 0; i < expectedSize; i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { {"1","2"}, {"2","3"} };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn3, stmtSyn4));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (int i = 0; i < expectedSize; i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

			expected = { {"1","2"}, {"2","3"} };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(wildcard, wildcard));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			for (int i = 0; i < expectedSize; i++) {
				rPair = *std::next(r.begin(), i);				rPair = *std::next(r.begin(), i);
				expectedPair = expected[i];
				Assert::AreEqual(rPair.first, expectedPair.first);
				Assert::AreEqual(rPair.second, expectedPair.second);
			}

		}

		TEST_METHOD(Test_NextEvaluator_evaluateClauseWithSyn)
		{
			/*
			* Test method: e.evaluateClauseWithSyn
			* To test for Next relationship result table.
			* Should return unordered_map<string, vector<string>>
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
			std::string header1 = std::get<Declaration>(stmtSyn1.argumentVariant).synName;
			std::string header2 = std::get<Declaration>(stmtSyn2.argumentVariant).synName;
			std::string header3 = std::get<Declaration>(stmtSyn3.argumentVariant).synName;
			std::string header4 = std::get<Declaration>(stmtSyn4.argumentVariant).synName;
			std::string header5 = std::get<Declaration>(stmtSyn5.argumentVariant).synName;
			std::string header6 = std::get<Declaration>(stmtSyn6.argumentVariant).synName;
			std::string header7 = std::get<Declaration>(stmtSyn7.argumentVariant).synName;
			std::string header8 = std::get<Declaration>(stmtSyn8.argumentVariant).synName;
			struct Argument wildcard = { WildCard() };

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			// Next pairs include{ {"1","2"}, {"2","3"} };
			vector<vector<string>> expected;
			int expectedSize;
			int rowsSize;
			Table r;

			expected = { { "3"} };
			expectedSize = expected.size();
			SuchThatClause st = SuchThatClause(make_shared<DA::NextRel>(stmtno2, stmtSyn2));
			NextEvaluator e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header2 }, r.getTableHeader()));
			vector<vector<string>> rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			for (int i = 0; i < rowsSize; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtno3, stmtSyn2));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header2 }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			for (int i = 0; i < rowsSize; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { };
			expectedSize = expected.size();
			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn3, stmtno1));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header3 }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			for (int i = 0; i < rowsSize; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { {"2"} };
			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn4, stmtno3));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header4 }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			for (int i = 0; i < rowsSize; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { {"1","2"}, {"2","3"} };
			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn3, stmtSyn4));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header3, header4 }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			for (int i = 0; i < rowsSize; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { {"1"}, {"2"} };
			st = SuchThatClause(make_shared<DA::NextRel>(stmtSyn4, wildcard));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header4 }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			for (int i = 0; i < rowsSize; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}

			expected = { {"2"}, {"3"} };
			st = SuchThatClause(make_shared<DA::NextRel>(wildcard, stmtSyn2));
			e = NextEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.evaluateClauseWithSyn();
			Assert::IsTrue(isEqualVec({ header2 }, r.getTableHeader()));
			rows = r.getTableRows();
			rowsSize = rows.size();
			Assert::AreEqual(expected.size(), rows.size());
			for (int i = 0; i < rowsSize; i++) {
				Assert::IsTrue(isEqualVec(expected[i], rows[i]));
			}
		}
	};

}
