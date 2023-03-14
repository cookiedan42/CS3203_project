#include "stdafx.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include <unordered_set>

#include "../source/SourceProcessor/DesignExtractor/EntityExtractor/EntityExtractor.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/Entity/Entity.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


using EntityStub = std::string;
using OutputStub = std::unordered_set<EntityStub>;

struct EntityTestHelper {
	// convinience functions for creating entities
	shared_ptr<InsertEntityRequest> Proc(std::string name) {
		return make_shared<InsertProcedureEntityRequest>(Ent::Procedure(name));
	}
	shared_ptr<InsertEntityRequest> Var(std::string name) {
		return make_shared<InsertVariableEntityRequest>(Ent::Variable(name));
	}
	shared_ptr<InsertEntityRequest> Const(std::string val) {
		return make_shared<InsertConstantEntityRequest>(Ent::Constant(val));
	}
	shared_ptr<InsertEntityRequest> Stmt(int lineNo, std::string stmtType) {
		return make_shared<InsertStatementEntityRequest>(Ent::Statement(lineNo, stmtType));
	}
	shared_ptr<InsertEntityRequest> Read(int lineNo, std::string varName) {
		return make_shared<InsertStatementEntityRequest>(Ent::ReadStatement(lineNo, varName));
	}
	shared_ptr<InsertEntityRequest> Print(int lineNo, std::string varName) {
		return make_shared<InsertStatementEntityRequest>(Ent::PrintStatement(lineNo, varName));
	}
	shared_ptr<InsertEntityRequest> Assign(int lineNo) {
		return make_shared<InsertStatementEntityRequest>(Ent::AssignStatement(lineNo));
	}
	shared_ptr<InsertEntityRequest> If(int lineNo) {
		return make_shared<InsertStatementEntityRequest>(Ent::IfStatement(lineNo));
	}
	shared_ptr<InsertEntityRequest> While(int lineNo) {
		return make_shared<InsertStatementEntityRequest>(Ent::WhileStatement(lineNo));
	}
	shared_ptr<InsertEntityRequest> Call(int lineNo, std::string procName) {
		return make_shared<InsertStatementEntityRequest>(Ent::CallStatement(lineNo, procName));
	}

	EntityStub makeStub(shared_ptr<InsertEntityRequest> e) {
		auto casted = dynamic_pointer_cast<InsertStatementEntityRequest>(e);
		if (casted) return std::to_string(casted->getStatement().getStmtNo()) + " " + casted->getStatement().getName() + " " + casted->getStatement().getEntityType();

		auto casted1 = dynamic_pointer_cast<InsertConstantEntityRequest>(e);
		if (casted1) return casted1->getConstant().getName() + casted1->getConstant().getEntityType();

		auto casted2 = dynamic_pointer_cast<InsertProcedureEntityRequest>(e);
		if (casted2) return casted2->getProcedure().getName() + casted2->getProcedure().getEntityType();

		auto casted3 = dynamic_pointer_cast<InsertVariableEntityRequest>(e);
		return casted3->getVariable().getName() + casted3->getVariable().getEntityType();
	}

	OutputStub outputConverter(std::vector<shared_ptr<InsertEntityRequest>> inp) {
		OutputStub out {};
		for (const auto& e : inp) {
			out.insert(makeStub(e));
		}
		return out;
	}

	OutputStub makeEntities(std::string prog) {
		auto tokens = TokenSplitter(prog).tokenize();
		auto ast = Parser(tokens).parseTokens();
		auto& requests = EntityExtractor().extractEntities(ast);

		return outputConverter(requests);
	}

};
auto e = EntityTestHelper();

template<typename T>
void assertSize(size_t size, T to_test) {
	Assert::AreEqual(size, to_test.size());
}

namespace UnitTesting {
	TEST_CLASS(TestDE_Entity) {
public:
	TEST_METHOD(Test_Read_Print) {
		auto prog = "procedure name{"
			"read a; print b;read a; print b;"
			"}";
		auto output = e.makeEntities(prog);

		OutputStub testStub = e.outputConverter({
			e.Proc("name"),
			e.Var("a"),
			e.Var("b"),
			e.Read(1, "a"),
			e.Print(2, "b"),
			e.Read(3, "a"),
			e.Print(4, "b"),
		});
		assertSize(7, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Assign) {
		auto prog = "procedure procedure{"
			"LHS = RHS+RHS/OK;"
			"}";
		auto output = e.makeEntities(prog);

		OutputStub testStub = e.outputConverter({
			e.Proc("procedure"),
			e.Var("LHS"),
			e.Var("RHS"),
			e.Var("OK"),
			e.Assign(1),
			});
		assertSize(5, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_If) {
		auto prog = "procedure procedure{"
			"if(a<b)"
			"then{read a;} "
			"else {read b;}}";
		auto output = e.makeEntities(prog);

		OutputStub testStub = e.outputConverter({
			e.Proc("procedure"),
			e.Var("a"),
			e.Var("b"),
			e.If(1),
			e.Read(2, "a"),
			e.Read(3, "b"),
			});
		assertSize(6, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_While) {
		auto prog = "procedure procedure{"
			"while(a==a){read a;}"
			"}";
		auto output = e.makeEntities(prog);

		OutputStub testStub = e.outputConverter({
			e.Proc("procedure"),
			e.Var("a"),
			e.While(1),
			e.Read(2, "a"),
			});
		assertSize(4, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Nested) {
		auto prog = "procedure procedure{"
			"while(a==a){"
			"while(a==a){"
			"while(a==a){"
			"while(a==a){"
			"while(a==a){"
			"read a11;"
			"}}}}}}";
		auto output = e.makeEntities(prog);

		OutputStub testStub = e.outputConverter({
			e.Proc("procedure"),
			e.Var("a"),
			e.Var("a11"),
			e.While(1),
			e.While(2),
			e.While(3),
			e.While(4),
			e.While(5),
			e.Read(6, "a11"),
			});
		assertSize(9, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Forked_If) {
		auto prog = 
			"procedure procedure1{"
			"if(a<b)then{"
				"if(c>d)then{read d;}else{read c;}"
			"}else {"
				"if (e!=f)then {read e;} else {read f;}"
			"}}";
		auto output = e.makeEntities(prog);

		OutputStub testStub = e.outputConverter({
			e.Proc("procedure1"),
			e.Var("a"),
			e.Var("b"),
			e.Var("c"),
			e.Var("d"),
			e.Var("e"),
			e.Var("f"),
			e.If(1),
			e.If(2),
			e.Read(3, "d"),
			e.Read(4, "c"),
			e.If(5),
			e.Read(6, "e"),
			e.Read(7, "f"),
			});
		assertSize(14, output);
		Assert::IsTrue(output == testStub);
	
	
	}

	TEST_METHOD(Test_Call) {
		auto prog = "procedure name{"
						"call d;"
					"}";
		auto output = e.makeEntities(prog);

		OutputStub testStub = e.outputConverter({
			e.Proc("name"),
			e.Call(1, "d"),
			});
		assertSize(2, output);
		Assert::IsTrue(output == testStub);
		
	}

	TEST_METHOD(Test_Multiple_Procedure) {
		auto prog =
			"procedure procedure0{read a;}"
			"procedure procedure1{read b;}";
		auto output = e.makeEntities(prog);

		OutputStub testStub = e.outputConverter({
			e.Proc("procedure0"),
			e.Proc("procedure1"),
			e.Var("a"),
			e.Var("b"),
			e.Read(1, "a"),
			e.Read(2, "b"),
			});
		assertSize(6, output);
		Assert::IsTrue(output == testStub);
	}
};
}