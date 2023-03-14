#include "stdafx.h"
#include "../source/QP/QueryEvaluator/QueryEvaluator.h"
#include "../source/QP/QueryObject/Declaration.h"
#include "../source/QP/QueryObject/QueryObject.h"
#include "../source/QP/QueryParser/QueryParserResult.h"
#include "../source/Optimizer/Optimizer.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Request/InsertRequests/InsertPatternRequests/InsertAssignPatternRequest.h"
#include "../source/Request/InsertRequests/InsertPatternRequests/InsertIfPatternRequest.h"
#include "../source/Request/InsertRequests/InsertPatternRequests/InsertWhilePatternRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertConstantEntityRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertProcedureEntityRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertStatementEntityRequest.h"
#include "../source/Request/InsertRequests/InsertEntityRequests/InsertVariableEntityRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertCallsRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertFollowsRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertModifiesRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertParentRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertUsesRequest.h"
#include <codecvt>
#include <locale>
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting

{
	TEST_CLASS(TestQueryEvaluator)
	{
	public:

		PKB initialisePKB() {
			auto pkb = PKB();
			auto insertFacade = InsertFacade(pkb);

			PrintStatement s1(1, "print");
			WhileStatement s2(2);
			IfStatement s3(3);
			AssignStatement s4(4);
			AssignStatement s5(5);
			Procedure s6("p1_name");
			Procedure s7("p2_name");
			Procedure s8("p3_name");
			ReadStatement s9(9, "r1");
			ReadStatement s10(10, "r2");
			CallStatement s11(11, "c1");
			CallStatement s12(12, "c2");
			CallStatement s13(13, "c3");
			CallStatement s14(14, "c4");

			Variable v1("x");
			Variable v2("y");
			Variable v3("y");
			Variable v4("print");
			Variable v5("r1");
			Variable v6("r2");

			Constant const1("1");
			Constant const2("2");
			Constant const3("3");

			std::vector<Ent::Variable> varList{};
			std::vector<Ent::Procedure> procList{};
			std::vector<Ent::Statement> stmtList{};
			std::vector<Ent::Constant> constList{};
			std::vector<Ptn::AssignPtn> assignPtnList{};
			std::vector<Ptn::IfPtn> ifPtnList{};
			std::vector<Ptn::WhilePtn> whilePtnList{};

			stmtList.push_back(s1);
			stmtList.push_back(s2);
			stmtList.push_back(s3);
			stmtList.push_back(s4);
			stmtList.push_back(s5);
			procList.push_back(s6);
			procList.push_back(s7);
			procList.push_back(s8);
			stmtList.push_back(s9);
			stmtList.push_back(s10);
			stmtList.push_back(s11);
			stmtList.push_back(s12);
			stmtList.push_back(s13);
			stmtList.push_back(s14);
			varList.push_back(v1);
			varList.push_back(v2);
			varList.push_back(v3);
			varList.push_back(v4);
			varList.push_back(v5);
			varList.push_back(v6);
			constList.push_back(const1);
			constList.push_back(const2);
			constList.push_back(const3);


			assignPtnList.push_back(Ptn::AssignPtn(4, "y", "+xy", "x+y"));
			assignPtnList.push_back(Ptn::AssignPtn(5, "y", "y", "y"));
			ifPtnList.push_back(Ptn::IfPtn(3, { Variable("a") }));
			whilePtnList.push_back(Ptn::WhilePtn(2, { Variable("b") }));

			for (auto a : stmtList) {
				auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
				insertFacade.insert(requestS);
			}

			for (Procedure a : procList) {
				auto requestP = std::make_shared<InsertProcedureEntityRequest>(a);
				insertFacade.insert(requestP);
			}

			for (Variable a : varList) {
				auto requestV = std::make_shared<InsertVariableEntityRequest>(a);
				insertFacade.insert(requestV);
			}

			for (Constant a : constList) {
				auto requestV = std::make_shared<InsertConstantEntityRequest>(a);
				insertFacade.insert(requestV);
			}

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

			// please note that order of insertion matters for checking
			Follows f1(s1, s2);
			Follows f2(s2, s3);
			Parent p1(s2, s3);
			Parent p2(s3, s4);
			Uses u1(s4, v1);
			Uses u2(s4, v2);
			Uses u3(s5, v3);
			Uses u4(s1, v4); // 1, "print"
			Modifies m1(s4, v1);
			Modifies m2(s4, v2);
			Modifies m3(s5, v3);
			Modifies m4(s9, v5);
			Modifies m5(s10, v6);
			Calls c1(s6, s7);
			Calls c2(s7, s8);


			vector<Rel::Follows> followsList{};
			vector<Rel::Parent> parentList{};
			vector<Rel::Uses> usesList{};
			vector<Rel::Modifies> modifiesList{};
			vector<Rel::Calls> callsList{};

			followsList.push_back(f1);
			followsList.push_back(f2);
			parentList.push_back(p1);
			parentList.push_back(p2);
			usesList.push_back(u1);
			usesList.push_back(u2);
			usesList.push_back(u3);
			usesList.push_back(u4);
			modifiesList.push_back(m1);
			modifiesList.push_back(m2);
			modifiesList.push_back(m3);
			modifiesList.push_back(m4);
			modifiesList.push_back(m5);
			callsList.push_back(c1);
			callsList.push_back(c2);

			for (Parent a : parentList) {
				auto parentRequest = std::make_shared<InsertParentRequest>(a);
				insertFacade.insert(parentRequest);
			}

			for (Follows a : followsList) {
				auto followsRequest = std::make_shared<InsertFollowsRequest>(a);
				insertFacade.insert(followsRequest);
			}

			for (Modifies a : modifiesList) {
				auto modifiesRequest = std::make_shared<InsertModifiesRequest>(a);
				insertFacade.insert(modifiesRequest);
			}

			for (Uses a : usesList) {
				auto usesRequest = std::make_shared<InsertUsesRequest>(a);
				insertFacade.insert(usesRequest);
			}

			for (Calls a : callsList) {
				auto callsRequest = std::make_shared<InsertCallsRequest>(a);
				insertFacade.insert(callsRequest);
			}

			//after all insertion then generate star data
			insertFacade.generateSecondaryData();

			return pkb;
		}

		bool isEqualList(std::list<std::string> v1, std::list<std::string> v2) {
			v1.sort();
			v2.sort();
			return v1 == v2;
		}

		TEST_METHOD(Test_New_QE_BooleanReturn)
		{
			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);

			//Follows
			list<string> expected = { "TRUE" };
			Declaration d1 = Declaration(make_shared<DE::Statement>(), "a");
			SelectObject so = SelectObject();

			shared_ptr<SuchThatClause> st = make_shared<SuchThatClause>(
				make_shared<DA::FollowsRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			ClauseGroup group = ClauseGroup();
			group.addClauseToGroup(st);
			ClauseGroup clausesWithNoSynonym = {};
			std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect = { group };
			std::vector<ClauseGroup> clauseGroupsRelatedToSelect = {};

			QueryObject in = QueryObject(so, { d1 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect, clauseGroupsRelatedToSelect);
			list<string> res = QueryEvaluator::evaluate(in, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));
		}

		TEST_METHOD(Test_New_QE_TupleReturn)
		{
			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);

			// 1 clause in a group related to Select
			list<string> expected = { "2 p1_name", "2 p2_name", "2 p3_name" };
			Declaration d1 = Declaration(make_shared<DE::Statement>(), "a");
			Declaration d2 = Declaration(make_shared<DE::Procedure>(), "b");
			SelectObject so = SelectObject({ {d1, ArgType::SYN}, {d2, ArgType::SYN} });

			shared_ptr<SuchThatClause> st = make_shared<SuchThatClause>(
				make_shared<DA::FollowsRel>(Argument{ d1 }, Argument{ StmtNo("3") })); // 2
			ClauseGroup group = ClauseGroup();
			group.addClauseToGroup(st);
			ClauseGroup clausesWithNoSynonym = {};
			std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect = {};
			std::vector<ClauseGroup> clauseGroupsRelatedToSelect = { group };

			QueryObject in = QueryObject(so, { d1, d2 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);
			list<string> res = QueryEvaluator::evaluate(in, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));


			// 1, 2 clause groups related to Select
			expected = { "2 p3_name" };
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			d2 = Declaration(make_shared<DE::Procedure>(), "b");
			so = SelectObject({ {d1, ArgType::SYN}, {d2, ArgType::SYN} });

			shared_ptr<SuchThatClause> st1 = make_shared<SuchThatClause>(
				make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") })); // 1,2
			shared_ptr<SuchThatClause> st2 = make_shared<SuchThatClause>(
				make_shared<DA::ParentRel>(Argument{ d1 }, Argument{ StmtNo("3") }));  // 2
			shared_ptr<WithClause> w1 = make_shared<WithClause>(Argument{ SynAttr{d2, ArgType::ATTR_REF_NAME} }, Argument{ StringExp("p3_name") }); // p3_name 
			ClauseGroup group1 = ClauseGroup();
			ClauseGroup group2 = ClauseGroup();
			/*
			group1.addClauseToGroup(st1);
			group1.addClauseToGroup(st2);
			group2.addClauseToGroup(w1);
			clausesWithNoSynonym = {};
			clauseGroupsNotRelatedToSelect = {};
			clauseGroupsRelatedToSelect = { group1, group2 };*/

			SuchThatClause test_st1 = SuchThatClause(make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") })); // 1,2
			SuchThatClause test_st2 = SuchThatClause(make_shared<DA::ParentRel>(Argument{ d1 }, Argument{ StmtNo("3") }));  // 2
			WithClause test_w1 = WithClause(Argument{ SynAttr{d2, ArgType::ATTR_REF_NAME} }, Argument{ StringExp("p3_name") }); // p3_name 

			QueryParserResult result = QueryParserResult({ d1 }, so, {}, { test_st1, test_st2 }, { }, {}, {}, { test_w1 });
			QueryObject optimizedQueryObj = Optimizer::group(result);

			//in = QueryObject(so, { d1, d2 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
			//	clauseGroupsRelatedToSelect);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));

			// 1, 2 clause groups related to Select, but joining gives empty result
			expected = { };
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			d2 = Declaration(make_shared<DE::Procedure>(), "b");
			so = SelectObject({ {d1, ArgType::SYN}, {d2, ArgType::SYN} });

			st1 = make_shared<SuchThatClause>(
				make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") })); // 1,2
			st2 = make_shared<SuchThatClause>(
				make_shared<DA::ParentRel>(Argument{ d1 }, Argument{ StmtNo("4") }));  // 3
			w1 = make_shared<WithClause>(Argument{ SynAttr{d2, ArgType::ATTR_REF_NAME} }, Argument{ StringExp("p3_name") }); // p3_name 
			group1 = ClauseGroup();
			group2 = ClauseGroup();
			group1.addClauseToGroup(st1);
			group1.addClauseToGroup(st2);
			group2.addClauseToGroup(w1);
			clausesWithNoSynonym = {};
			clauseGroupsNotRelatedToSelect = {};
			clauseGroupsRelatedToSelect = { group1, group2 };

			in = QueryObject(so, { d1, d2 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);
			res = QueryEvaluator::evaluate(in, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));


			// 2, 2 clause groups related to Select, but joining gives empty result
			expected = { };
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			d2 = Declaration(make_shared<DE::Procedure>(), "b");
			so = SelectObject({ {d1, ArgType::SYN}, {d2, ArgType::SYN} });

			st1 = make_shared<SuchThatClause>(
				make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") })); // 1,2
			st2 = make_shared<SuchThatClause>(
				make_shared<DA::ParentRel>(Argument{ d1 }, Argument{ StmtNo("3") }));  // 2
			w1 = make_shared<WithClause>(Argument{ SynAttr{d2, ArgType::ATTR_REF_NAME} }, Argument{ StringExp("p3_name") }); //  p3_name
			shared_ptr<WithClause> w2 = make_shared<WithClause>(Argument{ SynAttr{d2, ArgType::ATTR_REF_NAME} }, Argument{ StringExp("p4_name") }); //  
			group1 = ClauseGroup();
			group2 = ClauseGroup();
			group1.addClauseToGroup(st1);
			group1.addClauseToGroup(st2);
			group2.addClauseToGroup(w1);
			group2.addClauseToGroup(w2);
			clausesWithNoSynonym = {};
			clauseGroupsNotRelatedToSelect = {};
			clauseGroupsRelatedToSelect = { group1, group2 };

			in = QueryObject(so, { d1, d2 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);
			res = QueryEvaluator::evaluate(in, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));
		}

		TEST_METHOD(Test_QE_Tuple_Syn_SynREFINT_ExistingInSuchThatTable_Return)
		{
			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);

			// Test stmt a; Select <a, a.stmt#> such that Follows*(a, 3) // answer: 1 1, 2 2
			list<string> expected = { "1 1", "2 2" };
			Declaration d1 = Declaration(make_shared<DE::Statement>(), "a");
			shared_ptr<DesignAbstraction> da = make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") });
			shared_ptr<SuchThatClause> st = make_shared<SuchThatClause>(da);
			SelectObject so = SelectObject({ {d1, ArgType::SYN}, {d1, ArgType::ATTR_REF_INT } });

			ClauseGroup group = ClauseGroup();
			group.addClauseToGroup(st);
			ClauseGroup clausesWithNoSynonym = {};
			std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect = {};
			std::vector<ClauseGroup> clauseGroupsRelatedToSelect = { group };

			QueryObject optimizedQueryObj = QueryObject(so, { d1 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);
			list<string> res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));
		}

		TEST_METHOD(Test_QE_Tuple_Syn_SynREFINT_NotExistingInTables_Return)
		{
			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);

			// Test stmt a; Select <a, a.stmt#> such that Follows*(b, 3) // answer: 1 1, 2 2
			list<string> expected = { "1 1", "2 2", "3 3", "4 4", "5 5", "9 9", "10 10", "11 11", "12 12", "13 13", "14 14" };
			Declaration d1 = Declaration(make_shared<DE::Statement>(), "a");
			Declaration d2 = Declaration(make_shared<DE::Statement>(), "b");
			shared_ptr<DesignAbstraction> da = make_shared<DA::FollowsTRel>(Argument{ d2 }, Argument{ StmtNo("3") }); // b = 1, 2
			shared_ptr<SuchThatClause> st = make_shared<SuchThatClause>(da);
			SelectObject so = SelectObject({ {d1, ArgType::SYN}, {d1, ArgType::ATTR_REF_INT } });

			ClauseGroup group = ClauseGroup();
			group.addClauseToGroup(st);
			ClauseGroup clausesWithNoSynonym = {};
			std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect = {};
			std::vector<ClauseGroup> clauseGroupsRelatedToSelect = { group };

			QueryObject optimizedQueryObj = QueryObject(so, { d1 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);
			list<string> res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));
		}

		TEST_METHOD(Test_QE_Tuple_SynREFNAME_SynREFINT_NotExistingInTables_Return)
		{
			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);

			// Test print pr; Select <pr.varName, pr.stmt#> such that Follows*(b, 3) // answer: print 1
			list<string> expected = { "print 1" };
			Declaration d1 = Declaration(make_shared<DE::Print>(), "pr");
			Declaration d2 = Declaration(make_shared<DE::Statement>(), "b");
			shared_ptr<DesignAbstraction> da = make_shared<DA::FollowsTRel>(Argument{ d2 }, Argument{ StmtNo("3") }); // b = 1, 2
			shared_ptr<SuchThatClause> st = make_shared<SuchThatClause>(da);
			SelectObject so = SelectObject({ {d1, ArgType::ATTR_REF_NAME}, {d1, ArgType::ATTR_REF_INT } });

			ClauseGroup group = ClauseGroup();
			group.addClauseToGroup(st);
			ClauseGroup clausesWithNoSynonym = {};
			std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect = {};
			std::vector<ClauseGroup> clauseGroupsRelatedToSelect = { group };

			QueryObject optimizedQueryObj = QueryObject(so, { d1 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);
			list<string> res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));
		}

		TEST_METHOD(Test_QE_Tuple_SynREFINT_SynREFNAME_NotExistingInTables_Return)
		{
			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);

			// Test print pr; Select <c.value, pr.varName> such that Follows*(b, 3) // answer: 1 print, 2 print, 3 print
			list<string> expected = { "1 print", "2 print", "3 print" };
			Declaration d1 = Declaration(make_shared<DE::Print>(), "pr");
			Declaration d2 = Declaration(make_shared<DE::Statement>(), "b");
			Declaration d3 = Declaration(make_shared<DE::Constant>(), "c");
			shared_ptr<DesignAbstraction> da = make_shared<DA::FollowsTRel>(Argument{ d2 }, Argument{ StmtNo("3") }); // b = 1, 2
			shared_ptr<SuchThatClause> st = make_shared<SuchThatClause>(da);
			SelectObject so = SelectObject({ {d3, ArgType::ATTR_REF_INT}, {d1, ArgType::ATTR_REF_NAME } });

			ClauseGroup group = ClauseGroup();
			group.addClauseToGroup(st);
			ClauseGroup clausesWithNoSynonym = {};
			std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect = {};
			std::vector<ClauseGroup> clauseGroupsRelatedToSelect = { group };

			QueryObject optimizedQueryObj = QueryObject(so, { d1 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);
			list<string> res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));
		}

		TEST_METHOD(Test_QE_Tuple_REFNAME_Return)
		{
			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);

			// Test call c; Select c.procName such that Follows*(b, 3) // answer: c1, c2, c3, c4
			list<string> expected = { "c1", "c2", "c3", "c4" };
			Declaration d1 = Declaration(make_shared<DE::Call>(), "c");
			Declaration d2 = Declaration(make_shared<DE::Statement>(), "b");
			shared_ptr<DesignAbstraction> da = make_shared<DA::FollowsTRel>(Argument{ d2 }, Argument{ StmtNo("3") }); // b = 1, 2
			shared_ptr<SuchThatClause> st = make_shared<SuchThatClause>(da);
			SelectObject so = SelectObject({ {d1, ArgType::ATTR_REF_NAME} });

			ClauseGroup group = ClauseGroup();
			group.addClauseToGroup(st);
			ClauseGroup clausesWithNoSynonym = {};
			std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect = {};
			std::vector<ClauseGroup> clauseGroupsRelatedToSelect = { group };

			QueryObject optimizedQueryObj = QueryObject(so, { d1 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);
			list<string> res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));
		}

		TEST_METHOD(Test_QE_Tuple_Read_REFNAME_Return)
		{
			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);

			// Test read r; Select r.varName such that Follows*(b, 3) // answer: r1, r2
			list<string> expected = { "r1", "r2" };
			Declaration d1 = Declaration(make_shared<DE::Read>(), "r");
			Declaration d2 = Declaration(make_shared<DE::Statement>(), "b");
			shared_ptr<DesignAbstraction> da = make_shared<DA::FollowsTRel>(Argument{ d2 }, Argument{ StmtNo("3") }); // b = 1, 2
			shared_ptr<SuchThatClause> st = make_shared<SuchThatClause>(da);
			SelectObject so = SelectObject({ {d1, ArgType::ATTR_REF_NAME} });

			ClauseGroup group = ClauseGroup();
			group.addClauseToGroup(st);
			ClauseGroup clausesWithNoSynonym = {};
			std::vector<ClauseGroup> clauseGroupsNotRelatedToSelect = {};
			std::vector<ClauseGroup> clauseGroupsRelatedToSelect = { group };

			QueryObject optimizedQueryObj = QueryObject(so, { d1 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);
			list<string> res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));
		}

		TEST_METHOD(Test_QE_evaluate)
		{
			PKB pkb = initialisePKB();
			auto getFacade = GetFacade(pkb);

			//Follows
			list<string> expected = { "2" };
			Declaration d1 = Declaration(make_shared<DE::Statement>(), "a");
			SuchThatClause st = SuchThatClause(make_shared<DA::FollowsRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			SelectObject so = SelectObject({ {d1, ArgType::SYN} });
			QueryParserResult result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});

			QueryObject optimizedQueryObj = Optimizer::group(result);

			list<string> res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));


			//Follows-while
			expected = { "2" };
			d1 = Declaration(make_shared<DE::While>(), "a");
			st = SuchThatClause(make_shared<DA::FollowsRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			so = SelectObject({ {d1, ArgType::SYN} });
			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});

			optimizedQueryObj = Optimizer::group(result);

			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));


			//Follows-print
			expected = { };
			d1 = Declaration(make_shared<DE::Print>(), "a");
			st = SuchThatClause(make_shared<DA::FollowsRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			so = SelectObject({ {d1, ArgType::SYN} });
			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});

			optimizedQueryObj = Optimizer::group(result);

			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));

			//Follows*
			expected = { "1" };
			d1 = Declaration(make_shared<DE::Print>(), "a");
			st = SuchThatClause(make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			so = SelectObject({ {d1, ArgType::SYN} });
			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::IsTrue(isEqualList(expected, res));

			// Follows* gives 1,2 and With clause wants a.stmtno=1
			expected = { "1" };
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			st = SuchThatClause(make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			so = SelectObject({ {d1, ArgType::SYN} });
			WithClause with = WithClause(Argument{ SynAttr{d1, ArgType::ATTR_REF_INT} }, Argument{ Integer("1") });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, { with });
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::IsTrue(isEqualList(expected, res));

			// Follows* gives 1,2 and With clause wants a = b
			expected = { "1", "2" };
			st = SuchThatClause(make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			Declaration d2 = Declaration(make_shared<DE::Statement>(), "b");
			so = SelectObject({ {d1, ArgType::SYN} });
			with = WithClause(Argument{ SynAttr{d1, ArgType::ATTR_REF_INT} }, Argument{ SynAttr{d2, ArgType::ATTR_REF_INT} });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, { with });
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::IsTrue(isEqualList(expected, res));

			// Follows* gives 1,2 and With clause wants 1 = 1
			expected = { "1", "2" };
			st = SuchThatClause(make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			so = SelectObject({ {d1, ArgType::SYN} });
			with = WithClause(Argument{ Integer("1") }, Argument{ Integer("1") });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, { with });
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::IsTrue(isEqualList(expected, res));

			// Follows* gives 1,2 and With clause wants a.stmtno = 4
			expected = { };
			st = SuchThatClause(make_shared<DA::FollowsTRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			so = SelectObject({ {d1, ArgType::SYN} });
			with = WithClause(Argument{ SynAttr{d1, ArgType::ATTR_REF_INT} }, Argument{ Integer("4") });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, { with });
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::IsTrue(isEqualList(expected, res));

			//Parent
			expected = { "2" };
			st = SuchThatClause(make_shared<DA::ParentRel>(Argument{ d1 }, Argument{ StmtNo("3") }));
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			so = SelectObject({ {d1, ArgType::SYN} });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::AreEqual(expected.size(), res.size());


			//Parent*
			expected = { "2", "3" };
			st = SuchThatClause(make_shared<DA::ParentTRel>(Argument{ d1 }, Argument{ StmtNo("4") }));
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			so = SelectObject({ {d1, ArgType::SYN} });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::IsTrue(isEqualList(expected, res));

			//Uses
			expected = { "1", "4", "5" };
			// Select a such that Uses(a, _)
			st = SuchThatClause(make_shared<DA::UsesRel>(Argument{ d1 }, Argument{ WildCard() }));
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			so = SelectObject({ {d1, ArgType::SYN} });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));

			//Modifies
			expected = { "4", "5" };
			// Select a such that Modifies(a, "y")
			st = SuchThatClause(make_shared<DA::ModifiesRel>(Argument{ d1 }, Argument{ StringExp("y") }));
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			so = SelectObject({ {d1, ArgType::SYN} });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));

			//Calls
			expected = { "p1_name", "p2_name" };
			d1 = Declaration(make_shared<DE::Procedure>(), "a");
			st = SuchThatClause(make_shared<DA::CallsRel>(Argument{ d1 }, Argument{ WildCard() }));
			so = SelectObject({ {d1, ArgType::SYN} });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));

			//Calls
			expected = { "p1_name" };
			d1 = Declaration(make_shared<DE::Procedure>(), "a");
			st = SuchThatClause(make_shared<DA::CallsRel>(Argument{ d1 }, Argument{ StringExp("p2_name") }));
			so = SelectObject({ {d1, ArgType::SYN} });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));

			//Calls*
			expected = { "p1_name", "p2_name" };
			d1 = Declaration(make_shared<DE::Procedure>(), "a");
			st = SuchThatClause(make_shared<DA::CallsTRel>(Argument{ d1 }, Argument{ StringExp("p3_name") }));
			so = SelectObject({ {d1, ArgType::SYN} });

			result = QueryParserResult({ d1 }, so, {}, { st }, {}, {}, {}, {});
			optimizedQueryObj = Optimizer::group(result);
			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));

			////Pattern Assign
			//expected = { "4", "5" };
			//d1 = Declaration(make_shared<DE::Statement>(), "a");
			//Declaration d2 = Declaration(make_shared<DE::Assign>(), "p");
			//so = SelectObject({ d2 });
			//PatternClause pt = PatternClause(d2, "_", "_");
			//in = QueryObject({ d1, d2 }, so, {}, {}, {}, { pt });
			//res = QueryEvaluator::evaluate(in, &getFacade);
			//Assert::AreEqual(expected.size(), res.size());
			//Assert::IsTrue(isEqualList(expected, res));


			//expected = { "4" };
			//d1 = Declaration(make_shared<DE::Statement>(), "a");
			//d2 = Declaration(make_shared<DE::Assign>(), "p");
			//so = SelectObject({ d2 });
			//pt = PatternClause(d2, "_", "\"x+y\"");
			//in = QueryObject({ d1, d2 }, so, {}, {}, {}, { pt });
			//res = QueryEvaluator::evaluate(in, &getFacade);
			//Assert::AreEqual(expected.size(), res.size());
			//Assert::IsTrue(isEqualList(expected, res));


			//expected = { "4" };
			//d1 = Declaration(make_shared<DE::Statement>(), "a");
			//d2 = Declaration(make_shared<DE::Assign>(), "p");
			//so = SelectObject({ d2 });
			//pt = PatternClause(d2, "\"y\"", "_\"x\"_");
			//in = QueryObject({ d1, d2 }, so, {}, {}, {}, { pt });
			//res = QueryEvaluator::evaluate(in, &getFacade);
			//Assert::AreEqual(expected.size(), res.size());
			//Assert::IsTrue(isEqualList(expected, res));


			//expected = { };
			//d1 = Declaration(make_shared<DE::Statement>(), "a");
			//d2 = Declaration(make_shared<DE::Assign>(), "p");
			//so = SelectObject({ d2 });
			//pt = PatternClause(d2, "\"x\"", "_\"x\"_");
			//in = QueryObject({ d1, d2 }, so, {}, {}, {}, { pt });
			//res = QueryEvaluator::evaluate(in, &getFacade);
			//Assert::AreEqual(expected.size(), res.size());
			//Assert::IsTrue(isEqualList(expected, res));

			// Pattern If
			expected = { "3" };
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			d2 = Declaration(make_shared<DE::If>(), "p");
			so = SelectObject({ {d2, ArgType::SYN} });
			shared_ptr<PatternClause> pt = make_shared<PatternClause>(d2, Argument{ StringExp("a") }, Argument{ WildCard() });

			ClauseGroup group1 = ClauseGroup();
			group1.addClauseToGroup(pt);
			ClauseGroup clausesWithNoSynonym = {};
			vector<ClauseGroup> clauseGroupsNotRelatedToSelect = {};
			vector<ClauseGroup> clauseGroupsRelatedToSelect = { group1 };

			optimizedQueryObj = QueryObject(so, { d1, d2 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);

			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);

			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));

			// Pattern While
			expected = { "2" };
			d1 = Declaration(make_shared<DE::Statement>(), "a");
			d2 = Declaration(make_shared<DE::While>(), "p");
			so = SelectObject({ {d2, ArgType::SYN} });
			Argument arg1 = { StringExp("b") };
			Argument arg2 = { WildCard() };
			pt = make_shared<PatternClause>(d2, arg1, arg2);
			group1 = ClauseGroup();
			group1.addClauseToGroup(pt);
			clausesWithNoSynonym = {};
			clauseGroupsNotRelatedToSelect = {};
			clauseGroupsRelatedToSelect = { group1 };

			optimizedQueryObj = QueryObject(so, { d1, d2 }, clausesWithNoSynonym, clauseGroupsNotRelatedToSelect,
				clauseGroupsRelatedToSelect);

			res = QueryEvaluator::evaluate(optimizedQueryObj, &getFacade);
			Assert::AreEqual(expected.size(), res.size());
			Assert::IsTrue(isEqualList(expected, res));

			//SuchThat and Pattern

			// stmt s; assign a; Select s such that Modifies(s, "x") pattern a ("y", _"x"_); 
			//expected = { "4" };
			//d1 = Declaration(make_shared<DE::Statement>(), "s");
			//d2 = Declaration(make_shared<DE::Assign>(), "a");
			//so = SelectObject({ d1 });
			//st = SuchThatClause("Modifies", "s", "\"x\"");
			//pt = PatternClause(d2, "_", "_");
			///*pt = PatternClause(d2, "\"y\"", "_\"x\"_");*/
			//in = QueryObject({ d1, d2 }, so, {}, { st }, {}, { pt });
			//res = QueryEvaluator::evaluate(in, &getFacade);
			//Assert::AreEqual(expected.size(), res.size());
			//Assert::IsTrue(isEqualList(expected, res));

			// stmt s; assign a; Select s such that Uses(s, "y") pattern a ("y", _"x"_);
			// select synonym result is 1,2,3,4,5
			//expected = { "4", "5"};
			//d1 = Declaration(make_shared<DE::Statement>(), "s");
			//d2 = Declaration(make_shared<DE::Assign>(), "a");
			//so = SelectObject({ d1 });
			//st = SuchThatClause("Uses", "s", "\"y\"");   // result is 4,5
			//pt = PatternClause(d2, "\"y\"", "_\"x\"_");  // result is 4, but not common synonym so just check if exists
			//in = QueryObject({ d1, d2 }, so, {}, { st }, {}, { pt });
			//res = QueryEvaluator::evaluate(in, &getFacade);
			//Assert::AreEqual(expected.size(), res.size());
			//Assert::IsTrue(isEqualList(expected, res));
		}
	};
}