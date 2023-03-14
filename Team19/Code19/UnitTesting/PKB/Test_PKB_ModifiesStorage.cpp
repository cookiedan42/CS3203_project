#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB/Storage/RelationshipStorages/ModifiesStorage.h"
#include "../source/Relationship/Relationship.h"
#include "../source/Entity/Entity.h"
#include "../source/TypeAlias/TypeAlias.h"

#include <cstddef>
#include <iostream>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(Test_PKB_ModifiesStorage)
    {
    public:
        TEST_METHOD(TestModifies)
        {
            ModifiesStorage storageStub = ModifiesStorage();

            Statement s1(1, "print");
            Statement s2(2, "while");
            Statement s3(3, "if");
            Statement s4(4, "assign");
            Statement s5(5, "assign");

            Variable v1("x");
            Variable v2("x");
            Variable v3("y");
            Variable v4("x");

            Procedure p1("main");
            Procedure p2("p2");
            Procedure p3("p3");
            Procedure p4("p4");
            Procedure p5("p5");

            Modifies m1(p1, v1);    //(main, x)
            Modifies m2(p1, v2);    //(main, x)
            Modifies m3(s4, v2);    //(4, x)
            Modifies m4(s4, v3);    //(4, y)
            Modifies m5(s5, v4);    //(5, x)

            storageStub.insertRelationship(m1);
            storageStub.insertRelationship(m2);
            storageStub.insertRelationship(m3);
            storageStub.insertRelationship(m4);
            storageStub.insertRelationship(m5);

            //Test for get all Modifies
            RelationshipResults allModifies = storageStub.getAllRelationship();
            int expected_size = 4; //This is 4 instead of 5, because main modifies "x" twice but will be captured only once.
            int actual_size = static_cast<int>(allModifies.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesModifiesPairExists(m1, allModifies, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m2, allModifies, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m3, allModifies, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m4, allModifies, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m5, allModifies, "statement"), true);

            //Test for get Modifies by arg1, arg1Type = "procedure"
            RelationshipResults modifiesP1 = storageStub.getRelationshipByArg1("main", Arg1Type::PROCEDURE);
            expected_size = 1;
            actual_size = static_cast<int>(modifiesP1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m1, modifiesP1, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m2, modifiesP1, "procedure"), true);

            //Test for get Modifies by arg1, arg1Type = "statement"
            RelationshipResults modifiesS4 = storageStub.getRelationshipByArg1("4", Arg1Type::STATEMENT);
            expected_size = 2;
            actual_size = static_cast<int>(modifiesS4.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m3, modifiesS4, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m4, modifiesS4, "statement"), true);

            //Test for get Modifies by arg2, arg1Type = "procedure"
            //Which procedures modified the arg2? Modifies(p, arg2)
            RelationshipResults procModifiedX = storageStub.getRelationshipByArg2("x", Arg1Type::PROCEDURE);
            expected_size = 1;
            actual_size = static_cast<int>(procModifiedX.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m1, procModifiedX, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m1, procModifiedX, "procedure"), true);

            //Test for get Modifies by arg2, arg1Type = "procedure"
            //Which statements modified the arg2? Modifies(s, arg2)
            RelationshipResults stmtModifiedX = storageStub.getRelationshipByArg2("x", Arg1Type::STATEMENT);
            expected_size = 2;
            actual_size = static_cast<int>(stmtModifiedX.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m3, stmtModifiedX, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m5, stmtModifiedX, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m4, stmtModifiedX, "statement"), false);

            //Test for get Modifies using arg1 and arg2, arg1Type = "procedure"
            //Modifies(main, x)
            RelationshipResults modifiesP1V1 = storageStub.getRelationshipByArg1AndArg2("main", "x", Arg1Type::PROCEDURE);
            expected_size = 1;
            actual_size = static_cast<int>(modifiesP1V1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m1, modifiesP1V1, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m2, modifiesP1V1, "procedure"), true);
            Assert::AreEqual(doesModifiesPairExists(m3, modifiesP1V1, "procedure"), false);

            //Test for get Modifies using arg1 and arg2, arg1Type = "statement"
            //Modifies(4, y)
            RelationshipResults modifiesS4V3 = storageStub.getRelationshipByArg1AndArg2("4", "y", Arg1Type::STATEMENT);
            expected_size = 1;
            actual_size = static_cast<int>(modifiesS4V3.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesModifiesPairExists(m4, modifiesS4V3, "statement"), true);
            Assert::AreEqual(doesModifiesPairExists(m5, modifiesS4V3, "statement"), false);
        }

        bool doesModifiesPairExists(Modifies modifies, RelationshipResults results, std::string arg1Type) {
            std::pair<std::string, std::string> item;
            if (arg1Type == "statement") {
                auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(modifies.getArg1());

                item = std::make_pair(std::to_string(castedArg1->getStmtNo()), modifies.getArg2()->getName());
            }
            else if (arg1Type == "procedure") {
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

        bool doesStarRelationExists(std::string arg1, std::string arg2, RelationshipResults results) {
            std::pair<std::string, std::string> item = std::make_pair(arg1, arg2);

            if (find(results.begin(), results.end(), item) != results.end()) {
                return true;
            }
            return false;
        }
    };
}