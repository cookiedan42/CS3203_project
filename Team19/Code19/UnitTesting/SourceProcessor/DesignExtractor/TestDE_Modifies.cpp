#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <unordered_set>
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/RelationshipExtractor/ModifiesExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using ModifiesStub = std::string;
using OutputStub = std::unordered_set<ModifiesStub>;

struct ModifiesTestHelper {
	// convinience functions for creating Follows
	Ent::Statement Stmt(int lineNo, std::string stmtType) {
		return Ent::Statement(lineNo, stmtType);
	}
	Ent::Statement Read(int lineNo, std::string varName) {
		return Ent::ReadStatement(lineNo, varName);
	}
	Ent::Statement Print(int lineNo, std::string varName) {
		return Ent::PrintStatement(lineNo, varName);
	}
	Ent::Statement Assign(int lineNo) {
		return Ent::AssignStatement(lineNo);
	}
	Ent::Statement If(int lineNo) {
		return Ent::IfStatement(lineNo);
	}
	Ent::Statement While(int lineNo) {
		return Ent::WhileStatement(lineNo);
	}
	Ent::Statement Call(int lineNo, std::string procName) {
		return Ent::CallStatement(lineNo, procName);
	}
	Ent::Variable Var( std::string value) {
		return Ent::Variable(value);
	}
	Ent::Procedure Procedure(std::string name) {
		return Ent::Procedure(name);
	}
	Rel::Modifies Modifies(Ent::Statement s, Ent::Variable v) {
		return Rel::Modifies(s, v);
	}
	Rel::Modifies Modifies(Ent::Procedure p, Ent::Variable v) {
		return Rel::Modifies(p, v);
	}

	ModifiesStub makeStub(Rel::Modifies m) {
		auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(m.getArg1());
		return castedArg1 ?
			std::to_string(castedArg1->getStmtNo()) + " " + castedArg1->getName() + " " + m.getArg2()->getName() :
			m.getArg1()->getName() + " " + m.getArg2()->getName();
	}

	OutputStub outputConverter(std::vector<Rel::Modifies> original) {
		OutputStub res {};
		for each (auto f in original) {
			res.insert(makeStub(f));
		}
		return res;
	}

	OutputStub makeModifies(std::string inProgram) {
		auto tokens = TokenSplitter(inProgram).tokenize();
		auto AST = Parser(tokens).parseTokens();
		auto output = ModifiesExtractor().extractModifies(AST);
		return outputConverter(output);
	}

};
auto m = ModifiesTestHelper();

template<typename T>
void assertSize(size_t size, T to_test) {
	Assert::AreEqual(size, to_test.size());
}


