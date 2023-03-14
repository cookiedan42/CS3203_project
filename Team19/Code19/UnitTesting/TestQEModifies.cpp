#include "stdafx.h"
#include "../source/QP/QueryObject/SuchThatClause.h"
#include "../source/QP/QueryEvaluator/ModifiesEvaluator.h"
#include "../source/QP/QueryEvaluator/QueryUtil.h"
#include "../source/QP/QueryObject/DesignAbstractionFactory.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertStatementEntityRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertVariableEntityRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertModifiesRequest.h"
#include <codecvt>
#include <locale>
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting

{
	TEST_CLASS(Test_QE_Modifies)
	{
	public:

		PKB* initialisePKB() {
			PKB* pkb = new PKB();
			InsertFacade insertFacade = InsertFacade(*pkb);

			ReadStatement s3(3, "read_3");
			AssignStatement s4(4);
			AssignStatement s5(5);
			ReadStatement s6(6, "read_6");

			Procedure p1("proc_name1");

			Variable v1( "x1");
			Variable v2( "y");
			Variable v3( "x2");
			Variable v4( "x3");
			Variable v5( "z");
			Variable v6( "y");

			std::vector<Ent::Statement> stmtList{};
			std::vector<Ent::Variable> varList{};
			stmtList.push_back(s3);
			stmtList.push_back(s4);
			stmtList.push_back(s5);
			stmtList.push_back(s6);
			varList.push_back(v1);
			varList.push_back(v2);
			varList.push_back(v3);
			varList.push_back(v4);
			varList.push_back(v5);
			varList.push_back(v6);

			for (Variable a : varList) {
				auto requestV = std::make_shared<InsertVariableEntityRequest>(a);
				insertFacade.insert(requestV);
			}

			for (auto a : stmtList) {
				auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
				insertFacade.insert(requestS);
			}

			// please note that order of insertion matters for checking
			Modifies r1(s3, v5);
			Modifies r2(s4, v1);
			Modifies r3(s4, v3);
			Modifies r4(s4, v4);
			Modifies r5(s5, v2);
			Modifies r6(s6, v6);
			Modifies r7(p1, v1);

			vector<Rel::Modifies> modifiesList{};

			modifiesList.push_back(r1);
			modifiesList.push_back(r2);
			modifiesList.push_back(r3);
			modifiesList.push_back(r4);
			modifiesList.push_back(r5);
			modifiesList.push_back(r6);

			for (Modifies a : modifiesList) {
				auto modifiesRequest = std::make_shared<InsertModifiesRequest>(a);
				insertFacade.insert(modifiesRequest);
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

		TEST_METHOD(Test_ModifiesEvaluator_getRelationPairs_evaluateClauseWithNoSyn)
		{
			/*
			* Test method: e.evaluateClauseWithNoSyn
			* To test for Modifies relationship with no common synonym with others.
			* Should return boolean.
			*/

			// Modifies pairs include{ {"3","z"}, {"4","x1"}, {"4","x2"}, {"4","x3"}, {"5","y"}, {"6", "y"} };
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
			struct Argument readSyn1 = { Declaration(make_shared<DE::Read>(), "s3") };
			struct Argument varSyn1 = { Declaration(make_shared<DE::Variable>(), "varSyn") };
			struct Argument wildcard = { WildCard() };

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			std::vector<std::pair<std::string, std::string>> expected;
			RelationshipResults r; // result for getRelationPairs
			std::pair<std::string, std::string> rPair;
			std::pair<std::string, std::string> expectedPair;
			bool res; // result for evaluateClauseWithNoSyn

			// STMTNO_QUOTE
			expected = { {"3", "z"} };
			SuchThatClause st = SuchThatClause(make_shared<DA::ModifiesRel>(stmtno3, varname_z));
			ModifiesEvaluator e = ModifiesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("3", "z", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// STMTNO_QUOTE
			expected = { };
			st = SuchThatClause(make_shared<DA::ModifiesRel>(stmtno3, varname_x));
			e = ModifiesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.empty(), r.empty());
			res = e.evaluateClauseWithNoSyn();
			Assert::IsFalse(res);

			// STMTNO_SYN
			expected = { {"4", "x1"}, {"4", "x2"}, {"4", "x3"} };
			st = SuchThatClause(make_shared<DA::ModifiesRel>(stmtno4, stmtSyn1)); // z is a synonym, should return all variables used by s4
			e = ModifiesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("4", "x1", r));
			Assert::IsTrue(doesPairExists("4", "x2", r));
			Assert::IsTrue(doesPairExists("4", "x3", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// STMTNO_SYN
			expected = { {"3", "z"} };
			st = SuchThatClause(make_shared<DA::ModifiesRel>(stmtno3, stmtSyn1)); // z is a synonym, should return all variables used by s3
			e = ModifiesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("3", "z", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// STMTNO_WILDCARD
			expected = { {"4", "x1"}, {"4", "x2"}, {"4", "x3"} };
			st = SuchThatClause(make_shared<DA::ModifiesRel>(stmtno4, wildcard)); // should return all variables used by s4
			e = ModifiesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("4", "x1", r));
			Assert::IsTrue(doesPairExists("4", "x2", r));
			Assert::IsTrue(doesPairExists("4", "x3", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// SYN_QUOTE
			expected = { {"5", "y"}, {"6", "y"} };
			st = SuchThatClause(make_shared<DA::ModifiesRel>(stmtSyn1, varname_y)); 
			e = ModifiesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("5", "y", r));
			Assert::IsTrue(doesPairExists("6", "y", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// SYN_SYN
			expected = { {"3","z"}, {"4","x2"}, {"4","x1"}, {"4","x3"}, {"5","y"}, {"6", "y"} };
			st = SuchThatClause(make_shared<DA::ModifiesRel>(stmtSyn1, varSyn1)); 
			e = ModifiesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("3", "z", r));
			Assert::IsTrue(doesPairExists("5", "y", r));
			Assert::IsTrue(doesPairExists("6", "y", r));
			Assert::IsTrue(doesPairExists("4", "x1", r));
			Assert::IsTrue(doesPairExists("4", "x2", r));
			Assert::IsTrue(doesPairExists("4", "x3", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// SYN_WILDCARD
			expected = { {"3","z"}, {"4","x2"}, {"4","x1"}, {"4","x3"}, {"5","y"}, {"6", "y"} };
			st = SuchThatClause(make_shared<DA::ModifiesRel>(stmtSyn1, wildcard)); 
			e = ModifiesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("3", "z", r));
			Assert::IsTrue(doesPairExists("5", "y", r));
			Assert::IsTrue(doesPairExists("6", "y", r));
			Assert::IsTrue(doesPairExists("4", "x1", r));
			Assert::IsTrue(doesPairExists("4", "x2", r));
			Assert::IsTrue(doesPairExists("4", "x3", r));
			res = e.evaluateClauseWithNoSyn();
			Assert::IsTrue(res);

			// SYN_WILDCARD
			expected = { {"3","z"}, {"4","x2"}, {"4","x1"}, {"4","x3"}, {"5","y"}, {"6", "y"} };
			st = SuchThatClause(make_shared<DA::ModifiesRel>(readSyn1, wildcard));
			e = ModifiesEvaluator(st.relationship->arg1, st.relationship->arg2, getFacade);
			r = e.getRelationPairs();
			/*Assert::AreEqual(expected.size(), r.size());
			Assert::IsTrue(doesPairExists("3", "z", r));
			Assert::IsTrue(doesPairExists("5", "y", r));
			Assert::IsTrue(doesPairExists("6", "y", r));
			Assert::IsTrue(doesPairExists("4", "x1", r));
			Assert::IsTrue(doesPairExists("4", "x2", r));
			Assert::IsTrue(doesPairExists("4", "x3", r));
			res = e.evaluateClauseWithNoSyn(st.relationship, &getFacade);*/
			Assert::IsTrue(!r.empty());
			//Assert::IsTrue(res);

		}

		//TEST_METHOD(Test_ModifiesEvaluator_evaluateClauseWithSyn)
		//{
		//	/*
		//	* Test method: ParentEvaluator::evaluateClauseWithSyn
		//	* To test for Parent relationship result table.
		//	* Should return unordered_map<string, vector<string>>
		//	*/

		//	PKBFacade* pkb = initialisePKB();
		//	// Modifies pairs include{ {"3","z"}, {"4","x1"}, {"4","x2"}, {"4","x3"}, {"5","y"}, {"6", "y"} };
		//	std::unordered_map<std::string, std::vector<std::string>> expected;
		//	std::unordered_map<std::string, std::vector<std::string>> r;
		//	std::pair<std::string, std::vector<std::string>> rPair;

		//	expected = { };
		//	SuchThatClause st = SuchThatClause(make_shared<DA::ModifiesRel>("3", "\"x\"")); // get by arg1 and arg2
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
		//	st = SuchThatClause(make_shared<DA::ModifiesRel>("4", "_")); // no comm syn, no table
		//	r = e.evaluateClauseWithSyn(st, &getFacade);
		//	Assert::AreEqual(expected.empty(), r.empty());

		//	expected = { {"z", { "5", "6" } } };
		//	st = SuchThatClause(make_shared<DA::ModifiesRel>("z", "\"y\"")); // syn z, table with z as header
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
		//	st = SuchThatClause(make_shared<DA::ModifiesRel>("z", "y")); // syn z and syn y, table with z and y as header, all pairs
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
		//	st = SuchThatClause(make_shared<DA::ModifiesRel>("z", "_")); // syn z and syn y, table with z and y as header, all pairs
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