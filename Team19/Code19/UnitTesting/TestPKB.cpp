#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Entity/Entity.h"
#include "../source/Pattern/Pattern.h"
#include "../source/TypeAlias/TypeAlias.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/Request/GetRequests/GetPatternRequests/GetAssignPatternRequest.h"
#include "../source/Request/GetRequests/GetPatternRequests/GetIfPatternRequest.h"
#include "../source/Request/GetRequests/GetPatternRequests/GetWhilePatternRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetAssignStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetCallStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetConstantEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetIfStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetPrintStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetProcedureEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetReadStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetVariableEntityRequest.h"
#include "../source/Request/GetRequests/GetEntityRequests/GetWhileStatementEntityRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetFollowsRequest.h"
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
//#include "TNode.h"
#include <cstddef>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{


    TEST_CLASS(TestPKB)
    {
    public:
        TEST_METHOD(DynamicCastingEntity) {
            Entity* m = dynamic_cast<Entity*>(&Variable("a"));
            Variable* k = dynamic_cast<Variable*>(m);
            Assert::IsFalse(k == nullptr);
        }
        TEST_METHOD(TestVariable){
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);

            std::vector<Ent::Variable> varList{};
            std::vector<Ent::Constant> constList{};
            std::vector<Ent::Procedure> procList{};
            varList.push_back(Variable("x"));
            varList.push_back(Variable("y"));
            varList.push_back(Variable("x"));
            varList.push_back(Variable("z"));
            constList.push_back(Constant("1"));
            constList.push_back(Constant("2"));
            constList.push_back(Constant("2"));
            constList.push_back(Constant("3"));
            procList.push_back(Procedure("main"));
            procList.push_back(Procedure("ok"));
            procList.push_back(Procedure("hospital"));
            procList.push_back(Procedure("die"));

            for (Constant a : constList) {
                auto requestC = std::make_shared<InsertConstantEntityRequest>(a);
                insertFacade.insert(requestC);
            }

            for (Variable a : varList) {
                auto requestV = std::make_shared<InsertVariableEntityRequest>(a);
                insertFacade.insert(requestV);
            }

            for (Procedure a : procList) {
                auto requestP = std::make_shared<InsertProcedureEntityRequest>(a);
                insertFacade.insert(requestP);
            }



            // variable working
            std::string check = "y";

            auto request = std::make_shared<GetVariableEntityRequest>();
            EntityResults res = getFacade.get(request);
            int sizeTvar = res.size();
            Assert::IsTrue(res.find(check) != res.end());
            Assert::AreEqual(sizeTvar, 3);
            
        }

        TEST_METHOD(TestConstant) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ent::Variable> varList{};
            std::vector<Ent::Constant> constList{};
            std::vector<Ent::Procedure> procList{};
            varList.push_back(Variable("x"));
            varList.push_back(Variable("y"));
            varList.push_back(Variable("x"));
            varList.push_back(Variable("z"));
            constList.push_back(Constant("1"));
            constList.push_back(Constant("2"));
            constList.push_back(Constant("2"));
            constList.push_back(Constant("3"));
            procList.push_back(Procedure("main"));
            procList.push_back(Procedure("ok"));
            procList.push_back(Procedure("hospital"));
            procList.push_back(Procedure("die"));

            for (Constant a : constList) {
                auto requestC = std::make_shared<InsertConstantEntityRequest>(a);
                insertFacade.insert(requestC);
            }

            for (Variable a : varList) {
                auto requestV = std::make_shared<InsertVariableEntityRequest>(a);
                insertFacade.insert(requestV);
            }

            for (Procedure a : procList) {
                auto requestP = std::make_shared<InsertProcedureEntityRequest>(a);
                insertFacade.insert(requestP);
            }

            //constant working
            std::string checkc = "3";
            auto request = std::make_shared<GetConstantEntityRequest>();
            int sizeTconst = getFacade.get(request).size();
            Assert::AreEqual(*getFacade.get(request).find(checkc), checkc);
            Assert::AreEqual(sizeTconst, 3);
        }

        TEST_METHOD(TestProcedure) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ent::Variable> varList{};
            std::vector<Ent::Constant> constList{};
            std::vector<Ent::Procedure> procList{};
            varList.push_back(Variable("x"));
            varList.push_back(Variable("y"));
            varList.push_back(Variable("x"));
            varList.push_back(Variable("z"));
            constList.push_back(Constant("1"));
            constList.push_back(Constant("2"));
            constList.push_back(Constant("2"));
            constList.push_back(Constant("3"));
            procList.push_back(Procedure("main"));
            procList.push_back(Procedure("ok"));
            procList.push_back(Procedure("hospital"));
            procList.push_back(Procedure("die"));


            for (Constant a : constList) {
                auto requestC = std::make_shared<InsertConstantEntityRequest>(a);
                insertFacade.insert(requestC);
            }

            for (Variable a : varList) {
                auto requestV = std::make_shared<InsertVariableEntityRequest>(a);
                insertFacade.insert(requestV);
            }

            for (Procedure a : procList) {
                auto requestP = std::make_shared<InsertProcedureEntityRequest>(a);
                insertFacade.insert(requestP);
            }
            // procedures working
            std::string checkp = "die";
            auto request = std::make_shared<GetProcedureEntityRequest>();
            EntityResults res = getFacade.get(request);
            int sizeTproc = res.size();
            Assert::IsTrue(res.find(checkp) != res.end());
            Assert::AreEqual(sizeTproc, 4);
        }

        TEST_METHOD(TestPrintStatement) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ent::Statement> stmtList{};
            stmtList.push_back(PrintStatement(1, "print_1"));
            stmtList.push_back(PrintStatement(2, "print_2"));
            stmtList.push_back(ReadStatement(5, "read_5"));
            stmtList.push_back(ReadStatement(6, "read_6"));
            stmtList.push_back(AssignStatement(3));
            stmtList.push_back(AssignStatement(7));
            stmtList.push_back(PrintStatement(4, "print_4"));
            stmtList.push_back(WhileStatement(9));
            stmtList.push_back(IfStatement(10));
            stmtList.push_back(CallStatement(8, "call_8"));

            for (auto a : stmtList) {
                auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
                insertFacade.insert(requestS);
            }

            // print statements working
            std::string checkprint = "4";
            auto request = std::make_shared<GetPrintStatementEntityRequest>(EntityMode::PRINT);
            EntityResults res = getFacade.get(request);
            Assert::IsTrue(res.find(checkprint)!= res.end());
            int sizePrint = res.size();
            Assert::AreEqual(sizePrint, 3);
        }

        TEST_METHOD(TestReadStatement) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ent::Statement> stmtList{};
            stmtList.push_back(PrintStatement(1, "print_1"));
            stmtList.push_back(PrintStatement(2, "print_2"));
            stmtList.push_back(ReadStatement(5, "read_5"));
            stmtList.push_back(ReadStatement(6, "read_6"));
            stmtList.push_back(AssignStatement(3));
            stmtList.push_back(AssignStatement(7));
            stmtList.push_back(PrintStatement(4, "print_4"));
            stmtList.push_back(WhileStatement(9));
            stmtList.push_back(IfStatement(10));
            stmtList.push_back(CallStatement(8, "call_8"));

            for (auto a : stmtList) {
                auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
                insertFacade.insert(requestS);
            }

            // read statements working
            std::string checkread = "6";
            auto request = std::make_shared<GetReadStatementEntityRequest>(EntityMode::READ);
            Assert::AreEqual(*getFacade.get(request).find(checkread), checkread);
            int sizeRead = getFacade.get(request).size();
            Assert::AreEqual(sizeRead, 2);

        }
        TEST_METHOD(TestAssignmentStatement) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ent::Statement> stmtList{};
            stmtList.push_back(PrintStatement(1, "print_1"));
            stmtList.push_back(PrintStatement(2, "print_2"));
            stmtList.push_back(ReadStatement(5, "read_5"));
            stmtList.push_back(ReadStatement(6, "read_6"));
            stmtList.push_back(AssignStatement(3));
            stmtList.push_back(AssignStatement(7));
            stmtList.push_back(PrintStatement(4, "print_4"));
            stmtList.push_back(WhileStatement(9));
            stmtList.push_back(IfStatement(10));
            stmtList.push_back(CallStatement(8, "call_8"));

            for (auto a : stmtList) {
                auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
                insertFacade.insert(requestS);
            }

            //std::vector<std::string> testName = { "print", "print", "read", "read", "assignment", "assignment", "print" , "while", "if", "call" };
            //std::vector<int> testStmt = { 1, 2, 5, 6 , 3, 7, 4 , 9, 10, 8 };
            //for (int i = 0; i < 10; i++) {
            //    Statement k = Statement(testStmt[i], testName[i]);
            //    insertFacade.insertEntity(k);
            //}

            // assignment statements working
            std::string checkassign = "7";
            auto request = std::make_shared<GetAssignStatementEntityRequest>();
            Assert::AreEqual(*getFacade.get(request).find(checkassign), checkassign);
            int sizeAssignment = getFacade.get(request).size();
            Assert::AreEqual(sizeAssignment, 2);
        }

        TEST_METHOD(TestGetAllStatement) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ent::Statement> stmtList{};
            stmtList.push_back(PrintStatement(1, "print_1"));
            stmtList.push_back(PrintStatement(2, "print_2"));
            stmtList.push_back(ReadStatement(5, "read_5"));
            stmtList.push_back(ReadStatement(6, "read_6"));
            stmtList.push_back(AssignStatement(3));
            stmtList.push_back(AssignStatement(7));
            stmtList.push_back(PrintStatement(4, "print_4"));
            stmtList.push_back(WhileStatement(9));
            stmtList.push_back(IfStatement(10));
            stmtList.push_back(CallStatement(8, "call_8"));

            for (auto a : stmtList) {
                auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
                insertFacade.insert(requestS);
            }

            // get all statements work 
            auto request = std::make_shared<GetStatementEntityRequest>(EntityMode::STATEMENT);
            int totalAssignment = getFacade.get(request).size();
            Assert::AreEqual(totalAssignment, 10);
        
        }

        TEST_METHOD(TestWhileStatement) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ent::Statement> stmtList{};
            stmtList.push_back(PrintStatement(1, "print_1"));
            stmtList.push_back(PrintStatement(2, "print_2"));
            stmtList.push_back(ReadStatement(5, "read_5"));
            stmtList.push_back(ReadStatement(6, "read_6"));
            stmtList.push_back(AssignStatement(3));
            stmtList.push_back(AssignStatement(7));
            stmtList.push_back(PrintStatement(4, "print_4"));
            stmtList.push_back(WhileStatement(9));
            stmtList.push_back(IfStatement(10));
            stmtList.push_back(CallStatement(8, "call_8"));

            for (auto a : stmtList) {
                auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
                insertFacade.insert(requestS);
            }

            std::string check;
            int sizeAssignment;

            // getWhile works
            check = "9";
            auto request = std::make_shared<GetWhileStatementEntityRequest>();
            Assert::AreEqual(*getFacade.get(request).find(check), check);
            sizeAssignment = getFacade.get(request).size();
            Assert::AreEqual(sizeAssignment, 1);
        }

        TEST_METHOD(TestIfStatement) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ent::Statement> stmtList{};
            stmtList.push_back(PrintStatement(1, "print_1"));
            stmtList.push_back(PrintStatement(2, "print_2"));
            stmtList.push_back(ReadStatement(5, "read_5"));
            stmtList.push_back(ReadStatement(6, "read_6"));
            stmtList.push_back(AssignStatement(3));
            stmtList.push_back(AssignStatement(7));
            stmtList.push_back(PrintStatement(4, "print_4"));
            stmtList.push_back(WhileStatement(9));
            stmtList.push_back(IfStatement(10));
            stmtList.push_back(CallStatement(8, "call_8"));

            for (auto a : stmtList) {
                auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
                insertFacade.insert(requestS);
            }

            std::string check;
            int sizeAssignment;

            // getAllIf works
            check = "10";
            auto request = std::make_shared<GetIfStatementEntityRequest>();
            Assert::AreEqual(*getFacade.get(request).find(check), check);
            sizeAssignment = getFacade.get(request).size();
            Assert::AreEqual(sizeAssignment, 1);
        }


        TEST_METHOD(TestIfPattern) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ptn::IfPtn> ptnList{};
            std::vector<std::pair<std::string, std::string>> ans;

            std::vector<Variable> entList{};
            entList.push_back(Variable("a"));
            entList.push_back(Variable("b"));
            entList.push_back(Variable("c"));
            entList.push_back(Variable("d"));
            entList.push_back(Variable("e"));

            std::vector<Variable> entList1 = std::vector(entList.begin(), entList.begin() + 3); // a,b,c
            std::vector<Variable> entList2 = std::vector(entList.begin(), entList.begin() + 4); // a,b,c,d
            std::vector<Variable> entList3 = std::vector(entList.begin(), entList.begin() + 5); // a,b,c,d,e

            ptnList.push_back(Ptn::IfPtn(1, entList1));
            ptnList.push_back(Ptn::IfPtn(2, entList2));
            ptnList.push_back(Ptn::IfPtn(3, entList3));
            
            for (Ptn::IfPtn a : ptnList) {
                auto requestP = std::make_shared<InsertIfPatternRequest>(a);
                insertFacade.insert(requestP);
            }

            std::vector<std::pair<std::string, std::string>> expect =
            { {"1", "a"}, {"2", "b"},{"3","c"} };

            auto request = std::make_shared<GetIfPatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
            PatternResults res = getFacade.get(request);
            int sizePtn = res.size();
            Assert::AreEqual(sizePtn, 12);
     
            for (auto& a : expect) {
                Assert::IsTrue(doesPairExists(a, res));
            }


        }

        TEST_METHOD(TestIfPatternArg1) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ptn::IfPtn> ptnList{};
            std::vector<std::pair<std::string, std::string>> ans;

            std::vector<Variable> entList{};
            entList.push_back(Variable("a"));
            entList.push_back(Variable("b"));
            entList.push_back(Variable("c"));
            entList.push_back(Variable("d"));
            entList.push_back(Variable("e"));

            std::vector<Variable> entList1 = std::vector(entList.begin(), entList.begin() + 3); // a,b,c
            std::vector<Variable> entList2 = std::vector(entList.begin(), entList.begin() + 4); // a,b,c,d
            std::vector<Variable> entList3 = std::vector(entList.begin(), entList.begin() + 5); // a,b,c,d,e

            ptnList.push_back(Ptn::IfPtn(1, entList1));
            ptnList.push_back(Ptn::IfPtn(2, entList2));
            ptnList.push_back(Ptn::IfPtn(3, entList3));

            for (Ptn::IfPtn a : ptnList) {
                auto requestP = std::make_shared<InsertIfPatternRequest>(a);
                insertFacade.insert(requestP);
            }

            std::vector<std::pair<std::string, std::string>> expect =
            { {"2", "d"},{"3","d"} };


            auto request = std::make_shared<GetIfPatternRequest>(PatternMode::ARG1, "d", EMPTY_ARG);
            PatternResults res = getFacade.get(request);
            int sizePtn = res.size();
            Assert::AreEqual(sizePtn, 2);

            for (auto& a : expect) {
                Assert::IsTrue(doesPairExists(a, res));
            }
        }

        TEST_METHOD(TestWhilePattern) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ptn::WhilePtn> ptnList{};
            std::vector<std::pair<std::string, std::string>> ans;
            int setSize;

            std::vector<Variable> entList{};
            entList.push_back(Variable("a"));
            entList.push_back(Variable("b"));
            entList.push_back(Variable("c"));
            entList.push_back(Variable("d"));
            entList.push_back(Variable("e"));

            std::vector<Variable> entList1 = std::vector(entList.begin(), entList.begin() + 3); // a,b,c
            std::vector<Variable> entList2 = std::vector(entList.begin(), entList.begin() + 4); // a,b,c,d
            std::vector<Variable> entList3 = std::vector(entList.begin(), entList.begin() + 5); // a,b,c,d,e

            ptnList.push_back(Ptn::WhilePtn(1, entList1));
            ptnList.push_back(Ptn::WhilePtn(2, entList2));
            ptnList.push_back(Ptn::WhilePtn(3, entList3));

            for (Ptn::WhilePtn a : ptnList) {
                auto requestP = std::make_shared<InsertWhilePatternRequest>(a);
                insertFacade.insert(requestP);
            }

            std::vector<std::pair<std::string, std::string>> expect =
            { {"1", "a"}, {"2", "b"},{"3","c"} };

           auto request = std::make_shared<GetWhilePatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
            PatternResults res = getFacade.get(request);
            setSize = res.size();
            Assert::AreEqual(setSize, 12);

            for (auto& a : expect) {
                Assert::IsTrue(doesPairExists(a, res));
            }
        }

        TEST_METHOD(TestWhilePatternArg1) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ptn::WhilePtn> ptnList{};
            std::vector<std::pair<std::string, std::string>> ans;
            int setSize;

            std::vector<Variable> entList{};
            entList.push_back(Variable("a"));
            entList.push_back(Variable("b"));
            entList.push_back(Variable("c"));
            entList.push_back(Variable("d"));
            entList.push_back(Variable("e"));

            std::vector<Variable> entList1 = std::vector(entList.begin(), entList.begin() + 3); // a,b,c
            std::vector<Variable> entList2 = std::vector(entList.begin(), entList.begin() + 4); // a,b,c,d
            std::vector<Variable> entList3 = std::vector(entList.begin(), entList.begin() + 5); // a,b,c,d,e

            ptnList.push_back(Ptn::WhilePtn(1, entList1));
            ptnList.push_back(Ptn::WhilePtn(2, entList2));
            ptnList.push_back(Ptn::WhilePtn(3, entList3));

            for (Ptn::WhilePtn a : ptnList) {
                auto requestP = std::make_shared<InsertWhilePatternRequest>(a);
                insertFacade.insert(requestP);
            }

            std::vector<std::pair<std::string, std::string>> expect =
            { {"2", "d"},{"3","d"} };

            auto request = std::make_shared<GetWhilePatternRequest>(PatternMode::ARG1, "d", EMPTY_ARG);
            PatternResults res = getFacade.get(request);
            setSize = res.size();
            Assert::AreEqual(setSize, 2);

            for (auto& a : expect) {
                Assert::IsTrue(doesPairExists(a, res));
            }
        }


        TEST_METHOD(TestCallStatement) {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ent::Statement> stmtList{};
            stmtList.push_back(PrintStatement(1, "print_1"));
            stmtList.push_back(PrintStatement(2, "print_2"));
            stmtList.push_back(ReadStatement(5, "read_5"));
            stmtList.push_back(ReadStatement(6, "read_6"));
            stmtList.push_back(AssignStatement(3));
            stmtList.push_back(AssignStatement(7));
            stmtList.push_back(PrintStatement(4, "print_4"));
            stmtList.push_back(WhileStatement(9));
            stmtList.push_back(IfStatement(10));
            stmtList.push_back(CallStatement(8, "call_8"));

            for (auto a : stmtList) {
                auto requestS = std::make_shared<InsertStatementEntityRequest>(a);
                insertFacade.insert(requestS);
            }

            std::string check;
            int sizeAssignment;

            // getCall works statements 
            check = "8";
            auto request = std::make_shared<GetCallStatementEntityRequest>(EntityMode::CALL);
            Assert::AreEqual(*getFacade.get(request).find(check), check);
            sizeAssignment = getFacade.get(request).size();
            Assert::AreEqual(sizeAssignment, 1);


        }
        TEST_METHOD(TestGetAllAssign)
        {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            // limitations: 
                // can only do exact matching (Sufficient for Milestone 1)
                // cannot deal with spacing 
                // cannot deal with spacing for exact  but can deal with dups ( 1 stmt no linked to 1 rhs and 1 stmtNo linked to 1 lhs)
            std::vector<Ptn::AssignPtn> ptnList{};
            std::vector<std::pair<std::string, std::string>> ans;
            int setSize;
            std::pair<std::string,std::string> expect;
            
            ptnList.push_back(Ptn::AssignPtn(1, "x", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 1. x = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(2, "y", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 2. y = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(3, "x", "(+(*(m)(y))(z))", "(((m)*(y))+(z))"));    // 3. x = m*y+z
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // 4. y = y+z*m
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // is a duplicate and wont accept
            ptnList.push_back(Ptn::AssignPtn(5, "xy", "(+(y)(z))", "((y)+(z))"));               // 5. xy = y+z
            ptnList.push_back(Ptn::AssignPtn(6, "xy", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));   // 6. xy = y+z*m
            ptnList.push_back(Ptn::AssignPtn(7, "xy", "(+(m)(+(y)(z)))", "((m)+((y)+(z)))"));   // 7. xy = m+(y+z)

            for (Ptn::AssignPtn a : ptnList) {
                auto requestP = std::make_shared<InsertAssignPatternRequest>(a);
                insertFacade.insert(requestP);
            }

            auto request = std::make_shared<GetAssignPatternRequest>(PatternMode::NO_ARG, EMPTY_ARG, EMPTY_ARG);
            PatternResults res = getFacade.get(request);
            setSize = res.size();    // returns stmtNo, lhs
            Assert::AreEqual(setSize, 7);       
            expect = std::make_pair("6", "xy");
            Assert::IsTrue(doesPairExists(expect, res));
        }

        TEST_METHOD(TestGetAssignByArg1)
        {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            // limitations: 
                // can only do exact matching (Sufficient for Milestone 1)
                // cannot deal with spacing 
                // cannot deal with spacing for exact  but can deal with dups ( 1 stmt no linked to 1 rhs and 1 stmtNo linked to 1 lhs)
            std::vector<Ptn::AssignPtn> ptnList{};
            int setSize;

            ptnList.push_back(Ptn::AssignPtn(1, "x", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 1. x = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(2, "y", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 2. y = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(3, "x", "(+(*(m)(y))(z))", "(((m)*(y))+(z))"));    // 3. x = m*y+z
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // 4. y = y+z*m
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // is a duplicate and wont accept
            ptnList.push_back(Ptn::AssignPtn(5, "xy", "(+(y)(z))", "((y)+(z))"));               // 5. xy = y+z
            ptnList.push_back(Ptn::AssignPtn(6, "xy", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));   // 6. xy = y+z*m
            ptnList.push_back(Ptn::AssignPtn(7, "xy", "(+(m)(+(y)(z)))", "((m)+((y)+(z)))"));   // 7. xy = m+(y+z)

            for (Ptn::AssignPtn a : ptnList) {
                auto requestP = std::make_shared<InsertAssignPatternRequest>(a);
                insertFacade.insert(requestP);
            }

            // check if only exact arg 1 is returned
                // this means that 5 and 6 should not be return as x is not an exact but a partial
            auto request = std::make_shared<GetAssignPatternRequest>(PatternMode::ARG1, "x", EMPTY_ARG);
            PatternResults res = getFacade.get(request);
            setSize = res.size();
            Assert::AreEqual(setSize, 2);
            std::vector<std::pair<std::string, std::string>> expect =
                { {"1", "x"} ,{"3","x"} };
            for (auto& a : expect) {
                Assert::IsTrue(doesPairExists(a, res));
            }
        }
        TEST_METHOD(TestGetAssignByCompleteArg2)
        {
            // limitations: 
                // cannot deal with spacing (Assumption that PQL sends the proper format)
                    // this only applies for Complete arg2. functions
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ptn::AssignPtn> ptnList{};
            int setSize;

            ptnList.push_back(Ptn::AssignPtn(1, "x", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 1. x = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(2, "y", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 2. y = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(3, "x", "(+(*(m)(y))(z))", "(((m)*(y))+(z))"));    // 3. x = m*y+z
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // 4. y = y+z*m
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // is a duplicate and wont accept
            ptnList.push_back(Ptn::AssignPtn(5, "xy", "(+(y)(z))", "((y)+(z))"));               // 5. xy = y+z
            ptnList.push_back(Ptn::AssignPtn(6, "xy", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));   // 6. xy = y+z*m
            ptnList.push_back(Ptn::AssignPtn(7, "xy", "(+(m)(+(y)(z)))", "((m)+((y)+(z)))"));   // 7. xy = m+(y+z)

            for (Ptn::AssignPtn a : ptnList) {
                auto requestP = std::make_shared<InsertAssignPatternRequest>(a);
                insertFacade.insert(requestP);
            }

            //GetPatternRequest request = GetPatternRequest(PatternType::ASSIGNMENT_PATTERN, PatternMode::COMPLETE_ARG2, EMPTY_ARG, "y +          z");
            //PatternResults res = getFacade.get(request);
            //setSize = res.size();
            //Assert::AreEqual(setSize, 1);

            // should only return 5 and not 6. even though 
            auto request = std::make_shared<GetAssignPatternRequest>(PatternMode::COMPLETE_ARG2, EMPTY_ARG, "y+z");
            PatternResults res = getFacade.get(request);
            setSize = res.size();
            Assert::AreEqual(setSize, 1);
            std::vector<std::pair<std::string, std::string>> expect =
            { {"5","xy"}};
            for (auto& a : expect) {
                Assert::IsTrue(doesPairExists(a, res));
            }
        }
        TEST_METHOD(TestGetAssignBySubArg2)
        {
            // limitations: 
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ptn::AssignPtn> ptnList{};
            int setSize;

            ptnList.push_back(Ptn::AssignPtn(1, "x", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 1. x = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(2, "y", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 2. y = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(3, "x", "(+(*(m)(y))(z))", "(((m)*(y))+(z))"));    // 3. x = m*y+z
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // 4. y = y+z*m
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // is a duplicate and wont accept
            ptnList.push_back(Ptn::AssignPtn(5, "xy", "(+(y)(z))", "((y)+(z))"));               // 5. xy = y+z
            ptnList.push_back(Ptn::AssignPtn(6, "xy", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));   // 6. xy = y+z*m
            ptnList.push_back(Ptn::AssignPtn(7, "xy", "(+(m)(+(y)(z)))", "((m)+((y)+(z)))"));   // 7. xy = m+(y+z)

            for (Ptn::AssignPtn a : ptnList) {
                auto requestP = std::make_shared<InsertAssignPatternRequest>(a);
                insertFacade.insert(requestP);
            }

            auto request = std::make_shared<GetAssignPatternRequest>(PatternMode::SUB_ARG2, EMPTY_ARG, "y+z");
            PatternResults res = getFacade.get(request);
            setSize = res.size();
            Assert::AreEqual(setSize, 2);
            std::vector<std::pair<std::string, std::string>> expect =
            { {"5","xy"}, {"7","xy"} };
            for (auto& a : expect) {
                Assert::IsTrue(doesPairExists(a, res));
            }
        }

        TEST_METHOD(TestGetAssignByArg1CompleteArg2)
        {
            // limitations: 
                // cannot deal with spacing (Assumption that PQL sends the proper format)
                    // this only applies for Complete arg2. functions
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ptn::AssignPtn> ptnList{};
            int setSize;

            ptnList.push_back(Ptn::AssignPtn(1, "x", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 1. x = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(2, "y", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 2. y = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(3, "x", "(+(*(m)(y))(z))", "(((m)*(y))+(z))"));    // 3. x = m*y+z
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // 4. y = y+z*m
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // is a duplicate and wont accept
            ptnList.push_back(Ptn::AssignPtn(5, "xy", "(+(y)(z))", "((y)+(z))"));               // 5. xy = y+z
            ptnList.push_back(Ptn::AssignPtn(6, "xy", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));   // 6. xy = y+z*m
            ptnList.push_back(Ptn::AssignPtn(7, "xy", "(+(m)(+(y)(z)))", "((m)+((y)+(z)))"));   // 7. xy = m+(y+z)

            for (Ptn::AssignPtn a : ptnList) {
                auto requestP = std::make_shared<InsertAssignPatternRequest>(a);
                insertFacade.insert(requestP);
            }

           auto request = std::make_shared<GetAssignPatternRequest>(PatternMode::ARG1_COMPLETE_ARG2, "x", "1/y+z");
            PatternResults res = getFacade.get(request);
            setSize = res.size();
            Assert::AreEqual(setSize, 1);
            std::vector<std::pair<std::string, std::string>> expect =
            { {"1", "x"} };
            for (auto& a : expect) {
                Assert::IsTrue(doesPairExists(a, res));
            }
        }
        TEST_METHOD(TestGetAssignByArg1SubArg2)
        {
            // limitations: 
                // can only do exact matching (Sufficient for Milestone 1)
                // cannot deal with spacing 
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);
            std::vector<Ptn::AssignPtn> ptnList{};
            int setSize;

            ptnList.push_back(Ptn::AssignPtn(1, "x", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 1. x = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(2, "y", "(+(/(1)(y))(z))", "(((1)/(y))+(z))"));    // 2. y = 1/y+z
            ptnList.push_back(Ptn::AssignPtn(3, "x", "(+(*(m)(y))(z))", "(((m)*(y))+(z))"));    // 3. x = m*y+z
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // 4. y = y+z*m
            ptnList.push_back(Ptn::AssignPtn(4, "y", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));    // is a duplicate and wont accept
            ptnList.push_back(Ptn::AssignPtn(5, "xy", "(+(y)(z))", "((y)+(z))"));               // 5. xy = y+z
            ptnList.push_back(Ptn::AssignPtn(6, "xy", "(+(y)(*(z)(m)))", "((y)+((z)*(m)))"));   // 6. xy = y+z*m
            ptnList.push_back(Ptn::AssignPtn(7, "xy", "(+(m)(+(y)(z)))", "((m)+((y)+(z)))"));   // 7. xy = m+(y+z)

            for (Ptn::AssignPtn a : ptnList) {
                auto requestP = std::make_shared<InsertAssignPatternRequest>(a);
                insertFacade.insert(requestP);
            }

            // should only return 6 and not 4 bhecause of the first argument

            auto request = std::make_shared<GetAssignPatternRequest>(PatternMode::ARG1_SUB_ARG2, "xy", "z*m");
            PatternResults res = getFacade.get(request);
            setSize = res.size();
            Assert::AreEqual(setSize, 1);
            std::vector<std::pair<std::string, std::string>> expect =
            { {"6","xy"} };
            for (auto& a : expect) {
                Assert::IsTrue(doesPairExists(a, res));
            }
        }


        TEST_METHOD(InsertRelationships)
        {
            PKB pkb = PKB();
            InsertFacade insertFacade = InsertFacade(pkb);
            GetFacade getFacade = GetFacade(pkb);

            Statement s1(1, "print");
            Statement s2(2, "while");
            Statement s3(3, "if");
            Statement s4(4, "assign");
            Statement s5(5, "assign");

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

            
            auto request_1 = std::make_shared<GetFollowsRequest>(Mode::ALL);
            getFacade.get(request_1);

            std::vector<std::pair<std::string, std::string>> check;
            std::pair<std::string, std::string> relPair;
            std::pair<std::string, std::string> checkPair;
            int size;

            check = { {"1","2"}, {"2","3"}, {"3","5"}, {"1","5"}, {"4","5"} };
            size = check.size();
            auto request_2 = std::make_shared<GetFollowsRequest>(Mode::ALL);
            RelationshipResults results = getFacade.get(request_2);
            for (int i = 0; i < size; i++) {
                relPair = *std::next(results.begin(), i);
                checkPair = check[i];
                Assert::AreEqual(relPair.first, checkPair.first);
                Assert::AreEqual(relPair.second, checkPair.second);
            }


            check = { {"1","2"}, {"1", "5"} };
            size = check.size();
            auto request_3 = std::make_shared<GetFollowsRequest>("1", Mode::FILTER_ARG_1);
            results = getFacade.get(request_3);
            for (int i = 0; i < size; i++) {
                relPair = *std::next(results.begin(), i);
                checkPair = check[i];
                Assert::AreEqual(relPair.first, checkPair.first);
                Assert::AreEqual(relPair.second, checkPair.second);
            }

            check = { {"3","5"}, {"4", "5"}, {"1", "5"} };
            size = check.size();
            auto request_4 = std::make_shared<GetFollowsRequest>("5", Mode::FILTER_ARG_2);
            results = getFacade.get(request_4);
            for (int i = 0; i < size; i++) {
                relPair = *std::next(results.begin(), i);
                checkPair = check[i];
                Assert::AreEqual(relPair.first, checkPair.first);
                Assert::AreEqual(relPair.second, checkPair.second);
            }

        }

        bool doesPairExists(std::pair<std::string,std::string> pairCheck, PatternResults set) {
            return find(set.begin(), set.end(), pairCheck) != set.end();
        }
    };
}