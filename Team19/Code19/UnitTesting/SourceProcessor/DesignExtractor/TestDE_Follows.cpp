#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <unordered_set>
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/RelationshipExtractor/FollowsExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using FollowsStub = std::string;
using OutputStub = std::unordered_set<FollowsStub>;

struct FollowsTestHelper {
	// convinience functions for creating Follows
	Statement Read(int lineNo, std::string varName) {
		return Ent::ReadStatement(lineNo, varName);
	}
	Statement Print(int lineNo, std::string varName) {
		return Ent::PrintStatement(lineNo, varName);
	}
	Statement Assign(int lineNo) {
		return Ent::AssignStatement(lineNo);
	}
	Statement If(int lineNo) {
		return Ent::IfStatement(lineNo);
	}
	Statement While(int lineNo) {
		return Ent::WhileStatement(lineNo);
	}
	Statement Call(int lineNo, std::string procName) {
		return Ent::CallStatement(lineNo, procName);
	}
	Rel::Follows Follows(Statement e, Statement f) {
		return Rel::Follows(e, f);
	}

	FollowsStub makeStub(Rel::Follows f) {
		// stmtNo1 StmtType1 stmtNo2 StmtType2
		auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(f.getArg1());
		auto castedArg2 = dynamic_pointer_cast<Ent::Statement>(f.getArg2());
		return std::to_string(castedArg1->getStmtNo())
			+ " " + castedArg1->getName()
			+ " " + std::to_string(castedArg2->getStmtNo())
			+ " " + castedArg2->getName();
	}

	OutputStub outputConverter(std::vector<Rel::Follows> original) {
		OutputStub res {};
		for each (auto f in original) {
			res.insert(makeStub(f));
		}
		return res;
	}

	OutputStub makeFollows(std::string inProgram) {
		auto tokens = TokenSplitter(inProgram).tokenize();
		auto AST = Parser(tokens).parseTokens();
		auto output =  FollowsExtractor().extractFollows(AST);
		return outputConverter(output);
	}

};
auto f = FollowsTestHelper();

template<typename T>
void assertSize(size_t size, T to_test) {
	Assert::AreEqual(size, to_test.size());
}


namespace UnitTesting {
	TEST_CLASS(TestDE_Follows) {
public:

	TEST_METHOD(Test_2Statement_correct) {
		auto prog = "procedure name{ read a; print b; }";
		auto actual = f.makeFollows(prog);
		OutputStub expected = f.outputConverter({
			f.Follows(f.Read(1, "a"),f.Print(2, "b")),
			});
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_3Statement_correct) {
		auto prog = 
			"procedure name{"
				"read a;"
				"read b;"
				"print c;"
			"}";
		auto actual = f.makeFollows(prog);
		OutputStub expected = f.outputConverter({
			f.Follows(f.Read(1, "a"),f.Read(2, "b")),
			f.Follows(f.Read(2, "b"),f.Print(3, "c")),
			});
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_DifferentBasicStatements_correct) {
		auto prog =
			"procedure name{"
				"read a;"
				"b = c+f;"
				"print c;"
			"}";
		auto actual = f.makeFollows(prog);
		OutputStub expected = f.outputConverter({
			f.Follows(f.Read(1, "a"),f.Assign(2)),
			f.Follows(f.Assign(2),f.Print(3, "c")),
			});
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_NestedIf_DifferentBranchNoFollow) {
		auto prog = 
			"procedure name{"
				"if(a<b)then{read a;}else{read a;}"
			"}";
		auto actual = f.makeFollows(prog);
		OutputStub expected = f.outputConverter({});
		assertSize(0, actual);
		Assert::IsTrue(actual == expected);
	}
	
	TEST_METHOD(Test_NestWhile_NoFollow) {
		auto prog = 
			"procedure name{"
				"while(a<b){read a;}"
			"}";
		auto actual = f.makeFollows(prog);
		OutputStub expected = f.outputConverter({});
		assertSize(0, actual);
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_BeforeAfterIfWhile_SameLevelFollow) {
		auto prog =
			"procedure name{"
				"read a;"
				"while(a>b){read d;}"
				"print c;"
				"if(a<b)then{read a;}else{read a;}"
				"print d;"
			"}";
		auto actual = f.makeFollows(prog);
		OutputStub expected = f.outputConverter({
			f.Follows(f.Read(1, "a"),f.While(2)),
			f.Follows(f.While(2),f.Print(4, "c")),
			f.Follows(f.Print(4, "c"),f.If(5)),
			f.Follows(f.If(5),f.Print(8, "d")),
			});
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_DifferentProc_NoFollow) {
		auto prog =
			"procedure name{read a;}"
			"procedure name2{read a;}";
		auto actual = f.makeFollows(prog);
		OutputStub expected = f.outputConverter({});
		Assert::IsTrue(actual == expected);
	}








	};
}
