#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/CfgExtractor.h"
#include "../source/Cfg/Cfg.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(TestCfg) {

	using stmtNoSet_t = std::set<stmtNo_t>;
	using stmtPair_t = std::vector<std::pair<stmtNo_t, stmtNo_t>>;

	bool stmtPairEq(stmtPair_t a, stmtPair_t b) {
		unordered_set<std::string> aa;
		unordered_set<std::string> bb;
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
	Cfg OneStmtCfg() {
		auto inProgram = "procedure proc {read a;}";
		return makeCfg(inProgram)[0];
	}
	Cfg TwoStmtCfg(){
		auto inProgram = "procedure proc {read a;read a;}";
		return makeCfg(inProgram)[0];
	}
	Cfg ThreeStmtCfg() {
		auto inProgram = "procedure proc {read a;print a;call a;}procedure b{read a;}";
		return makeCfg(inProgram)[0];
	}
	Cfg IfBeforeAfter() {
		auto inProgram =
			"procedure proc {"
			"	read a;"			// 1
			"	if(a==a)"			// 2
			"		then{read a;}"	// 3
			"		else{read a;}"	// 4
			"	read a;"			// 5
			"}";
		return makeCfg(inProgram)[0];
	}
	Cfg IfSimple() {
		auto inProgram =
			"procedure proc {"
			"	if(a==a)"			// 1
			"		then{read a;}"	// 2
			"		else{read a;}"	// 3
			"}";
		return makeCfg(inProgram)[0];
	}
	Cfg WhileSimple() {
		auto inProgram =
			"procedure proc {"
			"	while(a==a)"	// 1
			"		{read a;}"	// 2
			"	read a;"		// 3
			"}";
		return makeCfg(inProgram)[0];
	}

public:
	TEST_METHOD(Test_equals_true) {
		// with direct calls to insert 
		auto basicEmpty = Cfg("procedure", 1);
		auto basicEmpty2 = Cfg("procedure", 1);
		auto differentName = Cfg("procedure1", 1);
		auto differentStart = Cfg("procedure",2);
		
		Assert::IsTrue(basicEmpty == basicEmpty2);
		Assert::IsFalse(basicEmpty != basicEmpty2);
		Assert::IsFalse(basicEmpty == differentName);
		Assert::IsFalse(basicEmpty == differentStart);


		auto basicPopulated1 = Cfg("procedure", 1);
		auto basicPopulated2 = Cfg("procedure", 1);
		auto basicPopulated3 = Cfg("procedure", 1);
		basicPopulated1.insert(1, 2);
		basicPopulated2.insert(1, 2);
		basicPopulated3.insert(1, 3);

		Assert::IsTrue(basicPopulated1 == basicPopulated2);
		Assert::IsTrue(basicPopulated1 != basicPopulated3);
	}

	TEST_METHOD(Test_reinsertion_no_change) {
		// with direct calls to insert 
		auto cfg1 = Cfg("procedure", 1);
		auto cfg2 = Cfg("procedure", 1);
		Assert::IsTrue(cfg1 == cfg2);
		cfg1.insert(1, 2);
		cfg2.insert(1, 2);
		Assert::IsTrue(cfg1 == cfg2);
		cfg1.insert(1, 2);
		Assert::IsTrue(cfg1 == cfg2);
	}

	TEST_METHOD(Test_nextStar_oneStmt_correct) {
		// procedure with a single statement
		auto cfg = OneStmtCfg();
		Assert::IsFalse(cfg.hasNextStarWildWild());
		Assert::IsFalse(cfg.hasPath(1,1));
	}

	TEST_METHOD(Test_nextStar_twoStmt_correct) {
		auto cfg = TwoStmtCfg();

		//next*(int,int)
		Assert::IsTrue(cfg.hasPath(1, 2));
		Assert::IsFalse(cfg.hasPath(2, 1));
		// out of bounds
		Assert::IsFalse(cfg.hasPath(0, 2));
		Assert::IsFalse(cfg.hasPath(3, 2));
		Assert::IsFalse(cfg.hasPath(1, 0));
		Assert::IsFalse(cfg.hasPath(1, 3));


		// next*(int,syn)
		Assert::IsTrue(cfg.getNextStar(1) == stmtNoSet_t {2});
		Assert::IsTrue(cfg.getNextStar(2) == stmtNoSet_t {});
		// next*(syn,int)
		Assert::IsTrue(cfg.getPrevStar(2) == stmtNoSet_t {1});
		Assert::IsTrue(cfg.getPrevStar(1) == stmtNoSet_t {});
		
		// next*(int,_)
		Assert::IsTrue(cfg.hasNextStar(1));
		Assert::IsFalse(cfg.hasNextStar(2));
		// next*(_,int)
		Assert::IsFalse(cfg.hasPrevStar(1));
		Assert::IsTrue(cfg.hasPrevStar(2));
		//out of bounds
		Assert::IsFalse(cfg.hasNextStar(3));
		Assert::IsFalse(cfg.hasPrevStar(3));

		// next*(syn,_)
		Assert::IsTrue(cfg.getNextStarSynWild() == stmtNoSet_t { 1 });
		// next*(_,syn)
		Assert::IsTrue(cfg.getNextStarWildSyn() == stmtNoSet_t { 2 });

		// next*(_,_)
		Assert::IsTrue(cfg.hasNextStarWildWild());
		
		// next*(syn,syn)
		Assert::IsTrue(stmtPairEq(
			cfg.getNextStarSynSyn(),
			stmtPair_t { {1,2} }
		));

	}

	TEST_METHOD(Test_nextStar_threeStmt_correct) {
		auto cfg = ThreeStmtCfg();

		//next*(int,int)
		Assert::IsTrue(cfg.hasPath(1, 2));
		Assert::IsTrue(cfg.hasPath(2, 3));
		Assert::IsTrue(cfg.hasPath(1, 3));

		// next*(int,syn)
		Assert::IsTrue(cfg.getNextStar(1) == stmtNoSet_t {2,3});
		Assert::IsTrue(cfg.getNextStar(2) == stmtNoSet_t {3});
		Assert::IsTrue(cfg.getNextStar(3) == stmtNoSet_t {});
		// next*(syn,int)
		Assert::IsTrue(cfg.getPrevStar(3) == stmtNoSet_t { 2,1 });
		Assert::IsTrue(cfg.getPrevStar(2) == stmtNoSet_t {1});
		Assert::IsTrue(cfg.getPrevStar(1) == stmtNoSet_t {});

		// next*(int,_)
		Assert::IsTrue(cfg.hasNextStar(1));
		Assert::IsTrue(cfg.hasNextStar(2));
		Assert::IsFalse(cfg.hasNextStar(3));
		// next*(_,int)
		Assert::IsFalse(cfg.hasPrevStar(1));
		Assert::IsTrue(cfg.hasPrevStar(2));
		Assert::IsTrue(cfg.hasPrevStar(3));

		// next*(syn,_)
		Assert::IsTrue(cfg.getNextStarSynWild() == stmtNoSet_t { 1,2 });
		// next*(_,syn)
		Assert::IsTrue(cfg.getNextStarWildSyn() == stmtNoSet_t { 2,3 });

		// next*(_,_)
		Assert::IsTrue(cfg.hasNextStarWildWild());

		// next*(syn,syn)
		Assert::IsTrue(stmtPairEq(
			cfg.getNextStarSynSyn(),
			stmtPair_t { {1,2},{2,3},{1,3} }
		));
	}

	TEST_METHOD(Test_nextStar_forkIfSimple_correct) {
		// test that forks don't touch
		// test that if --> both forks

		auto cfg = IfSimple();
		
		// next*(int,int)
		Assert::IsFalse(cfg.hasPath(3, 2));
		Assert::IsFalse(cfg.hasPath(2, 3));

		
		// next*(int,_)
		Assert::IsTrue(cfg.hasNextStar(1));
		Assert::IsFalse(cfg.hasNextStar(2));
		Assert::IsFalse(cfg.hasNextStar(3));

		// next*(syn,syn)
		Assert::IsTrue(stmtPairEq(
			cfg.getNextStarSynSyn(),
			stmtPair_t { {1,2},{1,3} }
		));
	}
	
	TEST_METHOD(Test_nextStar_forkIfBeforeAfter_correct) {
		auto cfg = IfBeforeAfter();

		//next*(int,int)
		{
			Assert::IsTrue(cfg.hasPath(1, 2));
			Assert::IsTrue(cfg.hasPath(1, 3));
			Assert::IsTrue(cfg.hasPath(1, 4));
			Assert::IsTrue(cfg.hasPath(1, 5));

			Assert::IsTrue(cfg.hasPath(3, 5));
			Assert::IsTrue(cfg.hasPath(4, 5));

			// forks of the If 
			Assert::IsFalse(cfg.hasPath(3, 4));
			Assert::IsFalse(cfg.hasPath(4, 3));
		}

		// next*(int,syn)
		{
			Assert::IsTrue(cfg.getNextStar(3) == stmtNoSet_t { 5 });
			Assert::IsTrue(cfg.getNextStar(4) == stmtNoSet_t { 5 });
			Assert::IsTrue(cfg.getNextStar(2) == stmtNoSet_t { 3,4,5 });
		}
		// next*(syn,int)
		{
			Assert::IsTrue(cfg.getPrevStar(3) == stmtNoSet_t { 1,2 });
			Assert::IsTrue(cfg.getPrevStar(4) == stmtNoSet_t { 1,2 });
			Assert::IsTrue(cfg.getPrevStar(5) == stmtNoSet_t { 1,2,3,4 });
		}

		// next*(int,_)
		{
			Assert::IsTrue(cfg.hasNextStar(3));
			Assert::IsTrue(cfg.hasNextStar(4));
			Assert::IsFalse(cfg.hasNextStar(5));
		}
		// next*(_,int)
		{
			Assert::IsTrue(cfg.hasPrevStar(3));
			Assert::IsTrue(cfg.hasPrevStar(4));
		}

		// next*(syn,_)
		auto t = cfg.getNextStarSynWild();
		Assert::IsTrue(cfg.getNextStarSynWild() == stmtNoSet_t { 1,2,3,4 });
		// next*(_,syn)
		Assert::IsTrue(cfg.getNextStarWildSyn() == stmtNoSet_t { 5,4,3,2 });

		// next*(_,_)
		Assert::IsTrue(cfg.hasNextStarWildWild());

		auto l = cfg.getNextStarSynSyn();
		Assert::IsTrue(cfg.hasPath(1, 5));
		// next*(syn,syn)
		Assert::IsTrue(stmtPairEq(
			cfg.getNextStarSynSyn(),
			stmtPair_t { {1,2},{2,3},{2,4},{3,5},{4,5},
			{ 1,3 },{1,4},{1,5},{2,5} }
		));
	
	}

	TEST_METHOD(Test_whileSimple_paths_correct) {
		// while loop is next* of itself 

		auto cfg = WhileSimple();
		
		//next*(int,int)
		{
			Assert::IsTrue(cfg.hasPath(1, 2));
			Assert::IsTrue(cfg.hasPath(1, 3));
			Assert::IsTrue(cfg.hasPath(2, 1));

			Assert::IsTrue(cfg.hasPath(1, 1)); // 1--> 2--> 1
			Assert::IsTrue(cfg.hasPath(2, 2)); // 2--> 1--> 2
			Assert::IsTrue(cfg.hasPath(2, 3)); // 2--> 1--> 3
		}
	
		// next*(int,syn)
		{
			Assert::IsTrue(cfg.getNextStar(1) == stmtNoSet_t { 1,2,3 });
			Assert::IsTrue(cfg.getNextStar(2) == stmtNoSet_t { 1,2,3 });
			Assert::IsTrue(cfg.getNextStar(3) == stmtNoSet_t {  });
		}
		// next*(syn,int)
		{
			Assert::IsTrue(cfg.getPrevStar(3) == stmtNoSet_t { 1,2 });
			Assert::IsTrue(cfg.getPrevStar(2) == stmtNoSet_t { 1,2 });
			Assert::IsTrue(cfg.getPrevStar(1) == stmtNoSet_t { 1,2 });
		}

		// next*(int,_)
		// next*(_,int)
		// coveref by syn,int and int,syn

		// next*(syn,syn)
		Assert::IsTrue(stmtPairEq(
			cfg.getNextStarSynSyn(),
			stmtPair_t { 
				{1,2},{2,1},{1,3}, // direct
				{2,3},{1,1},{2,2} // loops
			}
		));
	}

	TEST_METHOD(Test_IfInWhile_paths_correct) {

	// each half can connect to each other
		auto inProgram = 
			"procedure proc {"
			"read a;"				// 1
			"while(a==a){"			// 2
			"	if(a==a)"			// 3
			"		then{read a;}"	// 4
			"		else{read a;}"	// 5
			"}}";

		auto cfg =  makeCfg(inProgram)[0];

		// next*(int,int)
		{
			// forks can path to each other
			Assert::IsTrue(cfg.hasPath(4, 5));
			Assert::IsTrue(cfg.hasPath(5, 4));
			// forks can path to self
			Assert::IsTrue(cfg.hasPath(4, 4));
			Assert::IsTrue(cfg.hasPath(5, 5));

			// forks can path to start
			Assert::IsTrue(cfg.hasPath(4, 2));
			Assert::IsTrue(cfg.hasPath(4, 3));
			Assert::IsTrue(cfg.hasPath(5, 2));
			Assert::IsTrue(cfg.hasPath(5, 3));
		}

		// next*(int,syn)
		{
			Assert::IsTrue(cfg.getNextStar(4) == stmtNoSet_t { 2,3,4,5 });
			Assert::IsTrue(cfg.getNextStar(5) == stmtNoSet_t { 2,3,4,5 });
		}
		// next*(syn,int)
		{
			Assert::IsTrue(cfg.getPrevStar(4) == stmtNoSet_t { 1,2,3,4,5 });
			Assert::IsTrue(cfg.getPrevStar(5) == stmtNoSet_t { 1,2,3,4,5 });
		}
	}

	TEST_METHOD(Test_IfInIf_paths_correct) {
		auto inProgram =
			"procedure proc {"
			"if(a==a)"	// 1
			"	then{if(a==a)then {read a;}else{read a;}}"	// 2,3,4
			"	else{if(a==a)then {read a;}else{read a;}}"	// 5,6,7
			"read a;"	// 8
			"}";

		auto cfg = makeCfg(inProgram)[0];

		// next*(int,int)
		{
			// outer then block
			Assert::IsFalse(cfg.hasPath(3, 5));
			Assert::IsFalse(cfg.hasPath(2, 6));
			Assert::IsFalse(cfg.hasPath(2, 7));
			Assert::IsTrue(cfg.hasPath(2, 8));

			// outer else block
			Assert::IsFalse(cfg.hasPath(5, 2));
			Assert::IsFalse(cfg.hasPath(5, 3));
			Assert::IsFalse(cfg.hasPath(5, 4));
			Assert::IsTrue(cfg.hasPath(5, 8));
		}

		// next*(int,syn)
		{
			Assert::IsTrue(cfg.getNextStar(2) == stmtNoSet_t { 3,4,8});
			Assert::IsTrue(cfg.getNextStar(5) == stmtNoSet_t { 6,7,8 });
		}
	
	
	}


	TEST_METHOD(Test_WhileInWhile_paths_correct) {
		auto inProgram =
			"procedure proc {"
			"while(a==a){"			// 1
			"	while(a==a){"			// 2
			"		read a;"	// 3
			"}}}";

		auto cfg = makeCfg(inProgram)[0];

		// next*(int,int)
		{
			Assert::IsTrue(cfg.hasPath(1, 1));
			Assert::IsTrue(cfg.hasPath(1, 2));
			Assert::IsTrue(cfg.hasPath(1, 3));

			Assert::IsTrue(cfg.hasPath(2, 1));
			Assert::IsTrue(cfg.hasPath(2, 2));
			Assert::IsTrue(cfg.hasPath(2, 3));

			Assert::IsTrue(cfg.hasPath(3, 1));
			Assert::IsTrue(cfg.hasPath(3, 2));
			Assert::IsTrue(cfg.hasPath(3, 3));
		}

		// next*(int,syn)
		{
			Assert::IsTrue(cfg.getNextStar(1) == stmtNoSet_t { 1,2,3 });
			Assert::IsTrue(cfg.getNextStar(2) == stmtNoSet_t { 1,2,3 });
			Assert::IsTrue(cfg.getNextStar(3) == stmtNoSet_t { 1,2,3 });
		}
		// next*(syn,int)
		{
			Assert::IsTrue(cfg.getPrevStar(1) == stmtNoSet_t { 1,2,3 });
			Assert::IsTrue(cfg.getPrevStar(2) == stmtNoSet_t { 1,2,3 });
			Assert::IsTrue(cfg.getPrevStar(3) == stmtNoSet_t { 1,2,3 });
		}
	}


	TEST_METHOD(Test_GetAllNext_correct) {
		auto inProgram =
			"procedure proc1 {"
			"	read a; read a;" // 1,2
			"}"
			"procedure proc1 {"
			"	read a; read a;" // 3,4
			"}";

		auto cfg = makeCfg(inProgram);
		using StrPairVec_t = std::vector<std::pair<std::string, std::string>>;
		Assert::IsTrue(cfg[0].getAllNext() == StrPairVec_t{ {"1","2"} });
		Assert::IsTrue(cfg[1].getAllNext() == StrPairVec_t{ {"3","4"} });
	}




};
}