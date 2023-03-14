#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include <unordered_set>
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/DesignExtractor/RelationshipExtractor/CallsExtractor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using CallsStub = std::string;
using OutputStub = std::unordered_set<CallsStub>;

struct CallsTestHelper {
	// convinience functions for creating Calls
	Rel::Calls Calls(std::string e, std::string f) {
		return Rel::Calls(Ent::Procedure(e), Ent::Procedure(f));
	}

	CallsStub makeStub(Rel::Calls f) {
		return f.getArg1()->getName() + " " + f.getArg2()->getName();
	}

	OutputStub outputConverter(std::vector<Rel::Calls> original) {
		OutputStub res {};
		for each (auto f in original) {
			res.insert(makeStub(f));
		}
		return res;
	}

	OutputStub makeCalls(std::string inProgram) {
		auto tokens = TokenSplitter(inProgram).tokenize();
		auto AST = Parser(tokens).parseTokens();
		auto output = CallsExtractor().extractCalls(AST);
		return outputConverter(output);
	}

};
auto c = CallsTestHelper();

template<typename T>
void assertSize(size_t size, T to_test) {
	Assert::AreEqual(size, to_test.size());
}


namespace UnitTesting {
	TEST_CLASS(TestDE_Calls) {
public:

	TEST_METHOD(Test_Calls_basic) {
		auto prog =
			"procedure proc1 {call proc2;}"
			"procedure proc2 {read a;}";

		auto actual = c.makeCalls(prog);
		OutputStub expected = c.outputConverter({
			c.Calls("proc1","proc2"),
			});
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_Calls_nest_while) {
		auto prog =
			"procedure proc1 {while(a==a){call proc2;}}"
			"procedure proc2 {read a;}";

		auto actual = c.makeCalls(prog);
		OutputStub expected = c.outputConverter({
			c.Calls("proc1","proc2"),
			});
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_Calls_nest_if) {
		auto prog =
			"procedure proc1 {if(a==a)"
			"	then{call proc2;}"
			"	else{call proc3;}}"
			"procedure proc2 {read a;}"
			"procedure proc3 {read a;}";

		auto actual = c.makeCalls(prog);
		OutputStub expected = c.outputConverter({
			c.Calls("proc1","proc2"),
			c.Calls("proc1","proc3"),
			});
		Assert::IsTrue(actual == expected);
	}

	TEST_METHOD(Test_Calls_multi) {
		auto prog =
			"procedure proc1 {call proc2; call proc3;}"
			"procedure proc2 {call proc3;}"
			"procedure proc3 {call proc4;}"
			"procedure proc4 {read a;}";

		auto actual = c.makeCalls(prog);
		OutputStub expected = c.outputConverter({
			c.Calls("proc1","proc2"),
			c.Calls("proc1","proc3"),
			c.Calls("proc2","proc3"),
			c.Calls("proc3","proc4"),
			});
		Assert::IsTrue(actual == expected);
	}

};
}
