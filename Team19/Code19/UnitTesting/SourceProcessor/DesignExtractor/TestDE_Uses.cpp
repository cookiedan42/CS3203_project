#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <unordered_set>
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/RelationshipExtractor/UsesExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using UsesStub = std::string;
using OutputStub = std::unordered_set<UsesStub>;

struct UsesTestHelper {
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
	Ent::Variable Var(int lineNo, std::string value) {
		return Ent::Variable( value);
	}
	Ent::Variable Var(std::string value) {
		return Ent::Variable(value);
	}
	Ent::Procedure Procedure(std::string name) {
		return Ent::Procedure(name);
	}
	Rel::Uses Uses(Ent::Statement s, Ent::Variable v) {
		return Rel::Uses(s, v);
	}
	Rel::Uses Uses(Ent::Procedure p, Ent::Variable v) {
		return Rel::Uses(p, v);
	}

	UsesStub makeStub(Rel::Uses u) {
		auto castedArg1 = dynamic_pointer_cast<Ent::Statement>(u.getArg1());
		// the important data is the two lineNo
		return castedArg1 ?
			std::to_string(castedArg1->getStmtNo()) + " " + castedArg1->getName() + " " + u.getArg2()->getName() :
			u.getArg1()->getName() + " " + u.getArg2()->getName();
	}

	OutputStub outputConverter(std::vector<Rel::Uses> original) {
		OutputStub res {};
		for each (auto f in original) {
			res.insert(makeStub(f));
		}
		return res;
	}

	OutputStub makeUses(std::string inProgram) {
		auto tokens = TokenSplitter(inProgram).tokenize();
		auto AST = Parser(tokens).parseTokens();
		auto output = UsesExtractor().extractUses(AST);
		return outputConverter(output);
	}

};
auto u = UsesTestHelper();

template<typename T>
void assertSize(size_t size, T to_test) {
	Assert::AreEqual(size, to_test.size());
}


