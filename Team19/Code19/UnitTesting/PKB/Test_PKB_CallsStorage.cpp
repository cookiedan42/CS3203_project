#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB/Storage/RelationshipStorages/CallsStorage.h"
#include "../source/Relationship/Relationship.h"
#include "../source/Entity/Entity.h"
#include "../source/TypeAlias/TypeAlias.h"

#include <cstddef>
#include <iostream>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(Test_PKB_CallsStorage)
    {
    public:
        TEST_METHOD(TestCalls)
        {
            CallsStorage storageStub = CallsStorage();

            Procedure p1("main");
            Procedure p2("p2");
            Procedure p3("p3");
            Procedure p4("p4");
            Procedure p5("p5");

            Calls c1(p1, p2);
            Calls c2(p2, p3);
            Calls c3(p2, p4);
            Calls c4(p3, p4);
            Calls c5(p1, p5);

            storageStub.insertRelationship(c1);
            storageStub.insertRelationship(c2);
            storageStub.insertRelationship(c3);
            storageStub.insertRelationship(c4);
            storageStub.insertRelationship(c5);

            //Test for get all Calls
            RelationshipResults allCalls = storageStub.getAllRelationship();
            int expected_size = 5;
            int actual_size = static_cast<int>(allCalls.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesCallsPairExists(c1, allCalls), true);
            Assert::AreEqual(doesCallsPairExists(c2, allCalls), true);
            Assert::AreEqual(doesCallsPairExists(c3, allCalls), true);

            //Test for get Calls by arg1
            RelationshipResults callsP2 = storageStub.getRelationshipByArg1("p2");
            expected_size = 2;
            actual_size = static_cast<int>(callsP2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesCallsPairExists(c2, callsP2), true);
            Assert::AreEqual(doesCallsPairExists(c3, callsP2), true);
            Assert::AreEqual(doesCallsPairExists(c4, callsP2), false);


            //Test for get Calls by arg2
            RelationshipResults calledP4 = storageStub.getRelationshipByArg2("p4");
            expected_size = 2;
            actual_size = static_cast<int>(calledP4.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesCallsPairExists(c3, calledP4), true);
            Assert::AreEqual(doesCallsPairExists(c4, calledP4), true);
            Assert::AreEqual(doesCallsPairExists(c1, calledP4), false);

            //Test for get Calls using arg1 and arg2
            RelationshipResults calledP1P5 = storageStub.getRelationshipByArg1AndArg2("main", "p5");
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
            CallsStorage storageStub = CallsStorage();

            Procedure p1("main");
            Procedure p2("p2");
            Procedure p3("p3");
            Procedure p4("p4");
            Procedure p5("p5");

            Calls c1(p1, p2);
            Calls c2(p2, p3);
            Calls c3(p2, p4);
            Calls c4(p3, p4);
            Calls c5(p3, p5);
            Calls c6(p1, p5);

            storageStub.insertRelationship(c1);
            storageStub.insertRelationship(c2);
            storageStub.insertRelationship(c3);
            storageStub.insertRelationship(c4);
            storageStub.insertRelationship(c5);
            storageStub.insertRelationship(c6);

            //after all insertion then generate star data
            storageStub.generateStarData();

            //Test for get all CallsStar
            RelationshipResults allCallsStar = storageStub.getAllRelationship(true);
            int expected_size = 9;
            int actual_size = static_cast<int>(allCallsStar.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesStarRelationExists("main", "p2", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("main", "p4", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p2", "p5", allCallsStar), true);

            //Test for get CallsStar by arg1
            RelationshipResults callsStarP2 = storageStub.getRelationshipByArg1("p2", true);
            expected_size = 3;
            actual_size = static_cast<int>(callsStarP2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("p2", "p3", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p2", "p4", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p2", "p5", allCallsStar), true);

            //Test for get CallsStar by arg2
            RelationshipResults calledStarP5 = storageStub.getRelationshipByArg2("p5", true);
            expected_size = 3;
            actual_size = static_cast<int>(calledStarP5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("main", "p5", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p2", "p5", allCallsStar), true);
            Assert::AreEqual(doesStarRelationExists("p3", "p5", allCallsStar), true);

            //Test for get CallsStar using arg1 and arg2
            RelationshipResults callsStarP1P5 = storageStub.getRelationshipByArg1AndArg2("main", "p5", true);
            expected_size = 1;
            actual_size = static_cast<int>(callsStarP1P5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("main", "p5", allCallsStar), true);
        }

        bool doesCallsPairExists(Calls calls, RelationshipResults results) {
            std::pair<std::string, std::string> item(calls.getArg1()->getName(), calls.getArg2()->getName());

            if (results.find(item) != results.end()) {
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
    };
}