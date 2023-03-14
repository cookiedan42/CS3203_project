#include "stdafx.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include <unordered_set>

#include "../source/SourceProcessor/DesignExtractor/PatternExtractor/PatternExtractor.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/Pattern/Pattern.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using PatternStub = std::string;
using OutputStub = std::unordered_set<PatternStub>;

struct PatternTestHelper {
	shared_ptr<InsertPatternRequest> WhilePattern(int stmtNo, vector<Ent::Variable> controlVariables) {
		return make_shared<InsertWhilePatternRequest>(Ptn::WhilePtn(stmtNo, controlVariables));
	}

	shared_ptr<InsertPatternRequest> IfPattern(int stmtNo, vector<Ent::Variable> controlVariables) {
		return make_shared<InsertIfPatternRequest>(Ptn::IfPtn(stmtNo, controlVariables));
	}

	shared_ptr<InsertPatternRequest> AssignPattern(int stmtNo, std::string LHS, std::string RHS) {
		auto tokens = TokenSplitter(RHS).tokenize();
		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto expression = parser.parse();
		string preOrder = DesignExtractorUtil::getPreOrderTraversal(expression);
		string inOrder = DesignExtractorUtil::getInOrderTraversal(expression);

		return make_shared<InsertAssignPatternRequest>(Ptn::AssignPtn(stmtNo, LHS, preOrder, inOrder));
	}

	PatternStub makeStub(Ptn::WhilePtn p) {
		string toString = p.getType() +
			" " + p.getStmtNo();
		for (const auto& controlVariable : p.getVarSet()) {
			toString += " " + controlVariable;
		}
		return toString;
	}

	PatternStub makeStub(Ptn::IfPtn p) {
		string toString = p.getType() +
			" " + p.getStmtNo();
		for (const auto& controlVariable : p.getVarSet()) {
			toString += " " + controlVariable;
		}
		return toString;
	}

	PatternStub makeStub(Ptn::AssignPtn p) {
		return p.getType()
			+ " " + p.getStmtNo()
			+ " " + p.getLhs()
			+ " " + p.getPreOrderRHS()
			+ " " + p.getInOrderRHS();
	}

	PatternStub makeStub(shared_ptr<InsertPatternRequest> p) {
		auto casted = dynamic_pointer_cast<InsertAssignPatternRequest>(p);
		if (casted) return makeStub(casted->getAssignPattern());

		auto casted1 = dynamic_pointer_cast<InsertIfPatternRequest>(p);
		if (casted1) return makeStub(casted1->getIfPattern());

		auto casted2 = dynamic_pointer_cast<InsertWhilePatternRequest>(p);
		return makeStub(casted2->getWhilePattern());
	}

	OutputStub outputConverter(std::vector<shared_ptr<InsertPatternRequest>> input) {
		OutputStub output{};
		for (const auto& pattern : input) {
			output.insert(makeStub(pattern));
		}
		return output;
	}

	OutputStub makePatterns(std::string prog) {
		auto tokens = TokenSplitter(prog).tokenize();
		auto ast = Parser(tokens).parseTokens();
		auto output = PatternExtractor().extractPattern(ast);
		return outputConverter(output);
	}

};
auto p = PatternTestHelper();

template<typename T>
void assertSize(size_t size, T to_test) {
	Assert::AreEqual(size, to_test.size());
}

