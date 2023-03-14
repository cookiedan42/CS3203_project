#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <unordered_set>
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/CfgExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;



using output_t = std::unordered_map < procName_t, Cfg>;
struct CfgTestHelper {
	output_t outputConverter(std::vector<Cfg> inp) {
		// must use .at because Cfg has no default constructor
		std::unordered_map < procName_t, Cfg> output = {};
		for each (auto entry in inp) {
			output.insert({entry.getProcName(),entry});
		}
		return output;
	}

	std::vector<Cfg> makeCfgTyped(std::string inProgram) {
		auto tokens = TokenSplitter(inProgram).tokenize();
		auto AST = Parser(tokens).parseTokens();
		auto output = CfgExtractor().extractCfg(AST);
		return output;
	}

	output_t makeCfg(std::string inProgram) {
		auto tokens = TokenSplitter(inProgram).tokenize();
		auto AST = Parser(tokens).parseTokens();
		auto output = CfgExtractor().extractCfg(AST);
		return outputConverter(output);
	}
};
auto c = CfgTestHelper();

template<typename T>
void assertSize(size_t size, T to_test) {
	Assert::AreEqual(size, to_test.size());
}


namespace UnitTesting {
	TEST_CLASS(TestDE_Cfg) {
public:

	TEST_METHOD(Test_emptyCfg_callNotNext) {
		auto prog =
			"procedure proc1 {call proc2;}"
			"procedure proc2 {1=1;}";
		auto actual = c.makeCfg(prog);


		auto cfg1 = Cfg("proc1", 1); cfg1.insert(1, 0);
		auto cfg2 = Cfg("proc2", 2); cfg2.insert(2, 0);
		auto expected = c.outputConverter({ cfg1,cfg2, });
		
		Assert::IsTrue(actual == expected);

		auto actualT = c.makeCfgTyped(prog);
		Assert::IsTrue(actualT[0].getStmtType(1) == Ent::call_stmtType);
		Assert::IsTrue(actualT[0].getStmtType(2) == "");
		Assert::IsTrue(actualT[1].getStmtType(2) == Ent::assign_stmtType);
	}

	TEST_METHOD(Test_oneProc_Simple) {
		output_t expected = c.outputConverter({ Cfg("proc",1) });
		expected.at("proc").insert(1, 2);
		expected.at("proc").insert(2,0);
		
		std::vector<std::string> progs{
			"procedure proc {read a;read a;}",
			"procedure proc {read a;print a;}",
			"procedure proc {read a;call a;}",
			"procedure proc {read a;a=a;}",

			"procedure proc {print a;read a;}",
			"procedure proc {print a;print a;}",
			"procedure proc {print a;call a;}",
			"procedure proc {print a;a=a;}",

			"procedure proc {call a;read a;}",
			"procedure proc {call a;print a;}",
			"procedure proc {call a;call a;}",
			"procedure proc {call a;a=a;}",

			"procedure proc {a=a;read a;}",
			"procedure proc {a=a;print a;}",
			"procedure proc {a=a;call a;}",
			"procedure proc {a=a;a=a;}",
		};

		for each (auto prog in progs) {
			output_t actual = c.makeCfg(prog);
			Assert::IsTrue(actual == expected);
		}
	}

	TEST_METHOD(Test_oneProc_simpleIf) {
		auto prog =
			"procedure proc {"
			"read a;"
			"if(a>a)"
			"	then{read a;}"
			"	else{read a;}"
			"read a;"
			"}";
		output_t actual = c.makeCfg(prog);

		output_t expected = c.outputConverter({ Cfg("proc",1) });
		expected.at("proc").insert(1, 2);
		expected.at("proc").insert(2, 3);
		expected.at("proc").insert(2, 4);
		expected.at("proc").insert(3, -2);
		expected.at("proc").insert(4, -2);
		expected.at("proc").insert(-2, 5);
		expected.at("proc").insert(5,0);

		Assert::IsTrue(actual == expected);

		auto actualT = c.makeCfgTyped(prog);
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(1));
		Assert::IsTrue(Ent::if_stmtType == actualT[0].getStmtType(2));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(3));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(4));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(5));
		Assert::IsTrue("" == actualT[0].getStmtType(6));
		Assert::IsTrue("" == actualT[0].getStmtType(-2));
	}

	TEST_METHOD(Test_oneProc_simpleWhile) {
		auto prog =
			"procedure proc {"
			"read a;"		// 1
			"while(a>a){"	// 2
			"	read a;"	// 3
			"	read a;}"	// 4
			"}";
		output_t actual = c.makeCfg(prog);

		output_t expected = c.outputConverter({ Cfg("proc",1) });
		expected.at("proc").insert(1, 2);
		expected.at("proc").insert(2, 3);
		expected.at("proc").insert(3, 4);
		expected.at("proc").insert(4, -2);
		expected.at("proc").insert(-2, 2);
		expected.at("proc").insert(2, 0);

		Assert::IsTrue(actual == expected);

		auto actualT = c.makeCfgTyped(prog);
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(1));
		Assert::IsTrue(Ent::while_stmtType == actualT[0].getStmtType(2));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(3));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(4));
		Assert::IsTrue("" == actualT[0].getStmtType(6));
		Assert::IsTrue("" == actualT[0].getStmtType(-2));

	}

	TEST_METHOD(Test_oneProc_forkedIf) {
		auto prog =
			"procedure proc {"
			"if(a>a)"					// 1
			"	then{if(a==a)then{read a;}else{read a;}}" // 2-->3,4
			"	else{if(a==a)then{read a;}else{read a;}}" // 5-->6,7
			"read a;"					// 8
			"}";
		output_t actual = c.makeCfg(prog);

		output_t expected = c.outputConverter({ Cfg("proc",1) });
		expected.at("proc").insert(1, 2);
		expected.at("proc").insert(1, 5);

		expected.at("proc").insert(2, 3);
		expected.at("proc").insert(2, 4);
		expected.at("proc").insert(3, -2);
		expected.at("proc").insert(4, -2);

		expected.at("proc").insert(5, 6);
		expected.at("proc").insert(5, 7);
		expected.at("proc").insert(6, -5);
		expected.at("proc").insert(7, -5);

		expected.at("proc").insert(-2, -1);
		expected.at("proc").insert(-5, -1);
		expected.at("proc").insert(-1, 8);
		expected.at("proc").insert(8,0);

		Assert::IsTrue(actual == expected);

		auto actualT = c.makeCfgTyped(prog);
		Assert::IsTrue(Ent::if_stmtType == actualT[0].getStmtType(1));
		Assert::IsTrue(Ent::if_stmtType == actualT[0].getStmtType(2));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(3));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(4));
		Assert::IsTrue(Ent::if_stmtType == actualT[0].getStmtType(5));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(6));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(7));
		Assert::IsTrue(Ent::read_stmtType == actualT[0].getStmtType(8));
	}

	TEST_METHOD(Test_getAssignStmt) {
		auto prog =
			"procedure proc {"
			"a=1;"		// 1
			"while(a>a){"	// 2
			"	b=2;"	// 3
			"}"
			"}";
	
		auto actualT = c.makeCfgTyped(prog);
		auto t = actualT[0].getAssignStmts();
		Assert::IsTrue(
			actualT[0].getAssignStmts() == std::vector<stmtNo_t>{1, 3}
		);

	}

		














};
}
