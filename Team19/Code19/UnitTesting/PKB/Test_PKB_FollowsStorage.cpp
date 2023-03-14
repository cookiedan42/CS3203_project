#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB/Storage/RelationshipStorages/FollowsStorage.h"
#include "../source/Relationship/Relationship.h"
#include "../source/Entity/Entity.h"
#include "../source/TypeAlias/TypeAlias.h"

#include <cstddef>
#include <iostream>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(Test_PKB_FollowsStorage)
    {
    public:
        TEST_METHOD(TestFollows)
        {
            FollowsStorage storageStub = FollowsStorage();

            Statement s1(1, "print");
            Statement s2(2, "while");
            Statement s3(3, "if");
            Statement s4(4, "assign");
            Statement s5(5, "assign");

            Follows f1(s1, s2);
            Follows f2(s2, s3);
            Follows f3(s3, s5);
            Follows f4(s4, s5);

            storageStub.insertRelationship(f1);
            storageStub.insertRelationship(f2);
            storageStub.insertRelationship(f3);
            storageStub.insertRelationship(f4);

            //Test for get all Follows
            RelationshipResults allFollows = storageStub.getAllRelationship();
            int expected_size = 4;
            int actual_size = static_cast<int>(allFollows.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesFollowsPairExists(f1, allFollows), true);
            Assert::AreEqual(doesFollowsPairExists(f2, allFollows), true);
            Assert::AreEqual(doesFollowsPairExists(f3, allFollows), true);

            //Test for get Follows by arg1
            RelationshipResults followsS1 = storageStub.getRelationshipByArg1("1");
            expected_size = 1;
            actual_size = static_cast<int>(followsS1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesFollowsPairExists(f1, followsS1), true);
            Assert::AreEqual(doesFollowsPairExists(f2, followsS1), false);

            //Test for get Followed by arg2
            RelationshipResults followedByS5 = storageStub.getRelationshipByArg2("5");
            expected_size = 2;
            actual_size = static_cast<int>(followedByS5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesFollowsPairExists(f1, followedByS5), false);
            Assert::AreEqual(doesFollowsPairExists(f3, followedByS5), true);
            Assert::AreEqual(doesFollowsPairExists(f4, followedByS5), true);

            //Test for get Follows using arg1 and arg2
            RelationshipResults followsS1S2 = storageStub.getRelationshipByArg1AndArg2("1", "2");
            expected_size = 1;
            actual_size = static_cast<int>(followsS1S2.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesFollowsPairExists(f1, followsS1S2), true);
            Assert::AreEqual(doesFollowsPairExists(f3, followsS1S2), false);
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
            FollowsStorage storageStub = FollowsStorage();

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

            storageStub.insertRelationship(f1);
            storageStub.insertRelationship(f2);
            storageStub.insertRelationship(f3);
            storageStub.insertRelationship(f4);

            //after all insertion then generate star data
            storageStub.generateStarData();

            //Test for get all Follows Star
            RelationshipResults allFollowsStar = storageStub.getAllRelationship(true);
            int expected_size = 7;
            int actual_size = static_cast<int>(allFollowsStar.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesStarRelationExists("1", "2", allFollowsStar), true);
            Assert::AreEqual(doesStarRelationExists("1", "6", allFollowsStar), true);
            Assert::AreEqual(doesStarRelationExists("2", "6", allFollowsStar), true);
            Assert::AreEqual(doesStarRelationExists("1", "5", allFollowsStar), false);


            //Test for get Follows* by arg1
            RelationshipResults followsStarS1 = storageStub.getRelationshipByArg1("1", true);
            expected_size = 3;
            actual_size = static_cast<int>(followsStarS1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "2", followsStarS1), true);
            Assert::AreEqual(doesStarRelationExists("1", "3", followsStarS1), true);
            Assert::AreEqual(doesStarRelationExists("1", "6", followsStarS1), true);
            Assert::AreEqual(doesStarRelationExists("2", "6", followsStarS1), false);

            //Test for get Follows* by arg2
            RelationshipResults followedStarByS6 = storageStub.getRelationshipByArg2("6", true);
            expected_size = 3;
            actual_size = static_cast<int>(followedStarByS6.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "6", followedStarByS6), true);
            Assert::AreEqual(doesStarRelationExists("2", "6", followedStarByS6), true);
            Assert::AreEqual(doesStarRelationExists("3", "6", followedStarByS6), true);
            Assert::AreEqual(doesStarRelationExists("2", "3", followedStarByS6), false);


            //Test for get Follows* using arg1 and arg2
            RelationshipResults followsStarS2S6 = storageStub.getRelationshipByArg1AndArg2("2", "6", true);
            expected_size = 1;
            actual_size = static_cast<int>(followsStarS2S6.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("2", "6", followsStarS2S6), true);
            Assert::AreEqual(doesStarRelationExists("2", "3", followsStarS2S6), false);
        }

        bool doesFollowsPairExists(Follows follows, RelationshipResults results) {
            auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(follows.getArg1());
            auto castedArg2 = dynamic_pointer_cast<Ent::Statement>(follows.getArg2());

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