namespace UnitTesting {
	TEST_CLASS(TestDE_Modifies) {
public:

	TEST_METHOD(Test_Read) {
		auto prog = "procedure name{ read a; }";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("name"), m.Var( "a")),
			m.Modifies(m.Read(1, "a"),m.Var("a")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Print) {
		auto prog = "procedure name{ print a; }";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Assign) {
		auto prog = "procedure name{ a=b; }";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("name"), m.Var("a")),
			m.Modifies(m.Assign(1),m.Var("a"))
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Nested_If) {
		auto prog = "procedure name{ if(c==c)then{read a;}else{read b;} }";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("name"), m.Var( "a")),
			m.Modifies(m.Procedure("name"), m.Var( "b")),
			m.Modifies(m.If(1),m.Var("a")),
			m.Modifies(m.If(1),m.Var("b")),
			m.Modifies(m.Read(2, "a"),m.Var("a")),
			m.Modifies(m.Read(3, "b"),m.Var("b")),
			});

		Assert::IsTrue(output == testStub);
	}
	
	TEST_METHOD(Test_Nested_While) {
		auto prog = "procedure name{ while(c==c){read a;} }";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("name"), m.Var( "a")),
			m.Modifies(m.While(1),m.Var("a")),
			m.Modifies(m.Read(2, "a"),m.Var("a")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Nested_While_nested) {
		auto prog =
			"procedure name{"
			"	while(c==c){" // 1
			"	while(c==c){" // 2 
			"	while(c==c){" // 3
			"	while(c==c){" // 4
			"	while(c==c){" // 5
			"		read a;" // 6
			"	}}}}}		"
			"}				";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("name"), m.Var( "a")),
			m.Modifies(m.While(1),m.Var("a")),
			m.Modifies(m.While(2),m.Var("a")),
			m.Modifies(m.While(3),m.Var("a")),
			m.Modifies(m.While(4),m.Var("a")),
			m.Modifies(m.While(5),m.Var("a")),
			m.Modifies(m.Read(6, "a"),m.Var("a")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Multiple_Procedure) {
		auto prog =
			"procedure p{"
			"	x = y;" // 1
			"}"
			"procedure p1 {"
			"	read z;" // 2
			"}";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("p"), m.Var( "x")),
			m.Modifies(m.Assign(1),m.Var( "x")),
			m.Modifies(m.Procedure("p1"), m.Var( "z")),
			m.Modifies(m.Read(2, "z"),m.Var("z")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Procedure_Call) {
		auto prog =
			"procedure p {"
			"	call p1;" // 1
			"}"
			"procedure p1 {"
			"	x = y;" // 2
			"	read z;" // 3
			"}";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("p"), m.Var( "x")),
			m.Modifies(m.Procedure("p"), m.Var( "z")),
			m.Modifies(m.Procedure("p1"), m.Var( "x")),
			m.Modifies(m.Procedure("p1"), m.Var( "z")),
			m.Modifies(m.Call(1, "p"), m.Var("x")),
			m.Modifies(m.Call(1, "p"), m.Var("z")),
			m.Modifies(m.Assign(2), m.Var( "x")),
			m.Modifies(m.Read(3, "z"), m.Var("z")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Procedure_Multiple_Call) {
		auto prog =
			"procedure p {"
			"	call p1;" // 1
			"	call p2;" // 2
			"}"
			"procedure p1 {"
			"	x = y;" // 3
			"	read z;" // 4
			"}"
			"procedure p2 {"
			"	a = b;" // 5
			"	read c;" // 6
			"}";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("p"), m.Var( "x")),
			m.Modifies(m.Procedure("p"), m.Var( "z")),
			m.Modifies(m.Procedure("p"), m.Var( "a")),
			m.Modifies(m.Procedure("p"), m.Var( "c")),
			m.Modifies(m.Procedure("p1"), m.Var( "x")),
			m.Modifies(m.Procedure("p1"), m.Var( "z")),
			m.Modifies(m.Procedure("p2"), m.Var( "a")),
			m.Modifies(m.Procedure("p2"), m.Var( "c")),
			m.Modifies(m.Call(1, "p"), m.Var("x")),
			m.Modifies(m.Call(1, "p"), m.Var("z")),
			m.Modifies(m.Call(2, "p1"), m.Var("a")),
			m.Modifies(m.Call(2, "p1"), m.Var("c")),
			m.Modifies(m.Assign(3), m.Var( "x")),
			m.Modifies(m.Read(4, "z"), m.Var("z")),
			m.Modifies(m.Assign(5), m.Var( "a")),
			m.Modifies(m.Read(6, "c"), m.Var("c")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Proceduree_Call_Nested) {
		auto prog =
			"procedure p{"
			"	while(a == b) {" // 1
			"	while(c == d) {" // 2
			"		call p1;" // 3
			"	}}"
			"}"
			"procedure p1 {"
			"	x = y;" // 4
			"	read z;" // 5
			"}";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("p"), m.Var( "x")),
			m.Modifies(m.Procedure("p"), m.Var( "z")),
			m.Modifies(m.Procedure("p1"), m.Var( "x")),
			m.Modifies(m.Procedure("p1"), m.Var("z")),
			m.Modifies(m.While(1), m.Var( "x")),
			m.Modifies(m.While(1), m.Var( "z")),
			m.Modifies(m.While(2), m.Var( "x")),
			m.Modifies(m.While(2), m.Var( "z")),
			m.Modifies(m.Call(3, "p"), m.Var( "x")),
			m.Modifies(m.Call(3, "p"), m.Var( "z")),
			m.Modifies(m.Assign(4), m.Var( "x")),
			m.Modifies(m.Read(5, "z"), m.Var("z")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Proceduree_Indirect_Call) {
		auto prog =
			"procedure p{"
			"	call p1;" // 1
			"}"
			"procedure p1 {"
			"	call p2;" // 2
			"}"
			"procedure p2 {"
			"	x = y;" // 3
			"	read z;" // 4
			"}";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("p"), m.Var("x")),
			m.Modifies(m.Procedure("p"), m.Var("z")),
			m.Modifies(m.Procedure("p1"), m.Var( "x")),
			m.Modifies(m.Procedure("p1"), m.Var( "z")),
			m.Modifies(m.Procedure("p2"), m.Var( "x")),
			m.Modifies(m.Procedure("p2"), m.Var( "z")),
			m.Modifies(m.Call(1, "p"), m.Var( "x")),
			m.Modifies(m.Call(1, "p"), m.Var( "z")),
			m.Modifies(m.Call(2, "p1"), m.Var( "x")),
			m.Modifies(m.Call(2, "p1"), m.Var( "z")),
			m.Modifies(m.Assign(3), m.Var( "x")),
			m.Modifies(m.Read(4, "z"), m.Var("z")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Proceduree_Indirect_Call_Nested) {
		auto prog =
			"procedure p{"
			"	while(a == b) {" // 1
			"		read c;" // 2
			"		call p1;" // 3
			"	}"
			"}"
			"procedure p1 {"
			"	read d;" // 4
			"	call p2;" // 5
			"}"
			"procedure p2 {"
			"	x = y;" // 6
			"	read z;" // 7
			"}";
		auto output = m.makeModifies(prog);
		OutputStub testStub = m.outputConverter({
			m.Modifies(m.Procedure("p"), m.Var( "c")),
			m.Modifies(m.Procedure("p"), m.Var( "d")),
			m.Modifies(m.Procedure("p"), m.Var( "x")),
			m.Modifies(m.Procedure("p"), m.Var( "z")),
			m.Modifies(m.Procedure("p1"), m.Var( "d")),
			m.Modifies(m.Procedure("p1"), m.Var( "x")),
			m.Modifies(m.Procedure("p1"), m.Var( "z")),
			m.Modifies(m.Procedure("p2"), m.Var( "x")),
			m.Modifies(m.Procedure("p2"), m.Var( "z")),
			m.Modifies(m.While(1), m.Var( "c")),
			m.Modifies(m.While(1), m.Var( "d")),
			m.Modifies(m.While(1), m.Var( "x")),
			m.Modifies(m.While(1), m.Var("z")),
			m.Modifies(m.Read(2, "c"), m.Var("c")),
			m.Modifies(m.Call(3, "p"), m.Var( "d")),
			m.Modifies(m.Call(3, "p"), m.Var( "x")),
			m.Modifies(m.Call(3, "p"), m.Var( "z")),
			m.Modifies(m.Read(4, "d"), m.Var("d")),
			m.Modifies(m.Call(5, "p1"), m.Var( "x")),
			m.Modifies(m.Call(5, "p1"), m.Var( "z")),
			m.Modifies(m.Assign(6), m.Var( "x")),
			m.Modifies(m.Read(7, "p2"), m.Var("z")),
			});

		Assert::IsTrue(output == testStub);
	}

};
}
