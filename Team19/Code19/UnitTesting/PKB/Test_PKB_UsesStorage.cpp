#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB/Storage/RelationshipStorages/UsesStorage.h"
#include "../source/Relationship/Relationship.h"
#include "../source/Entity/Entity.h"
#include "../source/TypeAlias/TypeAlias.h"

#include <cstddef>
#include <iostream>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(Test_PKB_UsesStorage)
    {
    public:
        TEST_METHOD(TestUses)
        {
            UsesStorage storageStub = UsesStorage();

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

            Uses u1(p1, v1);    //(main, x)
            Uses u2(p1, v2);    //(main, x)
            Uses u3(s4, v2);    //(4, x)
            Uses u4(s4, v3);    //(4, y)
            Uses u5(s5, v4);    //(5, x)

            storageStub.insertRelationship(u1);
            storageStub.insertRelationship(u2);
            storageStub.insertRelationship(u3);
            storageStub.insertRelationship(u4);
            storageStub.insertRelationship(u5);

            //Test for get all Uses
            RelationshipResults allUses = storageStub.getAllRelationship();
            int expected_size = 4; //This is 4 instead of 5, because main uses "x" twice but will be captured only once.
            int actual_size = static_cast<int>(allUses.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesUsesPairExists(u1, allUses, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u2, allUses, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u3, allUses, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u4, allUses, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u5, allUses, "statement"), true);

            //Test for get Uses by arg1, arg1Type = "procedure"
            RelationshipResults usesP1 = storageStub.getRelationshipByArg1("main", Arg1Type::PROCEDURE);
            expected_size = 1;
            actual_size = static_cast<int>(usesP1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u1, usesP1, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u2, usesP1, "procedure"), true);

            //Test for get Uses by arg1, arg1Type = "statement"
            RelationshipResults usesS4 = storageStub.getRelationshipByArg1("4", Arg1Type::STATEMENT);
            expected_size = 2;
            actual_size = static_cast<int>(usesS4.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u3, usesS4, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u4, usesS4, "statement"), true);

            //Test for get Uses by arg2, arg1Type = "procedure"
            //Which procedures uses the arg2? Uses(p, arg2)
            RelationshipResults procUsesX = storageStub.getRelationshipByArg2("x", Arg1Type::PROCEDURE);
            expected_size = 1;
            actual_size = static_cast<int>(procUsesX.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u1, procUsesX, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u1, procUsesX, "procedure"), true);

            //Test for get Uses by arg2, arg1Type = "procedure"
            //Which statements uses the arg2? Uses(s, arg2)
            RelationshipResults stmtUsesX = storageStub.getRelationshipByArg2("x", Arg1Type::STATEMENT);
            expected_size = 2;
            actual_size = static_cast<int>(stmtUsesX.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u3, stmtUsesX, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u5, stmtUsesX, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u4, stmtUsesX, "statement"), false);

            //Test for get Uses using arg1 and arg2, arg1Type = "procedure"
            //Uses(main, x)
            RelationshipResults usesP1V1 = storageStub.getRelationshipByArg1AndArg2("main", "x", Arg1Type::PROCEDURE);
            expected_size = 1;
            actual_size = static_cast<int>(usesP1V1.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u1, usesP1V1, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u2, usesP1V1, "procedure"), true);
            Assert::AreEqual(doesUsesPairExists(u3, usesP1V1, "procedure"), false);

            //Test for get Uses using arg1 and arg2, arg1Type = "statement"
            //Uses(4, y)
            RelationshipResults usesS4V3 = storageStub.getRelationshipByArg1AndArg2("4", "y", Arg1Type::STATEMENT);
            expected_size = 1;
            actual_size = static_cast<int>(usesS4V3.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesUsesPairExists(u4, usesS4V3, "statement"), true);
            Assert::AreEqual(doesUsesPairExists(u5, usesS4V3, "statement"), false);
        }

        bool doesUsesPairExists(Uses uses, RelationshipResults results, std::string arg1Type) {
            std::pair<std::string, std::string> item;
            if (arg1Type == "statement") {
                auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(uses.getArg1());

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
    };
}