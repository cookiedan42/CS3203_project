#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB/Storage/RelationshipStorages/ParentStorage.h"
#include "../source/Relationship/Relationship.h"
#include "../source/Entity/Entity.h"
#include "../source/TypeAlias/TypeAlias.h"

#include <cstddef>
#include <iostream>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(Test_PKB_ParentStorage)
    {
    public:
        TEST_METHOD(TestParent)
        {
            ParentStorage storageStub = ParentStorage();

            Statement s1(1, "print");
            Statement s2(2, "while");
            Statement s3(3, "if");
            Statement s4(4, "assign");
            Statement s5(5, "assign");

            Parent p1(s1, s2);
            Parent p2(s1, s3);
            Parent p3(s3, s2);
            Parent p4(s3, s5);

            storageStub.insertRelationship(p1);
            storageStub.insertRelationship(p2);
            storageStub.insertRelationship(p3);
            storageStub.insertRelationship(p4);


            //Test for get all Parent
            RelationshipResults allParent = storageStub.getAllRelationship();
            int expected_size = 4;
            int actual_size = static_cast<int>(allParent.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesParentPairExists(p1, allParent), true);
            Assert::AreEqual(doesParentPairExists(p2, allParent), true);
            Assert::AreEqual(doesParentPairExists(p3, allParent), true);
            Assert::AreEqual(doesParentPairExists(p4, allParent), true);


            //Test for get Parent by arg1
            RelationshipResults parentS1 = storageStub.getRelationshipByArg1("1");
            expected_size = 2;
            actual_size = static_cast<int>(parentS1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesParentPairExists(p1, parentS1), true);
            Assert::AreEqual(doesParentPairExists(p2, parentS1), true);

            //Test for get Parent by arg2
            RelationshipResults childS2 = storageStub.getRelationshipByArg2("2");
            expected_size = 2;
            actual_size = static_cast<int>(childS2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesParentPairExists(p1, childS2), true);
            Assert::AreEqual(doesParentPairExists(p3, childS2), true);
            Assert::AreEqual(doesParentPairExists(p2, childS2), false);

            //Test for get Parent using arg1 and arg2
            RelationshipResults parentS1S2 = storageStub.getRelationshipByArg1AndArg2("1", "2");
            expected_size = 1;
            actual_size = static_cast<int>(parentS1S2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesParentPairExists(p1, parentS1S2), true);
            Assert::AreEqual(doesParentPairExists(p2, parentS1S2), false);
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
            ParentStorage storageStub = ParentStorage();

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

            storageStub.insertRelationship(p1);
            storageStub.insertRelationship(p2);
            storageStub.insertRelationship(p3);
            storageStub.insertRelationship(p4);
            storageStub.insertRelationship(p5);
            storageStub.insertRelationship(p6);
            storageStub.insertRelationship(p7);
            storageStub.insertRelationship(p8);

            //after all insertion then generate star data
            storageStub.generateStarData();

            //Test for get all Parent Star
            RelationshipResults allParentStar = storageStub.getAllRelationship(true);
            int expected_size = 13;
            int actual_size = static_cast<int>(allParentStar.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesStarRelationExists("1", "2", allParentStar), true);
            Assert::AreEqual(doesStarRelationExists("1", "7", allParentStar), true);
            Assert::AreEqual(doesStarRelationExists("4", "7", allParentStar), true);
            Assert::AreEqual(doesStarRelationExists("4", "9", allParentStar), false);


            //Test for get Parent* by arg1
            RelationshipResults parentStarS1 = storageStub.getRelationshipByArg1("1", true);
            expected_size = 8;
            actual_size = static_cast<int>(parentStarS1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "2", parentStarS1), true);
            Assert::AreEqual(doesStarRelationExists("1", "7", parentStarS1), true);
            Assert::AreEqual(doesStarRelationExists("4", "7", parentStarS1), false);

            //Test for get Parent* by arg2
            RelationshipResults parentStarChildS7 = storageStub.getRelationshipByArg2("7", true);
            expected_size = 3;
            actual_size = static_cast<int>(parentStarChildS7.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "7", parentStarChildS7), true);
            Assert::AreEqual(doesStarRelationExists("4", "7", parentStarChildS7), true);
            Assert::AreEqual(doesStarRelationExists("6", "7", parentStarChildS7), true);
            Assert::AreEqual(doesStarRelationExists("1", "2", parentStarChildS7), false);

            //Test for get Parent* using arg1 and arg2
            RelationshipResults parentStarS4S6 = storageStub.getRelationshipByArg1AndArg2("4", "6", true);
            expected_size = 1;
            actual_size = static_cast<int>(parentStarS4S6.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("4", "6", parentStarS4S6), true);
            Assert::AreEqual(doesStarRelationExists("1", "2", parentStarS4S6), false);
        }

        bool doesParentPairExists(Parent parent, RelationshipResults results) {
            auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(parent.getArg1());
            auto castedArg2 = dynamic_pointer_cast<Ent::Statement>(parent.getArg2());

            std::pair<std::string, std::string> item(std::to_string(castedArg1->getStmtNo()), std::to_string(castedArg2->getStmtNo()));

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
    };
}