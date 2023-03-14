#include "stdafx.h"
#include "../source/QP/QueryEvaluator/QueryEvaluator.h"
#include "../source/QP/QueryObject/Declaration.h"
#include "../source/QP/QueryObject/QueryObject.h"
#include "../source/QP/QueryObject/WithClause.h"
#include "../source/QP/QueryEvaluator/WithEvaluator.h"
#include "../source/QP/QueryEvaluator/QueryUtil.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertConstantEntityRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertProcedureEntityRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertStatementEntityRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertVariableEntityRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertModifiesRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertUsesRequest.h"
#include <codecvt>
#include <locale>
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTesting

{
	TEST_CLASS(Test_QE_WithEvaluator)
	{
	public:

		PKB* initialisePKB() {
			PKB* pkb = new PKB();
			InsertFacade insertFacade = InsertFacade(*pkb);

			Variable v1("read");
			Variable v2("print");
			Constant v3("1");
			Constant v4("2");

			Procedure p1("proc_name1");
			Procedure p2("proc_name2");
			Procedure p3("proc_name3");

			ReadStatement s1(1, "read");
			PrintStatement s2(2, "print");
			AssignStatement s3(3);
			CallStatement s4(4, "proc_name1");
			CallStatement s5(5, "proc_name2");

			vector<Rel::Modifies> modifiesList{};
			vector<Rel::Uses> usesList{};
			Modifies r1(s1, v1);
			modifiesList.push_back(r1);
			Uses r2(s2, v2);
			usesList.push_back(r2);
			
			for (Modifies a : modifiesList) {
				auto modifiesRequest = std::make_shared<InsertModifiesRequest>(a);
				insertFacade.insert(modifiesRequest);
			}

			for (Uses a : usesList) {
				auto usesRequest = std::make_shared<InsertUsesRequest>(a);
				insertFacade.insert(usesRequest);
			}

			std::vector<Ent::Variable> varList{};
			std::vector<Ent::Constant> constList{};
			std::vector<Ent::Procedure> procList{};
			std::vector<Ent::Statement> stmtList{};

			varList.push_back(v1);
			varList.push_back(v2);
			constList.push_back(v3);
			constList.push_back(v4);
			procList.push_back(p1);
			procList.push_back(p2);
			procList.push_back(p3);
			stmtList.push_back(s1);
			stmtList.push_back(s2);
			stmtList.push_back(s3);
			stmtList.push_back(s4);
			stmtList.push_back(s5);
			
			for (Variable a : varList) {
				auto requestV = std::make_shared<InsertVariableEntityRequest>(a);
				insertFacade.insert(requestV);
			}

			for (Constant a : constList) {
				auto requestC = std::make_shared<InsertConstantEntityRequest>(a);
				insertFacade.insert(requestC);
			}

			for (auto a : stmtList) {
				auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
				insertFacade.insert(requestS);
			}

			for (Procedure a : procList) {
				auto requestP = std::make_shared<InsertProcedureEntityRequest>(a);
				insertFacade.insert(requestP);
			}


			return pkb;
		}

		bool doesStarRelationExists(std::string arg1, std::string arg2, std::vector<std::pair<std::string, std::string>> vec) {
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


		TEST_METHOD(Test_WithEvaluator_bug)
		{
			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			vector<string> expectedHeaders;
			vector<vector<string>> expectedRows;
			Table resultTable;
			vector<string> resultHeaders;
			vector<vector<string>> resultRows;
			int resultRowsSize;
			shared_ptr<WithClause> withCl;
			std::string value1;
			std::string value2;

			// SYNSTMTNO_VALUE eg. assign a; with 3 = a.stmtNo;
			expectedHeaders = { "a" };
			expectedRows = { {"3"} };
			Declaration dec1 = Declaration(make_shared<DE::Assign>(), "a");
			value2 = "3";

			Argument arg1 = { Integer(value2) };
			Argument arg2 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

		}
		// Tests for With evaluator
		TEST_METHOD(Test_WithEvaluator_evaluateNoCommSyn)
		{
			/*
			* Test method: e.evaluateNoCommSyn
			* To test for with clause evaluation with no common synonym with others.
			* Should return boolean.
			*/

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			std::string value1;
			std::string value2;
			shared_ptr<WithClause> withCl;

			bool booleanResult;

			// SYNATTR_SYNATTR eg. procedure p; procedure q; with p.procName = q.procName
			// Expected { { "proc_name1", "proc_name1" }, { "proc_name2", "proc_name2" }, { "proc_name3", "proc_name3" } }
			Declaration dec1 = Declaration(make_shared<DE::Procedure>(), "p");
			Declaration dec2 = Declaration(make_shared<DE::Procedure>(), "q");
			Argument arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			Argument arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsTrue(booleanResult);

			// SYNSTMTNO_SYNSTMTNO eg. statement s; assign a; with a.stmtno = s.stmtno;
			// Expected { { "3", "3" } }
			dec1 = Declaration(make_shared<DE::Statement>(), "s");
			dec2 = Declaration(make_shared<DE::Assign>(), "a");
			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_INT} };
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsTrue(booleanResult);

			// SYNSTMTNO_SYNSTMTNO eg. statement s; call c; with c.stmtno = s.stmtno;
			dec1 = Declaration(make_shared<DE::Statement>(), "s");
			dec2 = Declaration(make_shared<DE::Call>(), "c");
			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_INT} };
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsTrue(booleanResult);

			// SYNSTMTNO_VALUE eg. statement s; with s.stmt_no = 3;
			// Expected { { "3" } }
			dec1 = Declaration(make_shared<DE::Statement>(), "s");
			value2 = "3";
			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { Integer(value2)};
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsTrue(booleanResult);

			// SYNSTMTNO_VALUE eg. statement s; with s.stmt_no = 3;
			// Expected { }
			dec1 = Declaration(make_shared<DE::Statement>(), "s");
			value2 = "0"; 
			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { Integer(value2) };
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsFalse(booleanResult);

			// VALUE_SYNATTR eg. procedure p; with "proc_name1" = p.procName;
			// Expected { {"proc_name1"} }
			dec2 = Declaration(make_shared<DE::Procedure>(), "p");
			value1 = "proc_name1";
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			arg1 = { StringExp(value1) };
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsTrue(booleanResult);

			// VALUE_SYNATTR eg. procedure p; with "proc_name4" = p.procName;
			// Expected { }
			dec2 = Declaration(make_shared<DE::Procedure>(), "p");
			value1 = "proc_name4";
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			arg1 = { StringExp(value1) };
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsFalse(booleanResult);

			// VALUE_VALUE eg. with "proc_name4" = "proc_name4";
			// Expected { }
			value1 = "proc_name4";
			value2 = "proc_name4";
			arg2 = { StringExp(value2)};
			arg1 = { StringExp(value1) };
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsTrue(booleanResult);

			// VALUE_VALUE eg. with "proc_name4" = "proc_name3";
			// Expected { }
			value1 = "proc_name4";
			value2 = "proc_name3";
			arg1 = { StringExp(value1)};
			arg2 = { StringExp(value2) };
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsFalse(booleanResult);

			// VALUE_VALUE eg. with 1 = 1;
			// Expected { }
			value1 = "1";
			value2 = "1";
			arg1 = { Integer(value1) };
			arg2 = { Integer(value2) };
			withCl = make_shared<WithClause>(arg1, arg2);
			booleanResult = WithEvaluator::evaluateNoCommSyn(withCl, &getFacade);
			Assert::IsTrue(booleanResult);

		}

		TEST_METHOD(Test_WithEvaluator_evaluateHasCommSyn)
		{
			/*
			* Test method: e.evaluateHasCommSyn
			* To test for With clause evaluation result table.
			* Should return Table
			*/

			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			vector<string> expectedHeaders;
			vector<vector<string>> expectedRows;
			Table resultTable;
			vector<string> resultHeaders;
			vector<vector<string>> resultRows;
			int resultRowsSize;
			shared_ptr<WithClause> withCl;
			std::string value1;
			std::string value2;

			
			/* SYNATTR_SYNATTR eg.procedure p; procedure q; with p.procName = q.procName */
			expectedHeaders = { "p", "q" };
			expectedRows = { { "proc_name1", "proc_name1" }, { "proc_name2", "proc_name2" }, { "proc_name3", "proc_name3" } };
			Declaration dec1 = Declaration(make_shared<DE::Procedure>(), "p");
			Declaration dec2 = Declaration(make_shared<DE::Procedure>(), "q");
			Argument arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			Argument arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNSTMTNO_SYNSTMTNO eg. statement s; assign a; with s.stmtno = a.stmtno;
			// Expected { { "3", "3" } }
			expectedHeaders = { "s", "a" };
			expectedRows = { { "3", "3" } };
			dec1 = Declaration(make_shared<DE::Statement>(), "s");
			dec2 = Declaration(make_shared<DE::Assign>(), "a");
			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_INT} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNSTMTNO_SYNSTMTNO eg. statement s; call c; with c.stmtno = s.stmtno;
			expectedHeaders = { "c", "s" };
			expectedRows = { {"4", "4"}, {"5", "5"} };
			dec1 = Declaration(make_shared<DE::Statement>(), "s");
			dec2 = Declaration(make_shared<DE::Call>(), "c");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_INT} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNSTMTNO_VALUE eg. statement s; with s.stmtno = 3;
			expectedHeaders = { "s" };
			expectedRows = { { "3" } };
			dec1 = Declaration(make_shared<DE::Statement>(), "s");
			value2 = "3";

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { Integer(value2)};
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}
		

			// SYNSTMTNO_VALUE eg. statement s; with s.stmt_no = 0;
			expectedHeaders = { "s" };
			expectedRows = { };
			dec1 = Declaration(make_shared<DE::Statement>(), "s");
			value2 = "0";

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { Integer(value2)};
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNSTMTNO_VALUE eg. statement s; with s.stmt_no = 4;
			expectedHeaders = { "s" };
			expectedRows = { {"1"}};
			dec1 = Declaration(make_shared<DE::Read>(), "s");
			value2 = "1";

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { Integer(value2)};
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}


			// VALUE_SYNATTR eg. procedure p; with "proc_name1" = p.procName;
			// Expected { {"proc_name1"} }
			expectedHeaders = { "p" };
			expectedRows = { { "proc_name1" } };
			dec2 = Declaration(make_shared<DE::Procedure>(), "p");
			value1 = "proc_name1";
			arg1 = { StringExp(value1)};
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}


			// VALUE_SYNATTR eg. procedure p; with "proc_name4" = p.procName;
			// Expected { }
			expectedHeaders = { "p" };
			expectedRows = { };
			dec2 = Declaration(make_shared<DE::Procedure>(), "p");
			value1 = "proc_name4";

			arg1 = { StringExp(value1)};
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// VALUE_VALUE eg. with "proc_name4" = "proc_name4";
			// Expected { }
			expectedHeaders = { };
			expectedRows = { };
			value1 = "proc_name4";
			value2 = "proc_name4";
			arg1 = { StringExp(value1) };
			arg2 = { StringExp(value2) };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_SYNATTR eg. read a; variable v; with a.varName = v.varName; result table |a|v|
			// Expected { }
			expectedHeaders = {"a", "v"};
			expectedRows = { {"1", "read"} };
			dec1 = Declaration(make_shared<DE::Read>(), "a");
			dec2 = Declaration(make_shared<DE::Variable>(), "v");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}


			// SYNATTR_SYNATTR eg. print a; variable v; with a.varName = v.varName; result table |a|v|
			// Expected { }
			expectedHeaders = { "a", "v" };
			expectedRows = { {"2", "print"} };
			dec1 = Declaration(make_shared<DE::Print>(), "a");
			dec2 = Declaration(make_shared<DE::Variable>(), "v");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_SYNATTR eg. call c; procedure p; with c.procName = p.procName; result table |c|p|
			// Expected { }
			expectedHeaders = { "c", "p" };
			expectedRows = { {"4", "proc_name1"}, {"5", "proc_name2"} };
			dec1 = Declaration(make_shared<DE::Call>(), "c");
			dec2 = Declaration(make_shared<DE::Procedure>(), "p");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_SYN eg. call c; procedure p; with p.procName = c.procName; result table |p|c|
			// Expected { }
			expectedHeaders = { "p", "c" };
			expectedRows = { {"proc_name1", "4"}, {"proc_name2", "5"}};
			dec1 = Declaration(make_shared<DE::Procedure>(), "p");
			dec2 = Declaration(make_shared<DE::Call>(), "c");
			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_SYNATTR eg. call c; procedure p; with c.procName = p.procName; result table |c|p|
			// Expected { }
			expectedHeaders = { "c", "p" };
			expectedRows = { {"4", "proc_name1"}, {"5", "proc_name2"} };
			dec1 = Declaration(make_shared<DE::Call>(), "c");
			dec2 = Declaration(make_shared<DE::Procedure>(), "p");
			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_SYNATTR eg. print a; variable v; with a.varName = v.varName; result table |a|v|
			// Expected { }
			expectedHeaders = { "a", "v" };
			expectedRows = { {"2", "print"} };
			dec1 = Declaration(make_shared<DE::Print>(), "a");
			dec2 = Declaration(make_shared<DE::Variable>(), "v");
			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_SYNATTR eg. print a; read v; with a.varName = v.varName; result table |a|v|
			// Expected { }
			expectedHeaders = { "a", "v" };
			expectedRows = {  };
			dec1 = Declaration(make_shared<DE::Print>(), "a");
			dec2 = Declaration(make_shared<DE::Read>(), "v");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_SYNATTR eg. const c; const v; with c.value = v.value; result table |c|v|
			// Expected { }
			expectedHeaders = { "c", "v" };
			expectedRows = { {"1", "1"}, {"2", "2"} };
			dec1 = Declaration(make_shared<DE::Constant>(), "c");
			dec2 = Declaration(make_shared<DE::Constant>(), "v");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_INT} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_SYNSTMTNO eg. const c; read v; with c.value = v.stmtno; result table |c|v|
			// Expected { }
			expectedHeaders = { "c", "v" };
			expectedRows = { {"1", "1"} };
			dec1 = Declaration(make_shared<DE::Constant>(), "c");
			dec2 = Declaration(make_shared<DE::Read>(), "v");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_INT} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_VALUE eg. const c; read v; with c.value = 1; result table |c|v|
			// Expected { }
			expectedHeaders = { "c"};
			expectedRows = { {"1"} };
			dec1 = Declaration(make_shared<DE::Constant>(), "c");
			dec2 = Declaration(make_shared<DE::Read>(), "v");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { Integer("1")};
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_VALUE eg. const c; read v; with c.value = 1; result table |c|v|
			// Expected { }
			expectedHeaders = { "c" };
			expectedRows = { {"1"} };
			dec1 = Declaration(make_shared<DE::Constant>(), "c");
			dec2 = Declaration(make_shared<DE::Read>(), "v");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_INT} };
			arg2 = { Integer("1")};
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_VALUE eg. call c; read v; with c.procName = "proc_name1"; result table |c|v|
			// Expected { }
			expectedHeaders = { "c" };
			expectedRows = { {"4"} };
			dec1 = Declaration(make_shared<DE::Call>(), "c");
			dec2 = Declaration(make_shared<DE::Read>(), "v");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			arg2 = { StringExp("proc_name1")};
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// SYNATTR_VALUE eg. procedure c; read v; with c.procName = "proc_name1"; result table |c|v|
			// Expected { }
			expectedHeaders = { "c" };
			expectedRows = { {"proc_name1"} };
			dec1 = Declaration(make_shared<DE::Procedure>(), "c");
			dec2 = Declaration(make_shared<DE::Read>(), "v");

			arg1 = { SynAttr{dec1, ArgType::ATTR_REF_NAME} };
			arg2 = { StringExp("proc_name1")};
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

			// VALUE_SYNATTR eg. procedure c; read v; with  "proc_name1" = c.procName; result table |c|
			expectedHeaders = { "c" };
			expectedRows = { {"proc_name1"} };
			dec1 = Declaration(make_shared<DE::Read>(), "v");
			dec2 = Declaration(make_shared<DE::Procedure>(), "c");
			arg1 = { StringExp("proc_name1")};
			arg2 = { SynAttr{dec2, ArgType::ATTR_REF_NAME} };
			withCl = make_shared<WithClause>(arg1, arg2);
			// result
			resultTable = WithEvaluator::evaluateHasCommSyn(withCl, &getFacade);
			resultHeaders = resultTable.getTableHeader();
			resultRows = resultTable.getTableRows();
			// check headers
			Assert::AreEqual(expectedHeaders.size(), resultHeaders.size());
			Assert::IsTrue(isEqualVec(expectedHeaders, resultHeaders));
			// check rows
			Assert::AreEqual(expectedRows.size(), resultRows.size());
			resultRowsSize = resultRows.size();
			for (int i = 0; i < resultRowsSize; i++) {
				Assert::IsTrue(isEqualVec(expectedRows[i], resultRows[i]));
			}

		}
	};

}
