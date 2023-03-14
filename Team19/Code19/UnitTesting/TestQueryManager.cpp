#include "stdafx.h"
#include "../source/QP/QueryManager.h"
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

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<>
			static std::wstring ToString<list<string>>(const list<string>& strList) {
				std::string result = "list<string> with strings: '";
				for (string str: strList) {
					result += str + "'\n";
				}
				return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(result);
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting

{
	TEST_CLASS(TestQueryManager)
	{
	public:
		//TODO: FIX BUG HERE
		const string ERROR_MSG = "invalid query";
		PKB* pkb;
		QueryManager* queryManager = new QueryManager();

		PKB* initialisePKB() {
			pkb = new PKB();
			InsertFacade insertFacade = InsertFacade(*pkb);

			PrintStatement s1(1, "print");
			WhileStatement s2(2);
			IfStatement s3(3);
			AssignStatement s4(4);
			ReadStatement s5(6, "read");
			CallStatement s6(7, "call");

			Procedure p1("p1_name");
			Procedure p2("p2_name");
			Procedure p3("p3_name");

			Variable v1("x");
			Variable v2("y");
			Variable v3("z");

			Constant c1("c1");
			Constant c2("c2");

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
			procList.push_back(p1);
			procList.push_back(p2);
			procList.push_back(p3);
			varList.push_back(v1);
			varList.push_back(v2);
			varList.push_back(v3);
			constList.push_back(c1);
			constList.push_back(c2);

			assignPtnList.push_back(Ptn::AssignPtn(4, "y", "+xy", "x+y"));
			assignPtnList.push_back(Ptn::AssignPtn(5, "y", "y", "y"));
			ifPtnList.push_back(Ptn::IfPtn(3, { Variable("a") }));
			whilePtnList.push_back(Ptn::WhilePtn(2, { Variable("b") }));

			for (auto a : stmtList) {
				auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
				insertFacade.insert(requestS);
			}

			for (Ent::Procedure a : procList) {
				auto requestP = std::make_shared<InsertProcedureEntityRequest>(a);
				insertFacade.insert(requestP);
			}

			for (Ent::Variable a : varList) {
				auto requestV = std::make_shared<InsertVariableEntityRequest>(a);
				insertFacade.insert(requestV);
			}

			for (Ent::Constant a : constList) {
				auto requestC = std::make_shared<InsertConstantEntityRequest>(a);
				insertFacade.insert(requestC);
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
			Follows fol1(s1, s2);
			Follows fol2(s2, s3);
			Parent par1(s2, s3);
			Parent par2(s3, s4);
			Uses use1(s4, v1);
			Uses use2(s4, v2);
			Modifies mod1(s4, v1);
			Modifies mod2(s4, v2);
			Calls cal1(p1, p2);
			Calls cal2(p2, p3);

			vector<Rel::Follows> followsList1{};
			vector<Rel::Parent> parentList1{};
			vector<Rel::Uses> usesList1{};
			vector<Rel::Modifies> modifiesList1{};
			vector<Rel::Calls> callsList1{};

			followsList1.push_back(fol1);
			followsList1.push_back(fol2);
			parentList1.push_back(par1);
			parentList1.push_back(par2);
			usesList1.push_back(use1);
			usesList1.push_back(use2);
			modifiesList1.push_back(mod1);
			modifiesList1.push_back(mod2);
			callsList1.push_back(cal1);
			callsList1.push_back(cal2);

			for (Rel::Parent a : parentList1) {
				auto parentRequest = std::make_shared<InsertParentRequest>(a);
				insertFacade.insert(parentRequest);
			}

			for (Rel::Follows a : followsList1) {
				auto followsRequest = std::make_shared<InsertFollowsRequest>(a);
				insertFacade.insert(followsRequest);
			}

			for (Rel::Modifies a : modifiesList1) {
				auto modifiesRequest = std::make_shared<InsertModifiesRequest>(a);
				insertFacade.insert(modifiesRequest);
			}

			for (Rel::Uses a : usesList1) {
				auto usesRequest = std::make_shared<InsertUsesRequest>(a);
				insertFacade.insert(usesRequest);
			}

			for (Rel::Calls a : callsList1) {
				auto callsRequest = std::make_shared<InsertCallsRequest>(a);
				insertFacade.insert(callsRequest);
			}

			//after all insertion then generate star data
			insertFacade.generateSecondaryData();

			return pkb;
		}

		TEST_METHOD_INITIALIZE(Initialize_Pkb) {
			initialisePKB();
		}

		TEST_METHOD(Test_GetQueryResponse_BasicReadSelectNoClauses_) {
			string query = "read r; Select r";
			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			list<string> receivedResult = queryManager->getQueryResponse(query, &getFacade);
			Assert::AreEqual({ "6" }, receivedResult);
		}

		TEST_METHOD(Test_GetQueryResponse_BasicPrintSelectNoClauses_ReturnL1) {
			string query = "print p; Select p";
			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			list<string> receivedResult = queryManager->getQueryResponse(query, &getFacade);
			Assert::AreEqual({ "1" }, receivedResult);
		}

		TEST_METHOD(Test_GetQueryResponse_BasicWhileSelectNoClauses_ReturnL2) {
			string query = "while wh; Select wh";
			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			list<string> receivedResult = queryManager->getQueryResponse(query, &getFacade);
			Assert::AreEqual({ "2" }, receivedResult);
		}

		TEST_METHOD(Test_GetQueryResponse_WithOneSuchThatClause) {
			string query = "print p; while wh; Select wh such that Follows(p, wh)";
			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			list<string> receivedResult = queryManager->getQueryResponse(query, &getFacade);
			Assert::AreEqual({ "2" }, receivedResult);
		}

		TEST_METHOD(Test_GetQueryResponse_BasicIfSelectNoClauses_ReturnL3) {
			string query = "if ifs; Select ifs";
			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			list<string> receivedResult = queryManager->getQueryResponse(query, &getFacade);
			Assert::AreEqual({ "3" }, receivedResult);
		}

		TEST_METHOD(Test_GetQueryResponse_BasicAssignSelectNoClauses_ReturnL4) {
			string query = "assign a; Select a";
			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			list<string> receivedResult = queryManager->getQueryResponse(query, &getFacade);
			Assert::AreEqual({ "4" }, receivedResult);
		}
		TEST_METHOD(Test_GetQueryResponse_BasicVarSyntaxError_ThrowSyntaxError) {
			string query = "variable v Select v";
			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			list<string> receivedResult = queryManager->getQueryResponse(query, &getFacade);
			Assert::AreEqual({ "SyntaxError" }, receivedResult);
		}

		TEST_METHOD(Test_GetQueryResponse_BasicVarSemanticError_ThrowSemanticError) {
			string query = "variable v; Select v such that Uses(_, v)";
			PKB* pkb = initialisePKB();
			GetFacade getFacade = GetFacade(*pkb);
			list<string> receivedResult = queryManager->getQueryResponse(query, &getFacade);
			Assert::AreEqual({ "SemanticError" }, receivedResult);
		}
	};
}