namespace UnitTesting {
	TEST_CLASS(TestDE_Pattern) {
public:
	TEST_METHOD(Test_WhilePattern) {
		auto prog = "procedure name{"
						"while (a == b) {"
							"print x;"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.WhilePattern(1, { Ent::Variable("a"), Ent::Variable("b") })
			});
		assertSize(1, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_WhilePattern_NoVariable) {
		auto prog = "procedure name{"
						"while (1 < 2) {"
							"print x;"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.WhilePattern(1, { })
			});
		assertSize(1, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_WhilePattern_ComplicatedCondition) {
		auto prog = "procedure name{"
						"while ((a + b <= (c / 2 % 3)) && (!((d + e) * f != g + (7 - 8)))) {"
							"print x;"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		vector<Ent::Variable> controlVariables = {
			Ent::Variable("a"),
			Ent::Variable("b"),
			Ent::Variable("c"),
			Ent::Variable("d"),
			Ent::Variable("e"),
			Ent::Variable("f"),
			Ent::Variable("g"),
		};
		OutputStub expected = p.outputConverter({
			p.WhilePattern(1, controlVariables)
			});
		assertSize(1, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_WhilePattern_Multiple) {
		auto prog = "procedure name{"
						"while (a >= b) {"
							"print x;"
						"}"
						"while (c == 3) {"
							"read y;"
						"}"
						"while (d < e) {"
							"read z;"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.WhilePattern(1, { Ent::Variable("a"), Ent::Variable("b") }),
			p.WhilePattern(3, { Ent::Variable("c") }),
			p.WhilePattern(5, { Ent::Variable("d"), Ent::Variable("e") }),
			});
		assertSize(3, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_WhilePattern_Nested) {
		auto prog = "procedure name{"
						"while (a >= b) {"
							"while (c == 3) {"
								"while (d < e) {"
									"print x;"
								"}"
							"}"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.WhilePattern(1, { Ent::Variable("a"), Ent::Variable("b") }),
			p.WhilePattern(2, { Ent::Variable("c") }),
			p.WhilePattern(3, { Ent::Variable("d"), Ent::Variable("e") }),
			});
		assertSize(3, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_IfPattern) {
		auto prog = "procedure name{"
						"if (a == b) then {"
							"print x;"
						"} else {"
							"read y;"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.IfPattern(1, { Ent::Variable("a"), Ent::Variable("b") })
			});
		assertSize(1, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_IfPattern_NoVariable) {
		auto prog = "procedure name{"
						"if (1 > 2) then {"
							"print x;"
						"} else {"
							"read y;"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.IfPattern(1, { })
			});
		assertSize(1, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_IfPattern_ComplicatedCondition) {
		auto prog = "procedure name{"
						"if ((a + b <= (c / 2 % 3)) && (!((d + e) * f != g + (7 - 8)))) then {"
							"print x;"
						"} else {"
							"read y;"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		vector<Ent::Variable> controlVariables = {
			Ent::Variable("a"),
			Ent::Variable("b"),
			Ent::Variable("c"),
			Ent::Variable("d"),
			Ent::Variable("e"),
			Ent::Variable("f"),
			Ent::Variable("g"),
		};
		OutputStub expected = p.outputConverter({
			p.IfPattern(1, controlVariables)
			});
		assertSize(1, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_IfPattern_Multiple) {
		auto prog = "procedure name{"
						"if (a == b) then {"
							"print x;"
						"} else {"
							"read y;"
						"}"
						"if (c != 3) then {"
							"print x;"
						"} else {"
							"read y;"
						"}"
						"if (d < e) then {"
							"print x;"
						"} else {"
							"read y;"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.IfPattern(1, { Ent::Variable("a"), Ent::Variable("b") }),
			p.IfPattern(4, { Ent::Variable("c") }),
			p.IfPattern(7, { Ent::Variable("d"), Ent::Variable("e") }),
			});
		assertSize(3, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_IfPattern_Nested) {
		auto prog = "procedure name{"
						"if (a == b) then {"
							"if (c != 3) then {"
								"if (d < e) then {"
									"print x;"
								"} else {"
									"read y;"
								"}"
							"} else {"
								"print z;"
							"}"
						"} else {"
							"read w;"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.IfPattern(1, { Ent::Variable("a"), Ent::Variable("b") }),
			p.IfPattern(2, { Ent::Variable("c") }),
			p.IfPattern(3, { Ent::Variable("d"), Ent::Variable("e") }),
			});
		assertSize(3, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_Nested_While_If) {
		auto prog = "procedure name{"
						"while (a == b) {"
							"if (c != 3) then {"
								"print x;"
							"} else {"
								"read y;"
							"}"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.WhilePattern(1, { Ent::Variable("a"), Ent::Variable("b") }),
			p.IfPattern(2, { Ent::Variable("c") }),
			});
		assertSize(2, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_Nested_If_While) {
		auto prog = "procedure name{"
						"if (a == b) then {"
							"while (c != 3) {"
								"print x;"
							"}"
						"} else {"
							"while (d >= e) {"
								"read y;"
							"}"
						"}"
					"}";
		auto actual = p.makePatterns(prog);

		OutputStub expected = p.outputConverter({
			p.IfPattern(1, { Ent::Variable("a"), Ent::Variable("b") }),
			p.WhilePattern(2, { Ent::Variable("c") }),
			p.WhilePattern(4, { Ent::Variable("d"), Ent::Variable("e")}),
			});
		assertSize(3, expected);
		Assert::IsTrue(expected == actual);
	}

	TEST_METHOD(Test_AssignPattern_Variable) {
		auto prog = "procedure name{"
						"x = x+y*z-(u+v);"
					"}";
		auto output = p.makePatterns(prog);

		OutputStub testStub = p.outputConverter({
			p.AssignPattern(1, "x", "x+y*z-(u+v)")
			});
		assertSize(1, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_AssignPattern_Constant) {
		auto prog = "procedure procedure{"
						"x = 1+2-3*4/5-(6+7);"
					"}";
		auto output = p.makePatterns(prog);

		OutputStub testStub = p.outputConverter({
			p.AssignPattern(1, "x", "1+2-3*4/5-(6+7)")
			});
		assertSize(1, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_Multiple_AssignPatterns) {
		auto prog = "procedure procedure{"
						"x = y+z+1;"
						"y = x*z*1;"
						"z = x/y/1;"
						"w = w-x-y-z-1;"
					"}";
		auto output = p.makePatterns(prog);

		OutputStub testStub = p.outputConverter({
			p.AssignPattern(1, "x", "y+z+1"),
			p.AssignPattern(2, "y", "x*z*1"),
			p.AssignPattern(3, "z", "x/y/1"),
			p.AssignPattern(4, "w", "w-x-y-z-1")
			});
		assertSize(4, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_NestedWhile_AssignPattern) {
		auto prog = "procedure procedure{"
						"x = x+x*x-x/x;"
						"while(a==a){y = y*y/x;}"
					"}";
		auto output = p.makePatterns(prog);

		OutputStub testStub = p.outputConverter({
			p.AssignPattern(1, "x", "x+x*x-x/x"),
			p.WhilePattern(2, { Ent::Variable("a") }),
			p.AssignPattern(3, "y", "y*y/x")
			});
		assertSize(3, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_NestedIf_AssignPattern) {
		auto prog = "procedure procedure{"
						"a = 1+2-3*4/5;"
						"if (b==b) then {"
							"b = i*j*k;"
						"} else {"
							"c = 999/333;"
							"d = 1*2*3*4*5*6*7*8*9*10;"
						"}"
					"}";
		auto output = p.makePatterns(prog);

		OutputStub testStub = p.outputConverter({
			p.AssignPattern(1, "a", "1+2-3*4/5"),
			p.IfPattern(2, { Ent::Variable("b") }),
			p.AssignPattern(3, "b", "i*j*k"),
			p.AssignPattern(4, "c", "999/333"),
			p.AssignPattern(5, "d", "1*2*3*4*5*6*7*8*9*10")
			});
		assertSize(5, output);
		Assert::IsTrue(output == testStub);
	}

	TEST_METHOD(Test_AllPatterns) {
		auto prog = "procedure procedure{"
						"while (a > b) {"
							"if (c < d) then {"
								"x = x+y/z;"
							"} else {"
								"s = u-v*t;"
							"}"
						"}"
					"}";
		auto output = p.makePatterns(prog);

		OutputStub testStub = p.outputConverter({
			p.WhilePattern(1, { Ent::Variable("a"), Ent::Variable("b") }),
			p.IfPattern(2, { Ent::Variable("c"), Ent::Variable("d") }),
			p.AssignPattern(3, "x", "x+y/z"),
			p.AssignPattern(4, "s", "u-v*t")
			});
		assertSize(4, output);
		Assert::IsTrue(output == testStub);
	}
	};
}