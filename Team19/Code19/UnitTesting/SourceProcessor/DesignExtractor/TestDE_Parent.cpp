#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <unordered_set>
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/RelationshipExtractor/ParentExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using ParentStub = std::string;
using OutputStub = std::unordered_set<ParentStub>;

struct ParentTestHelper {
	// convinience functions for creating Parent
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
	Rel::Parent Parent(Statement s1, Statement s2) {
		return Rel::Parent(s1, s2);
	}

	ParentStub makeStub(Rel::Parent p) {
		auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(p.getArg1());
		auto castedArg2 = dynamic_pointer_cast<Ent::Statement>(p.getArg2());
		// stmtNo1 StmtType1 stmtNo2 StmtType2
		return std::to_string(castedArg1->getStmtNo())
			+ " " + castedArg1->getName()
			+ " " + std::to_string(castedArg2->getStmtNo())
			+ " " + castedArg2->getName();
	}

	OutputStub outputConverter(std::vector<Rel::Parent> original) {
		OutputStub res {};
		for each (auto f in original) {
			res.insert(makeStub(f));
		}
		return res;
	}

	OutputStub makeParent(std::string inProgram) {
		auto tokens = TokenSplitter(inProgram).tokenize();
		auto AST = Parser(tokens).parseTokens();
		auto output = ParentExtractor().extractParent(AST);
		return outputConverter(output);
	}

};
auto p = ParentTestHelper();

template<typename T>
void assertSize(size_t size, T to_test) {
	Assert::AreEqual(size, to_test.size());
}


namespace UnitTesting {
	TEST_CLASS(TestDE_Parent) {
public:

	TEST_METHOD(Test_If_OneChildEachSide) {
		auto prog = 
			"procedure name{"
			"if(a<b)then{"
			"	read a;"	
			"}else{"
			"	read a;"
			"}"
			"}";

		auto actual = p.makeParent(prog);
		OutputStub expected = p.outputConverter({
			p.Parent(p.If(1),p.Read(2, "a")),
			p.Parent(p.If(1),p.Read(3, "a")),
		});
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_If_MultiChildEachSide) {
		auto prog =
			"procedure name{"
			"if(a<b)then{"
			"	read a;"
			"	print a;"
			"}else{"
			"	read a;"
			"	print b;"
			"}"
			"}";

		auto actual = p.makeParent(prog);
		OutputStub expected = p.outputConverter({
			p.Parent(p.If(1),p.Read(2, "a")),
			p.Parent(p.If(1),p.Print(3, "a")),
			p.Parent(p.If(1),p.Read(4, "a")),
			p.Parent(p.If(1),p.Print(5, "b")),
			});
		Assert::IsTrue(actual == expected);
	}



	TEST_METHOD(Test_While_OneChild) {
		auto prog =
			"procedure name{"
			"while(a<b){"
			"	read a;"
			"}"
			"}";

		auto actual = p.makeParent(prog);
		OutputStub expected = p.outputConverter({
			p.Parent(p.While(1),p.Read(2, "a")),
		});
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_While_MultiChild) {
		auto prog =
			"procedure name{ "
			"while(a<b){"
			"	read a;"
			"	call other;"
			"	print c;"
			"	d=4;"
			"while(a<b){read a;}"
			"if(a<b)then{read a;}else{read a;}"
			"}"
			"}"
			"procedure other{read a;} ";

		auto actual = p.makeParent(prog);
		OutputStub expected = p.outputConverter({
			p.Parent(p.While(1),p.Read(2, "a")),
			p.Parent(p.While(1),p.Call(3, "other")),
			p.Parent(p.While(1),p.Print(4, "c")),
			p.Parent(p.While(1),p.Assign(5)),

			p.Parent(p.While(1),p.While(6)),
			p.Parent(p.While(6),p.Read(7, "a")),

			p.Parent(p.While(1),p.If(8)),
			p.Parent(p.If(8),p.Read(9, "a")),
			p.Parent(p.If(8),p.Read(10, "a")),
		}); 
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_Nested_WhileInIf) {
		auto prog = 
			"procedure name{ "
			"if(a<b)then{"
			"	while(a<b){"
			"		read a; "
			"	}"
			"}else{ "
			"	while(a<b){"
			"		read a; "
			"	}"
			"	read a;"
			"}"
			"}";

		auto actual = p.makeParent(prog);
		OutputStub expected = p.outputConverter({
			p.Parent(p.If(1),p.While(2)),
			p.Parent(p.While(2),p.Read(3, "a")),
			p.Parent(p.If(1),p.While(4)),
			p.Parent(p.While(4),p.Read(5, "a")),
			p.Parent(p.If(1),p.Read(6, "a")),

		});
		Assert::IsTrue(actual == expected);
	}


	TEST_METHOD(Test_Nested_IfInIf) {
		auto prog =
			"procedure name{ "
			"if(a<b)"
			"then{if(a<b)"
			"	then{read a;}"
			"	else{read a;}"
			"}else{if(a<b)"
			"	then{read a;}"
			"	else{read a;}"
			"}"
			"}";

		auto actual = p.makeParent(prog);
		OutputStub expected = p.outputConverter({
			p.Parent(p.If(1),p.If(2)),
			p.Parent(p.If(1),p.If(5)),

			p.Parent(p.If(2),p.Read(3, "a")),
			p.Parent(p.If(2),p.Read(4, "a")),

			p.Parent(p.If(5),p.Read(6, "a")),
			p.Parent(p.If(5),p.Read(7, "a")),
			});
		Assert::IsTrue(actual == expected);
	}
	TEST_METHOD(Test_while_DeepNest) {
		auto prog = "procedure name{ "
			"while(a<b){"
			"while(a<b){"
			"while(a<b){"
			"while(a<b){"
			"while(a<b){"
			"read a;"
			"}}}}}}";
		auto actual = p.makeParent(prog);
		OutputStub expected = p.outputConverter({
			p.Parent(p.While(1),p.While(2)),
			p.Parent(p.While(2),p.While(3)),
			p.Parent(p.While(3),p.While(4)),
			p.Parent(p.While(4),p.While(5)),
			p.Parent(p.While(5),p.Read(6, "a")),
		});
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_differentProc_CallNotParent) {
		auto prog = 
			"procedure name1{call name2;}"
			"procedure name2{read a;}";
		auto actual = p.makeParent(prog);
		OutputStub expected = p.outputConverter({});
		Assert::IsTrue(actual == expected);
	}



};
}
