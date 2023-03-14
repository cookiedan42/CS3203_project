#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB/Storage/RelationshipStorages/NextStorage.h"
#include "../source/PKB/Storage/RelationshipStorage.h"
#include "../source/Relationship/Relationship.h"
#include "../source/Entity/Entity.h"
#include "../source/TypeAlias/TypeAlias.h"
#include "../source/Cfg/Cfg.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/CfgExtractor.h"

#include <cstddef>
#include <iostream>
#include <algorithm>
#include <unordered_set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(Test_PKB_NextStorage)
    {
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

    public:
        TEST_METHOD(TestNext)
        {
            NextStorage storageStub = NextStorage();
            CFGStorage cfgStorage = CFGStorage();
            
            std::vector<Cfg> cfgs = sampleProgram();

            for (const auto& cfg : cfgs) {
                cfgStorage.insertCFG(cfg);
            }

            storageStub.generateNextData(cfgStorage);

            //Test for get all Next
            RelationshipResults allNext = storageStub.getAllRelationship(cfgStorage);
            int expected_size = 10;
            int actual_size = static_cast<int>(allNext.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesNextPairExists("2", "3", allNext), true);
            Assert::AreEqual(doesNextPairExists("9", "10", allNext), true);
            Assert::AreEqual(doesNextPairExists("6", "7", allNext), true);

            //Test for get Next by arg1
            RelationshipResults nextS6= storageStub.getRelationshipByArg1(cfgStorage, "6");
            expected_size = 2;
            actual_size = static_cast<int>(nextS6.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesNextPairExists("6", "7", nextS6), true);
            Assert::AreEqual(doesNextPairExists("6", "8", nextS6), true);
            Assert::AreEqual(doesNextPairExists("9", "10", nextS6), false);

            //Test for get Calls by arg2
            RelationshipResults nextByS11 = storageStub.getRelationshipByArg2(cfgStorage, "11");
            expected_size = 1;
            actual_size = static_cast<int>(nextByS11.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesNextPairExists("9", "11", nextByS11), true);
            Assert::AreEqual(doesNextPairExists("10", "9", nextByS11), false);
            Assert::AreEqual(doesNextPairExists("6", "7", nextByS11), false);


            //Test for get Next using arg1 and arg2
            RelationshipResults nextS3S4 = storageStub.getRelationshipByArg1AndArg2(cfgStorage, "2", "4");
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

            NextStorage storageStub = NextStorage();
            CFGStorage cfgStorage = CFGStorage();

            std::vector<Cfg> cfgs = sampleProgram();

            for (const auto& cfg : cfgs) {
                cfgStorage.insertCFG(cfg);
            }

            storageStub.generateNextData(cfgStorage);

            //Test for get all CallsStar
            RelationshipResults allNextStar = storageStub.getAllRelationship(cfgStorage, true);
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
            RelationshipResults nextStarS10 = storageStub.getRelationshipByArg1(cfgStorage, "10", true);
            expected_size = 3;
            actual_size = static_cast<int>(nextStarS10.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("10", "9", nextStarS10), true);
            Assert::AreEqual(doesStarRelationExists("10", "10", nextStarS10), true);
            Assert::AreEqual(doesStarRelationExists("10", "11", nextStarS10), true);
            Assert::AreEqual(doesStarRelationExists("2", "4", nextStarS10), false);

            //Test for get NextStar by arg2
            RelationshipResults nextStarS5 = storageStub.getRelationshipByArg2(cfgStorage, "5", true);
            expected_size = 4;
            actual_size = static_cast<int>(nextStarS5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "5", nextStarS5), true);
            Assert::AreEqual(doesStarRelationExists("2", "5", nextStarS5), true);
            Assert::AreEqual(doesStarRelationExists("3", "5", nextStarS5), true);
            Assert::AreEqual(doesStarRelationExists("4", "5", nextStarS5), true);
            Assert::AreEqual(doesStarRelationExists("6", "5", nextStarS5), false);


            //Test for get NextStar using arg1 and arg2
            RelationshipResults nextStarS1S5 = storageStub.getRelationshipByArg1AndArg2(cfgStorage, "1", "5", true);
            expected_size = 1;
            actual_size = static_cast<int>(nextStarS1S5.size());
            Assert::AreEqual(expected_size, actual_size);
            Assert::AreEqual(doesStarRelationExists("1", "5", nextStarS1S5), true);
            Assert::AreEqual(doesStarRelationExists("2", "5", nextStarS1S5), false);
        }

        bool doesNextPairExists(NextStmt s1, NextStmt s2, RelationshipResults results) {
            std::pair<std::string, std::string> item = std::make_pair(s1, s2);

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