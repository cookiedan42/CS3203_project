#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB/InsertFacade.h"
#include "../source/PKB/GetFacade.h"
#include "../source/Relationship/Relationship.h"
#include "../source/Entity/Entity.h"
#include "../source/TypeAlias/TypeAlias.h"
#include "../source/Definitions/Definitions.h"
#include "../source/Cfg/Cfg.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/CfgExtractor.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetFollowsRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetParentRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetModifiesRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetUsesRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetCallsRequest.h"
#include "../source/Request/GetRequests/GetRelationshipRequests/GetNextRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertCallsRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertFollowsRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertModifiesRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertParentRequest.h"
#include "../source/Request/InsertRequests/InsertRelationshipRequests/InsertUsesRequest.h"


#include <cstddef>
#include <iostream>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(Test_PKB_RelationshipStorage)
    {
    public:
        PKB* pkb = new PKB();
        InsertFacade insertFacade = InsertFacade(*pkb);
        GetFacade getFacade = GetFacade(*pkb);

        Statement s1 = Statement(1, "print");
        Statement s2 = Statement(2, "while");
        Statement s3 = Statement(3, "if");
        Statement s4 = Statement(4, "assign");
        Statement s5 = Statement(5, "assign");

        Variable v1 = Variable("x");
        Variable v2 = Variable("x");
        Variable v3 = Variable("y");
        Variable v4 = Variable("x");

        Procedure p1 = Procedure("main");
        Procedure p2 = Procedure("p2");
        Procedure p3 = Procedure("p3");
        Procedure p4 = Procedure("p4");
        Procedure p5 = Procedure("p5");

        TEST_METHOD(TestFollows)
        {            
            Follows f1(s1, s2);
            Follows f2(s2, s3);
            Follows f3(s3, s5);
            Follows f4(s4, s5);

            vector<Rel::Follows> followsList{};

            followsList.push_back(f1);
            followsList.push_back(f2);
            followsList.push_back(f3);
            followsList.push_back(f4);

            for (Follows a : followsList) {
                auto followsRequest = std::make_shared<InsertFollowsRequest>(a);
                insertFacade.insert(followsRequest);
            }


            //Test for get all Follows
            auto request_all = std::make_shared<GetFollowsRequest>(Mode::ALL);
            RelationshipResults allFollows = getFacade.get(request_all);
            int expected_size = 4;
            int actual_size = static_cast<int>(allFollows.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesFollowsPairExists(f1, allFollows), true);
            Assert::AreEqual(doesFollowsPairExists(f2, allFollows), true);
            Assert::AreEqual(doesFollowsPairExists(f3, allFollows), true);

            //Test for get Follows by arg1
            auto request_arg1 = std::make_shared<GetFollowsRequest>("1", Mode::FILTER_ARG_1);
            RelationshipResults followsS1 = getFacade.get(request_arg1);
            expected_size = 1;
            actual_size = static_cast<int>(followsS1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesFollowsPairExists(f1, followsS1), true);
            Assert::AreEqual(doesFollowsPairExists(f2, followsS1), false);

            //Test for get Followed by arg2
            auto request_arg2 = std::make_shared<GetFollowsRequest>("5", Mode::FILTER_ARG_2);
            RelationshipResults followedByS5 = getFacade.get(request_arg2);
            expected_size = 2;
            actual_size = static_cast<int>(followedByS5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesFollowsPairExists(f1, followedByS5), false);
            Assert::AreEqual(doesFollowsPairExists(f3, followedByS5), true);
            Assert::AreEqual(doesFollowsPairExists(f4, followedByS5), true);

            //Test for get Follows using arg1 and arg2
            auto request_arg1_arg2 = std::make_shared<GetFollowsRequest>("1", "2", Mode::FILTER_ARG1_ARG2);
            RelationshipResults followsS1S2 = getFacade.get(request_arg1_arg2);
            expected_size = 1;
            actual_size = static_cast<int>(followsS1S2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesFollowsPairExists(f1, followsS1S2), true);
            Assert::AreEqual(doesFollowsPairExists(f3, followsS1S2), false);
        }

        TEST_METHOD(TestParent)
        {
            Parent p1(s1, s2);
            Parent p2(s1, s3);
            Parent p3(s3, s2);
            Parent p4(s3, s5);

            vector<Rel::Parent> parentList{};

            parentList.push_back(p1);
            parentList.push_back(p2);
            parentList.push_back(p3);
            parentList.push_back(p4);

            for (Parent a : parentList) {
                auto parentRequest = std::make_shared<InsertParentRequest>(a);
                insertFacade.insert(parentRequest);
            }

            //Test for get all Parent
            auto request_all = std::make_shared<GetParentRequest>(Mode::ALL);
            RelationshipResults allParent = getFacade.get(request_all);
            int expected_size = 4;
            int actual_size = static_cast<int>(allParent.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesParentPairExists(p1, allParent), true);
            Assert::AreEqual(doesParentPairExists(p2, allParent), true);
            Assert::AreEqual(doesParentPairExists(p3, allParent), true);
            Assert::AreEqual(doesParentPairExists(p4, allParent), true);


            //Test for get Parent by arg1
            auto request_arg1 = std::make_shared<GetParentRequest>("1", Mode::FILTER_ARG_1);
            RelationshipResults parentS1 = getFacade.get(request_arg1);
            expected_size = 2;
            actual_size = static_cast<int>(parentS1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesParentPairExists(p1, parentS1), true);
            Assert::AreEqual(doesParentPairExists(p2, parentS1), true);

            //Test for get Parent by arg2
            auto request_arg2 = std::make_shared<GetParentRequest>("2", Mode::FILTER_ARG_2);
            RelationshipResults childS2 = getFacade.get(request_arg2);
            expected_size = 2;
            actual_size = static_cast<int>(childS2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesParentPairExists(p1, childS2), true);
            Assert::AreEqual(doesParentPairExists(p3, childS2), true);
            Assert::AreEqual(doesParentPairExists(p2, childS2), false);

            //Test for get Parent using arg1 and arg2
            auto request_arg1_arg2 = std::make_shared<GetParentRequest>("1", "2", Mode::FILTER_ARG1_ARG2);
            RelationshipResults parentS1S2 = getFacade.get(request_arg1_arg2);
            expected_size = 1;
            actual_size = static_cast<int>(parentS1S2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesParentPairExists(p1, parentS1S2), true);
            Assert::AreEqual(doesParentPairExists(p2, parentS1S2), false);
        }
        TEST_METHOD(TestModifies)
        {
            Modifies m1(p1, v1);    //(main, x)
            Modifies m2(p1, v2);    //(main, x)
            Modifies m3(s4, v2);    //(4, x)
            Modifies m4(s4, v3);    //(4, y)
            Modifies m5(s5, v4);    //(5, x)

            vector<Rel::Modifies> modifiesList{};

            modifiesList.push_back(m1);
            modifiesList.push_back(m2);
            modifiesList.push_back(m3);
            modifiesList.push_back(m4);
            modifiesList.push_back(m5);

            for (Modifies a : modifiesList) {
                auto modifiesRequest = std::make_shared<InsertModifiesRequest>(a);
                insertFacade.insert(modifiesRequest);
            }

            //Test for get all Modifies
            auto request_all = std::make_shared<GetModifiesRequest>(Mode::ALL);
            RelationshipResults allModifies = getFacade.get(request_all);
            int expected_size = 4; //This is 4 instead of 5, because main modifies "x" twice but will be captured only once.
            int actual_size = static_cast<int>(allModifies.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesModifiesPairExists(m1, allModifies, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m2, allModifies, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m3, allModifies, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m4, allModifies, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m5, allModifies, "statement"), true);

            //Test for get Modifies by arg1, arg1Type = "procedure"
            auto request_arg1_p = std::make_shared<GetModifiesRequest>("main", Mode::FILTER_ARG_1, Arg1Type::PROCEDURE);
            RelationshipResults modifiesP1 = getFacade.get(request_arg1_p);
            expected_size = 1;
            actual_size = static_cast<int>(modifiesP1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m1, modifiesP1, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m2, modifiesP1, "procedure"), true);

            //Test for get Modifies by arg1, arg1Type = "statement"
            auto request_arg1_s = std::make_shared<GetModifiesRequest>("4", Mode::FILTER_ARG_1, Arg1Type::STATEMENT);
            RelationshipResults modifiesS4 = getFacade.get(request_arg1_s);
            expected_size = 2;
            actual_size = static_cast<int>(modifiesS4.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m3, modifiesS4, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m4, modifiesS4, "statement"), true);

            //Test for get Modifies by arg2, arg1Type = "procedure"
            //Which procedures modified the arg2? Modifies(p, arg2)
            auto request_arg2_p = std::make_shared<GetModifiesRequest>("x", Mode::FILTER_ARG_2, Arg1Type::PROCEDURE);
            RelationshipResults procModifiedX = getFacade.get(request_arg2_p);
            expected_size = 1;
            actual_size = static_cast<int>(procModifiedX.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m1, procModifiedX, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m1, procModifiedX, "procedure"), true);

            //Test for get Modifies by arg2, arg1Type = "procedure"
            //Which statements modified the arg2? Modifies(s, arg2)
            auto request_arg2_s = std::make_shared<GetModifiesRequest>("x", Mode::FILTER_ARG_2, Arg1Type::STATEMENT);
            RelationshipResults stmtModifiedX = getFacade.get(request_arg2_s);
            expected_size = 2;
            actual_size = static_cast<int>(stmtModifiedX.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m3, stmtModifiedX, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m5, stmtModifiedX, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m4, stmtModifiedX, "statement"), false);

            //Test for get Modifies using arg1 and arg2, arg1Type = "procedure"
            //Modifies(main, x)
            auto request_arg1_arg2_p = std::make_shared<GetModifiesRequest>("main", "x", Mode::FILTER_ARG1_ARG2, Arg1Type::PROCEDURE);
            RelationshipResults modifiesP1V1 = getFacade.get(request_arg1_arg2_p);
            expected_size = 1;
            actual_size = static_cast<int>(modifiesP1V1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m1, modifiesP1V1, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m2, modifiesP1V1, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m3, modifiesP1V1, "procedure"), false);

            //Test for get Modifies using arg1 and arg2, arg1Type = "statement"
            //Modifies(4, y)
            auto request_arg1_arg2_s = std::make_shared<GetModifiesRequest>("4", "y", Mode::FILTER_ARG1_ARG2, Arg1Type::STATEMENT);
            RelationshipResults modifiesS4V3 = getFacade.get(request_arg1_arg2_s);
            expected_size = 1;
            actual_size = static_cast<int>(modifiesS4V3.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m4, modifiesS4V3, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m5, modifiesS4V3, "statement"), false);
        }

        TEST_METHOD(TestUses)
        {
            Uses u1(p1, v1);    //(main, x)
            Uses u2(p1, v2);    //(main, x)
            Uses u3(s4, v2);    //(4, x)
            Uses u4(s4, v3);    //(4, y)
            Uses u5(s5, v4);    //(5, x)

            vector<Rel::Uses> usesList{};
            usesList.push_back(u1);
            usesList.push_back(u2);
            usesList.push_back(u3);
            usesList.push_back(u4);
            usesList.push_back(u5);

            for (Uses a : usesList) {
                auto usesRequest = std::make_shared<InsertUsesRequest>(a);
                insertFacade.insert(usesRequest);
            }

            //Test for get all Uses
            auto request_all = std::make_shared<GetUsesRequest>(Mode::ALL);
            RelationshipResults allUses = getFacade.get(request_all);
            int expected_size = 4; //This is 4 instead of 5, because main uses "x" twice but will be captured only once.
            int actual_size = static_cast<int>(allUses.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesUsesPairExists(u1, allUses, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u2, allUses, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u3, allUses, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u4, allUses, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u5, allUses, "statement"), true);

            //Test for get Uses by arg1, arg1Type = "procedure"
            auto request_arg1_p = std::make_shared<GetUsesRequest>("main", Mode::FILTER_ARG_1, Arg1Type::PROCEDURE);
            RelationshipResults usesP1 = getFacade.get(request_arg1_p);
            expected_size = 1;
            actual_size = static_cast<int>(usesP1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u1, usesP1, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u2, usesP1, "procedure"), true);

            //Test for get Uses by arg1, arg1Type = "statement"
            auto request_arg1_s = std::make_shared<GetUsesRequest>("4", Mode::FILTER_ARG_1, Arg1Type::STATEMENT);
            RelationshipResults usesS4 = getFacade.get(request_arg1_s);
            expected_size = 2;
            actual_size = static_cast<int>(usesS4.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u3, usesS4, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u4, usesS4, "statement"), true);

            //Test for get Uses by arg2, arg1Type = "procedure"
            //Which procedures uses the arg2? Uses(p, arg2)
            auto request_arg2_p = std::make_shared<GetUsesRequest>("x", Mode::FILTER_ARG_2, Arg1Type::PROCEDURE);
            RelationshipResults procUsesX = getFacade.get(request_arg2_p);
            expected_size = 1;
            actual_size = static_cast<int>(procUsesX.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u1, procUsesX, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u1, procUsesX, "procedure"), true);

            //Test for get Uses by arg2, arg1Type = "procedure"
            //Which statements uses the arg2? Uses(s, arg2)
            auto request_arg2_s = std::make_shared<GetUsesRequest>("x", Mode::FILTER_ARG_2, Arg1Type::STATEMENT);
            RelationshipResults stmtUsesX = getFacade.get(request_arg2_s);
            expected_size = 2;
            actual_size = static_cast<int>(stmtUsesX.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u3, stmtUsesX, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u5, stmtUsesX, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u4, stmtUsesX, "statement"), false);

            //Test for get Uses using arg1 and arg2, arg1Type = "procedure"
            //Uses(main, x)
            auto request_arg1_arg2_p = std::make_shared<GetUsesRequest>("main", "x", Mode::FILTER_ARG1_ARG2, Arg1Type::PROCEDURE);
            RelationshipResults usesP1V1 = getFacade.get(request_arg1_arg2_p);
            expected_size = 1;
            actual_size = static_cast<int>(usesP1V1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u1, usesP1V1, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u2, usesP1V1, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u3, usesP1V1, "procedure"), false);

            //Test for get Uses using arg1 and arg2, arg1Type = "statement"
            //Uses(4, y)
            auto request_arg1_arg2_s = std::make_shared<GetUsesRequest>("4", "y", Mode::FILTER_ARG1_ARG2, Arg1Type::STATEMENT);
            RelationshipResults usesS4V3 = getFacade.get(request_arg1_arg2_s);
            expected_size = 1;
            actual_size = static_cast<int>(usesS4V3.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u4, usesS4V3, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u5, usesS4V3, "statement"), false);
        }

        TEST_METHOD(TestFollowsStar)
        {
            //Test with the following code structure
            //1     stmt;
            //2     stmt;
            //3     if () {
            //4         stmt;
            //5		    stmt;
            //      }
            //6     stmt;
            //      }

            //Follow*
            //(1,2), (1,3), (1,6)
            //(2,3), (2,6)
            //(3,6)
            //(4,5)

            
            //For star, reset and test again
            PKB* pkb = new PKB();
            InsertFacade insertFacade = InsertFacade(*pkb);
            GetFacade getFacade = GetFacade(*pkb);

            Statement s1(1, "assign");
            Statement s2(2, "assign");
            Statement s3(3, "if");
            Statement s4(4, "assign");
            Statement s5(5, "assign");
            Statement s6(6, "assign");

            Follows f1(s1, s2);
            Follows f2(s2, s3);
            Follows f3(s3, s6);
            Follows f4(s4, s5);

            vector<Rel::Follows> followsList{};

            followsList.push_back(f1);
            followsList.push_back(f2);
            followsList.push_back(f3);
            followsList.push_back(f4);

            for (Follows a : followsList) {
                auto followsRequest = std::make_shared<InsertFollowsRequest>(a);
                insertFacade.insert(followsRequest);
            }
 
            //after all insertion then generate star data
            insertFacade.generateSecondaryData();

            //Test for get all Follows Star
            auto request_all = std::make_shared<GetFollowsRequest>(Mode::ALL, STAR);
            RelationshipResults allFollowsStar = getFacade.get(request_all);
            int expected_size = 7;
            int actual_size = static_cast<int>(allFollowsStar.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesStarRelationExists("1", "2", allFollowsStar), true);
            Assert::AreEqual(doesStarRelationExists("1", "6", allFollowsStar), true);
            Assert::AreEqual(doesStarRelationExists("2", "6", allFollowsStar), true);
            Assert::AreEqual(doesStarRelationExists("1", "5", allFollowsStar), false);

            //Test for get Follows* by arg1
            auto request_arg1 = std::make_shared<GetFollowsRequest>("1", Mode::FILTER_ARG_1, STAR);
            RelationshipResults followsStarS1 = getFacade.get(request_arg1);
            expected_size = 3;
            actual_size = static_cast<int>(followsStarS1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "2", followsStarS1), true);
            Assert::AreEqual(doesStarRelationExists("1", "3", followsStarS1), true);
            Assert::AreEqual(doesStarRelationExists("1", "6", followsStarS1), true);
            Assert::AreEqual(doesStarRelationExists("2", "6", followsStarS1), false);

            //Test for get Follows* by arg2
            auto request_arg2 = std::make_shared<GetFollowsRequest>("6", Mode::FILTER_ARG_2, STAR);
            RelationshipResults followedStarByS6 = getFacade.get(request_arg2);
            expected_size = 3;
            actual_size = static_cast<int>(followedStarByS6.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "6", followedStarByS6), true);
            Assert::AreEqual(doesStarRelationExists("2", "6", followedStarByS6), true);
            Assert::AreEqual(doesStarRelationExists("3", "6", followedStarByS6), true);
            Assert::AreEqual(doesStarRelationExists("2", "3", followedStarByS6), false);


            //Test for get Follows* using arg1 and arg2
            auto request_arg1_arg2 = std::make_shared<GetFollowsRequest>("2", "6", Mode::FILTER_ARG1_ARG2, STAR);
            RelationshipResults followsStarS2S6 = getFacade.get(request_arg1_arg2);
            expected_size = 1;
            actual_size = static_cast<int>(followsStarS2S6.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("2", "6", followsStarS2S6), true);
            Assert::AreEqual(doesStarRelationExists("2", "3", followsStarS2S6), false);

        }

        TEST_METHOD(TestParentStar)
        {
            //Test with the following code structure
            //1   while () {
            //2	    stmt;
            //3     stmt;
            //4     if () {
            //5         stmt;
            //6       	while () {
            //7             stmt;
            //          }
            //8		    stmt;}
            //9     stmt;}

            //Parent*
            //(1,2), (1,3), (1,4), (1,9), (1,5), (1,6), (1,8), (1,7)
            //(4,5), (4,6), (4,8), (4,7)
            //(6,7)
            
            //For star, reset and test again
            PKB* pkb = new PKB();
            InsertFacade insertFacade = InsertFacade(*pkb);
            GetFacade getFacade = GetFacade(*pkb);

            Statement s1(1, "while");
            Statement s2(2, "assign");
            Statement s3(3, "assign");
            Statement s4(4, "if");
            Statement s5(5, "assign");
            Statement s6(6, "while");
            Statement s7(7, "assign");
            Statement s8(8, "assign");
            Statement s9(9, "assign");

            Parent p1(s1, s2);
            Parent p2(s1, s3);
            Parent p3(s1, s4);
            Parent p4(s1, s9);
            Parent p5(s4, s5);
            Parent p6(s4, s6);
            Parent p7(s4, s8);
            Parent p8(s6, s7);

            vector<Rel::Parent> parentList{};

            parentList.push_back(p1);
            parentList.push_back(p2);
            parentList.push_back(p3);
            parentList.push_back(p4);
            parentList.push_back(p5);
            parentList.push_back(p6);
            parentList.push_back(p7);
            parentList.push_back(p8);

            for (Parent a : parentList) {
                auto parentRequest = std::make_shared<InsertParentRequest>(a);
                insertFacade.insert(parentRequest);
            }

            //after all insertion then generate star data
            insertFacade.generateSecondaryData();

            //Test for get all Parent Star
            auto request_all = std::make_shared<GetParentRequest>(Mode::ALL, STAR);
            RelationshipResults allParentStar = getFacade.get(request_all);
            int expected_size = 13;
            int actual_size = static_cast<int>(allParentStar.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesStarRelationExists("1", "2", allParentStar), true);
            Assert::AreEqual(doesStarRelationExists("1", "7", allParentStar), true);
            Assert::AreEqual(doesStarRelationExists("4", "7", allParentStar), true);
            Assert::AreEqual(doesStarRelationExists("4", "9", allParentStar), false);


            //Test for get Parent* by arg1
            auto request_arg1 = std::make_shared<GetParentRequest>("1", Mode::FILTER_ARG_1, STAR);
            RelationshipResults parentStarS1 = getFacade.get(request_arg1);
            expected_size = 8;
            actual_size = static_cast<int>(parentStarS1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "2", parentStarS1), true);
            Assert::AreEqual(doesStarRelationExists("1", "7", parentStarS1), true);
            Assert::AreEqual(doesStarRelationExists("4", "7", parentStarS1), false);

            //Test for get Parent* by arg2
            auto request_arg2 = std::make_shared<GetParentRequest>("7", Mode::FILTER_ARG_2, STAR);
            RelationshipResults parentStarChildS7 = getFacade.get(request_arg2);
            expected_size = 3;
            actual_size = static_cast<int>(parentStarChildS7.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "7", parentStarChildS7), true);
            Assert::AreEqual(doesStarRelationExists("4", "7", parentStarChildS7), true);
            Assert::AreEqual(doesStarRelationExists("6", "7", parentStarChildS7), true);
            Assert::AreEqual(doesStarRelationExists("1", "2", parentStarChildS7), false);

            //Test for get Parent* using arg1 and arg2
            auto request_arg1_arg2 = std::make_shared<GetParentRequest>("4", "6", Mode::FILTER_ARG1_ARG2, STAR);
            RelationshipResults parentStarS4S6 = getFacade.get(request_arg1_arg2);
            expected_size = 1;
            actual_size = static_cast<int>(parentStarS4S6.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("4", "6", parentStarS4S6), true);
            Assert::AreEqual(doesStarRelationExists("1", "2", parentStarS4S6), false);
        }

        TEST_METHOD(TestCalls)
        {
            Calls c1(p1, p2);
            Calls c2(p2, p3);
            Calls c3(p2, p4);
            Calls c4(p3, p4);
            Calls c5(p1, p5);
           
            vector<Rel::Calls> callsList{};
            callsList.push_back(c1);
            callsList.push_back(c2);
            callsList.push_back(c3);
            callsList.push_back(c4);
            callsList.push_back(c5);

            for (Calls a : callsList) {
                auto callsRequest = std::make_shared<InsertCallsRequest>(a);
                insertFacade.insert(callsRequest);
            }

            //Test for get all Calls
            auto request_all = std::make_shared<GetCallsRequest>(Mode::ALL);
            RelationshipResults allCalls = getFacade.get(request_all);
            int expected_size = 5;
            int actual_size = static_cast<int>(allCalls.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesCallsPairExists(c1, allCalls), true);
            Assert::AreEqual(doesCallsPairExists(c2, allCalls), true);
            Assert::AreEqual(doesCallsPairExists(c3, allCalls), true);

            //Test for get Calls by arg1
            auto request_arg1 = std::make_shared<GetCallsRequest>("p2", Mode::FILTER_ARG_1);
            RelationshipResults callsP2 = getFacade.get(request_arg1);
            expected_size = 2;
            actual_size = static_cast<int>(callsP2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesCallsPairExists(c2, callsP2), true);
            Assert::AreEqual(doesCallsPairExists(c3, callsP2), true);
            Assert::AreEqual(doesCallsPairExists(c4, callsP2), false);


            //Test for get Calls by arg2
            auto request_arg2 = std::make_shared<GetCallsRequest>("p4", Mode::FILTER_ARG_2);
            RelationshipResults calledP4 = getFacade.get(request_arg2);
            expected_size = 2;
            actual_size = static_cast<int>(calledP4.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesCallsPairExists(c3, calledP4), true);
            Assert::AreEqual(doesCallsPairExists(c4, calledP4), true);
            Assert::AreEqual(doesCallsPairExists(c1, calledP4), false);

            //Test for get Calls using arg1 and arg2
            auto request_arg1_arg2 = std::make_shared<GetCallsRequest>("main", "p5", Mode::FILTER_ARG1_ARG2);
            RelationshipResults calledP1P5 = getFacade.get(request_arg1_arg2);
            expected_size = 1;
            actual_size = static_cast<int>(calledP1P5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesCallsPairExists(c5, calledP1P5), true);
            Assert::AreEqual(doesCallsPairExists(c4, calledP1P5), false);
        }

        TEST_METHOD(TestCallsStar)
        {
            //Calls*
            //(main,p2), (main,p3), (main,p4), (main,p5)
            //(p2, p3), (p2, p4), (p2, p5)
            //(p3, p4), (p3, p5)

            //For star, reset and test again
            PKB* pkb = new PKB();
            InsertFacade insertFacade = InsertFacade(*pkb);
            GetFacade getFacade = GetFacade(*pkb);

            Calls c1(p1, p2);
            Calls c2(p2, p3);
            Calls c3(p2, p4);
            Calls c4(p3, p4);
            Calls c5(p3, p5);
            Calls c6(p1, p5);

            vector<Rel::Calls> callsList{};
            callsList.push_back(c1);
            callsList.push_back(c2);
            callsList.push_back(c3);
            callsList.push_back(c4);
            callsList.push_back(c5);
            callsList.push_back(c6);

            for (Calls a : callsList) {
                auto callsRequest = std::make_shared<InsertCallsRequest>(a);
                insertFacade.insert(callsRequest);
            }


            //after all insertion then generate star data
            insertFacade.generateSecondaryData();

            //Test for get all CallsStar
            auto request_all = std::make_shared<GetCallsRequest>(Mode::ALL, STAR);
            RelationshipResults allCallsStar = getFacade.get(request_all);
            int expected_size = 9;
            int actual_size = static_cast<int>(allCallsStar.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesStarRelationExists("main", "p2", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("main", "p4", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p2", "p5", allCallsStar), true);

            //Test for get CallsStar by arg1
            auto request_arg1 = std::make_shared<GetCallsRequest>("p2", Mode::FILTER_ARG_1, STAR);
            RelationshipResults callsStarP2 = getFacade.get(request_arg1);
            expected_size = 3;
            actual_size = static_cast<int>(callsStarP2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("p2", "p3", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p2", "p4", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p2", "p5", allCallsStar), true);

            //Test for get CallsStar by arg2
            auto request_arg2 = std::make_shared<GetCallsRequest>("p5", Mode::FILTER_ARG_2, STAR);
            RelationshipResults calledStarP5 = getFacade.get(request_arg2);
            expected_size = 3;
            actual_size = static_cast<int>(calledStarP5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("main", "p5", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p2", "p5", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p3", "p5", allCallsStar), true);

            //Test for get CallsStar using arg1 and arg2
            auto request_arg1_arg2 = std::make_shared<GetCallsRequest>("main", "p5", Mode::FILTER_ARG1_ARG2, STAR);
            RelationshipResults callsStarP1P5 = getFacade.get(request_arg1_arg2);
            expected_size = 1;
            actual_size = static_cast<int>(callsStarP1P5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("main", "p5", allCallsStar), true);
        }

        typedef std::string NextStmt;
        using stmtset_t = std::unordered_set<stmtNo_t>;
        using stmtPair_t = std::vector<std::pair<stmtNo_t, stmtNo_t>>;

        bool stmtPairEq(stmtPair_t a, stmtPair_t b) {
            std::unordered_set<std::string> aa;
            std::unordered_set<std::string> bb;
            for (auto& [first, second] : a) {
                aa.insert(std::to_string(first) + "_" + std::to_string(second));
            }
            for (auto& [first, second] : b) {
                bb.insert(std::to_string(first) + "_" + std::to_string(second));
            }
            return aa == bb;
        }

        std::vector<Cfg> makeCfg(std::string inProg) {
            auto tokens = TokenSplitter(inProg).tokenize();
            auto AST = Parser(tokens).parseTokens();
            return  CfgExtractor().extractCfg(AST);
        }

        std::vector<Cfg> sampleProgram() {
            auto inProgram =
                "procedure one {"
                "	read a;"			// 1
                "	if(a==a) then {"	// 2
                "	    read a;}"	    // 3
                "	else { "
                "       read a; }"	    // 4
                "	read a;"			// 5
                "}"
                "procedure two {"
                "	if(a==a)"			// 6
                "		then{read a;}"	// 7
                "		else{read a;}"	// 8
                "}"
                "procedure proc {"
                "	while(a==a)"	    // 9
                "		{read a;}"	    // 10
                "	read a;"		    // 11
                "}";
            return makeCfg(inProgram);
        }

        std::vector<Cfg> simpleProgram() {
            auto inProgram =
                "procedure proc {"
                "	while(a==a)"	    // 1
                "		{read a;}"	    // 2
                "	read a;"		    // 3
                "}";
            return makeCfg(inProgram);
        }

        //Results are:
        //Next(1, 2)
        //Next(2, 3), Next(2, 4)
        //Next(3, 5)
        //Next(4, 5)
        //Next(6, 7), Next(6, 8)
        //Next(9, 10), Next(9, 11)
        //Next(10, 9)

        TEST_METHOD(TestNext)
        {
            PKB* pkb = new PKB();
            InsertFacade insertFacade = InsertFacade(*pkb);
            GetFacade getFacade = GetFacade(*pkb);   
            std::vector<Cfg> cfgs = sampleProgram();

            for (const auto& cfg : cfgs) {
                insertFacade.insertCFG(cfg);
            }

            insertFacade.generateSecondaryData();

            //Test for get all Next
            auto request_all = std::make_shared<GetNextRequest>(Mode::ALL);
            RelationshipResults allNext = getFacade.get(request_all);
            int expected_size = 10;
            int actual_size = static_cast<int>(allNext.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesNextPairExists("2", "3", allNext), true);
            Assert::AreEqual(doesNextPairExists("9", "10", allNext), true);
            Assert::AreEqual(doesNextPairExists("6", "7", allNext), true);

            //Test for get Next by arg1
            auto request_arg1 = std::make_shared<GetNextRequest>("6", Mode::FILTER_ARG_1);
            RelationshipResults nextS6 = getFacade.get(request_arg1);
            expected_size = 2;
            actual_size = static_cast<int>(nextS6.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesNextPairExists("6", "7", nextS6), true);
            Assert::AreEqual(doesNextPairExists("6", "8", nextS6), true);
            Assert::AreEqual(doesNextPairExists("9", "10", nextS6), false);

            //Test for get Calls by arg2
            auto request_arg2 = std::make_shared<GetNextRequest>("11", Mode::FILTER_ARG_2);
            RelationshipResults nextByS11 = getFacade.get(request_arg2);
            expected_size = 1;
            actual_size = static_cast<int>(nextByS11.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesNextPairExists("9", "11", nextByS11), true);
            Assert::AreEqual(doesNextPairExists("10", "9", nextByS11), false);
            Assert::AreEqual(doesNextPairExists("6", "7", nextByS11), false);


            //Test for get Next using arg1 and arg2
            auto request_arg1_arg2 = std::make_shared<GetNextRequest>("2", "4", Mode::FILTER_ARG1_ARG2);
            RelationshipResults nextS3S4 = getFacade.get(request_arg1_arg2);
            expected_size = 1;
            actual_size = static_cast<int>(nextS3S4.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesNextPairExists("2", "4", nextS3S4), true);
            Assert::AreEqual(doesNextPairExists("3", "5", nextS3S4), false);
        }

        TEST_METHOD(TestNextStar)
        {
            //Results are:
            //Next*(1, 2), Next*(1,3), Next*(1,4), Next*(1, 5)
            //Next*(2, 3), Next*(2, 4), Next*(4, 5)
            //Next*(3, 5)
            //Next*(4, 5)

            //Next*(6, 7), Next*(6, 8)

            //Next*(9, 9), Next*(9, 10),Next*(9, 11)
            //Next*(10, 9), Next*(10, 10), Next*(10, 11)

            PKB* pkb = new PKB();
            InsertFacade insertFacade = InsertFacade(*pkb);
            GetFacade getFacade = GetFacade(*pkb);
            std::vector<Cfg> cfgs = sampleProgram();

            for (const auto& cfg : cfgs) {
                insertFacade.insertCFG(cfg);
            }

            insertFacade.generateSecondaryData();

            //Test for get all NextStar
            auto request_all = std::make_shared<GetNextRequest>(Mode::ALL, STAR);

            RelationshipResults allNextStar = getFacade.get(request_all);
            int expected_size = 17;
            int actual_size = static_cast<int>(allNextStar.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesStarRelationExists("1", "2", allNextStar), true);
            Assert::AreEqual(doesStarRelationExists("1", "5", allNextStar), true);
            Assert::AreEqual(doesStarRelationExists("2", "5", allNextStar), true);
            Assert::AreEqual(doesStarRelationExists("6", "7", allNextStar), true);
            Assert::AreEqual(doesStarRelationExists("10", "9", allNextStar), true);
            Assert::AreEqual(doesStarRelationExists("10", "11", allNextStar), true);
            Assert::AreEqual(doesStarRelationExists("10", "10", allNextStar), true);
            Assert::AreEqual(doesStarRelationExists("9", "9", allNextStar), true);


            //Test for get NextStar by arg1
            auto request_arg1 = std::make_shared<GetNextRequest>("10", Mode::FILTER_ARG_1, STAR);
            RelationshipResults nextStarS10 = getFacade.get(request_arg1);
            expected_size = 3;
            actual_size = static_cast<int>(nextStarS10.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("10", "9", nextStarS10), true);
            Assert::AreEqual(doesStarRelationExists("10", "10", nextStarS10), true);
            Assert::AreEqual(doesStarRelationExists("10", "11", nextStarS10), true);
            Assert::AreEqual(doesStarRelationExists("2", "4", nextStarS10), false);

            //Test for get NextStar by arg2
            auto request_arg2 = std::make_shared<GetNextRequest>("5", Mode::FILTER_ARG_2, STAR);
            RelationshipResults nextStarS5 = getFacade.get(request_arg2);
            expected_size = 4;
            actual_size = static_cast<int>(nextStarS5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "5", nextStarS5), true);
            Assert::AreEqual(doesStarRelationExists("2", "5", nextStarS5), true);
            Assert::AreEqual(doesStarRelationExists("3", "5", nextStarS5), true);
            Assert::AreEqual(doesStarRelationExists("4", "5", nextStarS5), true);
            Assert::AreEqual(doesStarRelationExists("6", "5", nextStarS5), false);


            //Test for get NextStar using arg1 and arg2
            auto request_arg1_arg2 = std::make_shared<GetNextRequest>("1", "5", Mode::FILTER_ARG1_ARG2, STAR);
            RelationshipResults nextStarS1S5 = getFacade.get(request_arg1_arg2);
            expected_size = 1;
            actual_size = static_cast<int>(nextStarS1S5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "5", nextStarS1S5), true);
            Assert::AreEqual(doesStarRelationExists("2", "5", nextStarS1S5), false);
        }

        bool doesFollowsPairExists(Follows follows, RelationshipResults results) {
            auto castedArg1 = dynamic_pointer_cast<Statement>(follows.getArg1());
            auto castedArg2 = dynamic_pointer_cast<Statement>(follows.getArg2());
            std::pair<std::string, std::string> item(std::to_string(castedArg1->getStmtNo()), std::to_string(castedArg2->getStmtNo()));

            if (find(results.begin(), results.end(), item) != results.end()) {
                return true;
            }
            return false;
        }

        bool doesParentPairExists(Parent parent, RelationshipResults results) {
            auto castedArg1 = dynamic_pointer_cast<Statement>(parent.getArg1());
            auto castedArg2 = dynamic_pointer_cast<Statement>(parent.getArg2());
            std::pair<std::string, std::string> item(std::to_string(castedArg1->getStmtNo()), std::to_string(castedArg2->getStmtNo()));

            if (find(results.begin(), results.end(), item) != results.end()) {
                return true;
            }
            return false;
        }

        bool doesModifiesPairExists(Modifies modifies, RelationshipResults results, std::string arg1Type) {
            std::pair<std::string, std::string> item;
            if (arg1Type == "statement") {
                auto castedArg1 = dynamic_pointer_cast<Statement>(modifies.getArg1());
                item = std::make_pair(std::to_string(castedArg1->getStmtNo()), modifies.getArg2()->getName());
            } else if (arg1Type == "procedure") {
                item = std::make_pair(modifies.getArg1()->getName(), modifies.getArg2()->getName());
            }
            else {
                return false;
            }

            if (find(results.begin(), results.end(), item) != results.end()) {
                return true;
            }
            return false;
        }

        bool doesUsesPairExists(Uses uses, RelationshipResults results, std::string arg1Type) {
            std::pair<std::string, std::string> item;
            if (arg1Type == "statement") {
                auto castedArg1 = dynamic_pointer_cast<Statement>(uses.getArg1());
                item = std::make_pair(std::to_string(castedArg1->getStmtNo()), uses.getArg2()->getName());
            }
            else if (arg1Type == "procedure") {
                item = std::make_pair(uses.getArg1()->getName(), uses.getArg2()->getName());
            }
            else {
                return false;
            }

            if (find(results.begin(), results.end(), item) != results.end()) {
                return true;
            }
            return false;
        }

        bool doesStarRelationExists(std::string arg1, std::string arg2, RelationshipResults results) {
            std::pair<std::string, std::string> item = std::make_pair(arg1, arg2);

            if (find(results.begin(), results.end(), item) != results.end()) {
                return true;
            }
            return false;
        }

        bool doesCallsPairExists(Calls calls, RelationshipResults results) {
            std::pair<std::string, std::string> item(calls.getArg1()->getName(), calls.getArg2()->getName());

            if (results.find(item) != results.end()) {
                return true;
            }
            return false;
        }

        bool doesNextPairExists(NextStmt s1, NextStmt s2, RelationshipResults results) {
            std::pair<std::string, std::string> item = std::make_pair(s1, s2);

            if (results.find(item) != results.end()) {
                return true;
            }
            return false;
        }
    };
}