namespace UnitTesting {
	TEST_CLASS(TestDE_Uses) {
public:

	TEST_METHOD(Test_Read) {
		auto prog = "procedure name{ read a; }";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Print) {
		auto prog = "procedure name{ print a; }";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("name"), u.Var(1, "a")),
			u.Uses(u.Print(1, "a"),u.Var(1,"a"))
			});

		Assert::IsTrue(output == testStub);
	}
	
	TEST_METHOD(Test_Assign) {
		auto prog = "procedure name{ a=b; }";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("name"), u.Var(1, "b")),
			u.Uses(u.Assign(1),u.Var(1,"b")),
		});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Assign_expr) {
		auto prog = "procedure name{ a=(b+c)/d; }";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("name"), u.Var(1, "b")),
			u.Uses(u.Procedure("name"), u.Var(1, "c")),
			u.Uses(u.Procedure("name"), u.Var(1, "d")),
			u.Uses(u.Assign(1),u.Var(1,"b")),
			u.Uses(u.Assign(1),u.Var(1,"c")),
			u.Uses(u.Assign(1),u.Var(1,"d")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Nested_If) {
		auto prog = "procedure name{ if(c==c)then{print a;}else{print b;} }";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("name"), u.Var(1, "c")),
			u.Uses(u.Procedure("name"), u.Var(2, "a")),
			u.Uses(u.Procedure("name"), u.Var(3, "b")),
			u.Uses(u.If(1),u.Var(1,"c")),
			u.Uses(u.If(1),u.Var(2,"a")),
			u.Uses(u.If(1),u.Var(3,"b")),
			u.Uses(u.Print(2, "a"),u.Var(2,"a")),
			u.Uses(u.Print(3, "b"),u.Var(3,"b")),
		});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Nested_While) {
		auto prog = "procedure name{ while(c==c){print a;} }";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("name"), u.Var(1, "c")),
			u.Uses(u.Procedure("name"), u.Var(2, "a")),
			u.Uses(u.While(1),u.Var(1,"c")),
			u.Uses(u.While(1),u.Var(2,"a")),
			u.Uses(u.Print(2, "a"),u.Var(2,"a")),
		});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Nested_While_nest) {
		auto prog =
			"procedure name{"
			"	while(c==c){" // 1
			"	while(b==b){" // 2 
			"		print a;" // 3
			"	}}			"
			"}				";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("name"), u.Var(1, "c")),
			u.Uses(u.Procedure("name"), u.Var(2, "b")),
			u.Uses(u.Procedure("name"), u.Var(3, "a")),
			u.Uses(u.While(1),u.Var(1,"c")),
			u.Uses(u.While(1),u.Var(2,"b")),
			u.Uses(u.While(1),u.Var(3,"a")),
			u.Uses(u.While(2),u.Var(2,"b")),
			u.Uses(u.While(2),u.Var(3,"a")),
			u.Uses(u.Print(3, "a"),u.Var(3,"a")),
		});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Nested_While_nest_dupe) {
		auto prog =
			"procedure name{"
			"	while(c==c){" // 1
			"	while(c==c){" // 2 
			"		print c;" // 3
			"	}}			"
			"}				";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("name"), u.Var(3, "c")),
			u.Uses(u.While(1),u.Var(1,"c")),
			u.Uses(u.While(2),u.Var(2,"c")),
			u.Uses(u.Print(3, "c"),u.Var(3,"c")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Nested_While_nests) {
		auto prog = 
			"procedure name{"
			"	while(f>=1){" // 1
			"	while(e>1){" // 2 
			"	while(d<1){" // 3
			"	while(c<=1){" // 4
			"		print b;" // 5
			"	}}}}		"
			"}				";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter( {
			u.Uses(u.Procedure("name"), u.Var(1, "f")),
			u.Uses(u.Procedure("name"), u.Var(2, "e")),
			u.Uses(u.Procedure("name"), u.Var(3, "d")),
			u.Uses(u.Procedure("name"), u.Var(4, "c")),
			u.Uses(u.Procedure("name"), u.Var(5, "b")),

			u.Uses(u.While(1),u.Var(1,"f")),
			u.Uses(u.While(1),u.Var(2,"e")),
			u.Uses(u.While(1),u.Var(3,"d")),
			u.Uses(u.While(1),u.Var(4,"c")),
			u.Uses(u.While(1),u.Var(5,"b")),

			u.Uses(u.While(2),u.Var(2,"e")),
			u.Uses(u.While(2),u.Var(3,"d")),
			u.Uses(u.While(2),u.Var(4,"c")),
			u.Uses(u.While(2),u.Var(5,"b")),

			u.Uses(u.While(3),u.Var(3,"d")),
			u.Uses(u.While(3),u.Var(4,"c")),
			u.Uses(u.While(3),u.Var(5,"b")),

			u.Uses(u.While(4),u.Var(4,"c")),
			u.Uses(u.While(4),u.Var(5,"b")),

			u.Uses(u.Print(5, "b"),u.Var(5,"b")),

			});
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Multiple_Procedure) {
		auto prog =
			"procedure p{"
			"	x = y;" // 1
			"}"
			"procedure p1 {"
			"	print z;" // 2
			"}";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("p"), u.Var(1, "y")),
			u.Uses(u.Assign(1),u.Var(1,"y")),
			u.Uses(u.Procedure("p1"), u.Var(2, "z")),
			u.Uses(u.Print(2, "z"),u.Var(2,"z")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Procedure_Call) {
		auto prog =
			"procedure p{"
			"	call p1;" // 1
			"}"
			"procedure p1 {"
			"	x = y;" // 2
			"	print z;" // 3
			"}";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("p"), u.Var(2, "y")),
			u.Uses(u.Procedure("p"), u.Var(3, "z")),
			u.Uses(u.Procedure("p1"), u.Var(2, "y")),
			u.Uses(u.Procedure("p1"), u.Var(3, "z")),
			u.Uses(u.Call(1, "p"), u.Var(2, "y")),
			u.Uses(u.Call(1, "p"), u.Var(3, "z")),
			u.Uses(u.Assign(2),u.Var(2, "y")),
			u.Uses(u.Print(3, "z"),u.Var(3, "z")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Procedure_Multiple_Call) {
		auto prog =
			"procedure p{"
			"	call p1;" // 1
			"	call p2;" // 2
			"}"
			"procedure p1 {"
			"	x = y;" // 3
			"	print z;" // 4
			"}"
			"procedure p2 {"
			"	a = b;" // 5
			"	print c;" // 6
			"}";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("p"), u.Var(3, "y")),
			u.Uses(u.Procedure("p"), u.Var(4, "z")),
			u.Uses(u.Procedure("p"), u.Var(5, "b")),
			u.Uses(u.Procedure("p"), u.Var(6, "c")),
			u.Uses(u.Procedure("p1"), u.Var(3, "y")),
			u.Uses(u.Procedure("p1"), u.Var(4, "z")),
			u.Uses(u.Procedure("p2"), u.Var(5, "b")),
			u.Uses(u.Procedure("p2"), u.Var(6, "c")),
			u.Uses(u.Call(1, "p"), u.Var(3, "y")),
			u.Uses(u.Call(1, "p"), u.Var(4, "z")),
			u.Uses(u.Call(2, "p"), u.Var(5, "b")),
			u.Uses(u.Call(2, "p"), u.Var(6, "c")),
			u.Uses(u.Assign(3),u.Var(3, "y")),
			u.Uses(u.Print(4, "p1"),u.Var(4, "z")),
			u.Uses(u.Assign(5),u.Var(5, "b")),
			u.Uses(u.Print(6, "p2"),u.Var(6, "c")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Procedure_Call_Nested) {
		auto prog =
			"procedure p{"
			"	while(a == b) {" // 1
			"	while(c == d) {" // 2
			"		call p1;" // 3
			"	}}"
			"}"
			"procedure p1 {"
			"	x = y;" // 4
			"	print z;" // 5
			"}";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("p"), u.Var(1, "a")),
			u.Uses(u.Procedure("p"), u.Var(1, "b")),
			u.Uses(u.Procedure("p"), u.Var(2, "c")),
			u.Uses(u.Procedure("p"), u.Var(2, "d")),
			u.Uses(u.Procedure("p"), u.Var(4, "y")),
			u.Uses(u.Procedure("p"), u.Var(5, "z")),
			u.Uses(u.Procedure("p1"), u.Var(4, "y")),
			u.Uses(u.Procedure("p1"), u.Var(5, "z")),
			u.Uses(u.While(1), u.Var(1, "a")),
			u.Uses(u.While(1), u.Var(1, "b")),
			u.Uses(u.While(1), u.Var(2, "c")),
			u.Uses(u.While(1), u.Var(2, "d")),
			u.Uses(u.While(1), u.Var(4, "y")),
			u.Uses(u.While(1), u.Var(5, "z")),
			u.Uses(u.While(2), u.Var(2, "c")),
			u.Uses(u.While(2), u.Var(2, "d")),
			u.Uses(u.While(2), u.Var(4, "y")),
			u.Uses(u.While(2), u.Var(5, "z")),
			u.Uses(u.Call(3, "p"), u.Var(4, "y")),
			u.Uses(u.Call(3, "p"), u.Var(5, "z")),
			u.Uses(u.Assign(4),u.Var(4, "y")),
			u.Uses(u.Print(5, "z"),u.Var(5, "z")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Procedure_Indirect_Call) {
		auto prog =
			"procedure p{"
			"	call p1;" // 1
			"}"
			"procedure p1 {"
			"	call p2;" // 2
			"}"
			"procedure p2 {"
			"	x = y;" // 3
			"	print z;" // 4
			"}";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("p"), u.Var(3, "y")),
			u.Uses(u.Procedure("p"), u.Var(4, "z")),
			u.Uses(u.Procedure("p1"), u.Var(3, "y")),
			u.Uses(u.Procedure("p1"), u.Var(4, "z")),
			u.Uses(u.Procedure("p2"), u.Var(3, "y")),
			u.Uses(u.Procedure("p2"), u.Var(4, "z")),
			u.Uses(u.Call(1, "p"), u.Var(3, "y")),
			u.Uses(u.Call(1, "p"), u.Var(4, "z")),
			u.Uses(u.Call(2, "p1"), u.Var(3, "y")),
			u.Uses(u.Call(2, "p1"), u.Var(4, "z")),
			u.Uses(u.Assign(3),u.Var(3, "y")),
			u.Uses(u.Print(4, "p2"),u.Var(4, "z")),
			});

		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Procedure_Indirect_Call_Nested) {
		auto prog =
			"procedure p{"
			"	while(a == b) {" // 1
			"		print c;" // 2
			"		call p1;" // 3
			"	}"
			"}"
			"procedure p1 {"
			"	print d;" // 4
			"	call p2;" // 5
			"}"
			"procedure p2 {"
			"	x = y;" // 6
			"	print z;" // 7
			"}";
		auto output = u.makeUses(prog);
		OutputStub testStub = u.outputConverter({
			u.Uses(u.Procedure("p"), u.Var(1, "a")),
			u.Uses(u.Procedure("p"), u.Var(1, "b")),
			u.Uses(u.Procedure("p"), u.Var(2, "c")),
			u.Uses(u.Procedure("p"), u.Var(4, "d")),
			u.Uses(u.Procedure("p"), u.Var(6, "y")),
			u.Uses(u.Procedure("p"), u.Var(7, "z")),
			u.Uses(u.Procedure("p1"), u.Var(4, "d")),
			u.Uses(u.Procedure("p1"), u.Var(6, "y")),
			u.Uses(u.Procedure("p1"), u.Var(7, "z")),
			u.Uses(u.Procedure("p2"), u.Var(6, "y")),
			u.Uses(u.Procedure("p2"), u.Var(7, "z")),
			u.Uses(u.While(1), u.Var(1, "a")),
			u.Uses(u.While(1), u.Var(1, "b")),
			u.Uses(u.While(1), u.Var(2, "c")),
			u.Uses(u.While(1), u.Var(4, "d")),
			u.Uses(u.While(1), u.Var(6, "y")),
			u.Uses(u.While(1), u.Var(7, "z")),
			u.Uses(u.Print(2, "c"),u.Var(2, "c")),
			u.Uses(u.Call(3, "p"), u.Var(4, "d")),
			u.Uses(u.Call(3, "p"), u.Var(6, "y")),
			u.Uses(u.Call(3, "p"), u.Var(7, "z")),
			u.Uses(u.Print(4, "d"),u.Var(4, "d")),
			u.Uses(u.Call(5, "p1"), u.Var(6, "y")),
			u.Uses(u.Call(5, "p1"), u.Var(7, "z")),
			u.Uses(u.Assign(6),u.Var(6, "y")),
			u.Uses(u.Print(7, "z"),u.Var(7, "z")),
			});

		Assert::IsTrue(output == testStub);
	}
};
}
