#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/Parser/Operator.h"
#include "../source/SourceProcessor/Parser/ExpressionParser.h"
#include "../source/SourceProcessor/Parser/ParseException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std;

namespace UnitTesting {
	TEST_CLASS(TestExpresionParser) {
public:
	TEST_METHOD(TestParseVariable) {
		auto expected = make_unique<VariableNode>("variable");

		vector<Token> tokens;
		tokens.push_back(NameToken("variable"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto actual = parser.parse();

		Assert::IsTrue(visit([&](const auto& expression) {
			return *expression == *expected;
			}, actual));
	}

	TEST_METHOD(TestParseConstant) {
		auto expected = make_unique<ConstantNode>("0");

		vector<Token> tokens;
		tokens.push_back(IntegerToken("0"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto actual = parser.parse();

		Assert::IsTrue(visit([&](const auto& expression) {
			return *expression == *expected;
			}, actual));
	}

	TEST_METHOD(Test_ParseSamePrecedence) {
		// x + y
		auto x = make_unique<VariableNode>("x");
		auto y = make_unique<VariableNode>("y");
		auto expected = make_unique<BinaryOperatorNode>("+", move(x), move(y));

		vector<Token> tokens;
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(NameToken("y"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto actual = parser.parse();

		Assert::IsTrue(visit([&](const auto& expression) {
			return *expression == *expected;
			}, actual));
	}

	TEST_METHOD(Test_ParseLeftAssociative) {
		// x * y / 3
		auto x = make_unique<VariableNode>("x");
		auto y = make_unique<VariableNode>("y");
		auto multiply = make_unique<BinaryOperatorNode>("*", move(x), move(y));
		auto number = make_unique<ConstantNode>("3");
		auto expected = make_unique<BinaryOperatorNode>("/", move(multiply), move(number));

		vector<Token> tokens;
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken("*"));
		tokens.push_back(NameToken("y"));
		tokens.push_back(SymbolToken("/"));
		tokens.push_back(IntegerToken("3"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto actual = parser.parse();

		Assert::IsTrue(visit([&](const auto& expression) {
			return *expression == *expected;
			}, actual));
	}

	TEST_METHOD(Test_ParseDifferentPrecedence) {
		// x - y % z
		auto y = make_unique<VariableNode>("y");
		auto z = make_unique<VariableNode>("z");
		auto modulo = make_unique<BinaryOperatorNode>("%", move(y), move(z));
		auto x = make_unique<VariableNode>("x");
		auto expected = make_unique<BinaryOperatorNode>("-", move(x), move(modulo));

		vector<Token> tokens;
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken("-"));
		tokens.push_back(NameToken("y"));
		tokens.push_back(SymbolToken("%"));
		tokens.push_back(NameToken("z"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto actual = parser.parse();

		Assert::IsTrue(visit([&](const auto& expression) {
			return *expression == *expected;
			}, actual));
	}

	TEST_METHOD(Test_ParseComplicated) {
		// x - y * z + 1 % 2 - w / 3
		auto y = make_unique<VariableNode>("y");
		auto z = make_unique<VariableNode>("z");
		auto multiply = make_unique<BinaryOperatorNode>("*", move(y), move(z));
		auto x = make_unique<VariableNode>("x");
		auto minus1 = make_unique<BinaryOperatorNode>("-", move(x), move(multiply));
		auto num1 = make_unique<ConstantNode>("1");
		auto num2 = make_unique<ConstantNode>("2");
		auto modulo = make_unique<BinaryOperatorNode>("%", move(num1), move(num2));
		auto plus = make_unique<BinaryOperatorNode>("+", move(minus1), move(modulo));
		auto w = make_unique<VariableNode>("w");
		auto num3 = make_unique<ConstantNode>("3");
		auto divide = make_unique<BinaryOperatorNode>("/", move(w), move(num3));
		auto expected = make_unique<BinaryOperatorNode>("-", move(plus), move(divide));

		vector<Token> tokens;
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken("-"));
		tokens.push_back(NameToken("y"));
		tokens.push_back(SymbolToken("*"));
		tokens.push_back(NameToken("z"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(IntegerToken("1"));
		tokens.push_back(SymbolToken("%"));
		tokens.push_back(IntegerToken("2"));
		tokens.push_back(SymbolToken("-"));
		tokens.push_back(NameToken("w"));
		tokens.push_back(SymbolToken("/"));
		tokens.push_back(IntegerToken("3"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto actual = parser.parse();

		Assert::IsTrue(visit([&](const auto& expression) {
			return *expression == *expected;
			}, actual));
	}

	TEST_METHOD(Test_ParseParenthesis1) {
		// x - (y + z)
		auto y = make_unique<VariableNode>("y");
		auto z = make_unique<VariableNode>("z");
		auto plus = make_unique<BinaryOperatorNode>("+", move(y), move(z));
		auto x = make_unique<VariableNode>("x");
		auto expected = make_unique<BinaryOperatorNode>("-", move(x), move(plus));

		vector<Token> tokens;
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken("-"));
		tokens.push_back(SymbolToken("("));
		tokens.push_back(NameToken("y"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(NameToken("z"));
		tokens.push_back(SymbolToken(")"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto actual = parser.parse();

		Assert::IsTrue(visit([&](const auto& expression) {
			return *expression == *expected;
			}, actual));
	}

	TEST_METHOD(Test_ParseParenthesis2) {
		// (x + y) / z
		auto x = make_unique<VariableNode>("x");
		auto y = make_unique<VariableNode>("y");
		auto plus = make_unique<BinaryOperatorNode>("+", move(x), move(y));
		auto z = make_unique<VariableNode>("z");
		auto expected = make_unique<BinaryOperatorNode>("/", move(plus), move(z));

		vector<Token> tokens;
		tokens.push_back(SymbolToken("("));
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(NameToken("y"));
		tokens.push_back(SymbolToken(")"));
		tokens.push_back(SymbolToken("/"));
		tokens.push_back(NameToken("z"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto actual = parser.parse();

		Assert::IsTrue(visit([&](const auto& expression) {
			return *expression == *expected;
			}, actual));
	}

	TEST_METHOD(Test_ParseComplicatedParenthesis) {
		// 1 - (2 / 3) * (4 % 5) + ((w - x) * (y + z))
		auto num2 = make_unique<ConstantNode>("2");
		auto num3 = make_unique<ConstantNode>("3");
		auto divide = make_unique<BinaryOperatorNode>("/", move(num2), move(num3));
		auto num4 = make_unique<ConstantNode>("4");
		auto num5 = make_unique<ConstantNode>("5");
		auto modulo = make_unique<BinaryOperatorNode>("%", move(num4), move(num5));
		auto multiply1 = make_unique<BinaryOperatorNode>("*", move(divide), move(modulo));
		auto num1 = make_unique<ConstantNode>("1");
		auto minus1 = make_unique<BinaryOperatorNode>("-", move(num1), move(multiply1));
		auto w = make_unique<VariableNode>("w");
		auto x = make_unique<VariableNode>("x");
		auto minus2 = make_unique<BinaryOperatorNode>("-", move(w), move(x));
		auto y = make_unique<VariableNode>("y");
		auto z = make_unique<VariableNode>("z");
		auto plus1 = make_unique<BinaryOperatorNode>("+", move(y), move(z));
		auto multiply2 = make_unique<BinaryOperatorNode>("*", move(minus2), move(plus1));
		auto expected = make_unique<BinaryOperatorNode>("+", move(minus1), move(multiply2));

		vector<Token> tokens;
		tokens.push_back(IntegerToken("1"));
		tokens.push_back(SymbolToken("-"));
		tokens.push_back(SymbolToken("("));
		tokens.push_back(IntegerToken("2"));
		tokens.push_back(SymbolToken("/"));
		tokens.push_back(IntegerToken("3"));
		tokens.push_back(SymbolToken(")"));
		tokens.push_back(SymbolToken("*"));
		tokens.push_back(SymbolToken("("));
		tokens.push_back(IntegerToken("4"));
		tokens.push_back(SymbolToken("%"));
		tokens.push_back(IntegerToken("5"));
		tokens.push_back(SymbolToken(")"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(SymbolToken("("));
		tokens.push_back(SymbolToken("("));
		tokens.push_back(NameToken("w"));
		tokens.push_back(SymbolToken("-"));
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken(")"));
		tokens.push_back(SymbolToken("*"));
		tokens.push_back(SymbolToken("("));
		tokens.push_back(NameToken("y"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(NameToken("z"));
		tokens.push_back(SymbolToken(")"));
		tokens.push_back(SymbolToken(")"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);
		auto actual = parser.parse();

		Assert::IsTrue(visit([&](const auto& expression) {
			return *expression == *expected;
			}, actual));
	}

	TEST_METHOD(Test_UnmatchedParenthesis1) {
		// (
		vector<Token> tokens;
		tokens.push_back(SymbolToken("("));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);

		Assert::ExpectException<ParseException>([&]() { parser.parse(); });
	}

	TEST_METHOD(Test_UnmatchedParenthesis2) {
		// )
		vector<Token> tokens;
		tokens.push_back(SymbolToken(")"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);

		Assert::ExpectException<ParseException>([&]() { parser.parse(); });
	}

	TEST_METHOD(Test_UnmatchedParenthesis3) {
		// x + ((y+z)
		vector<Token> tokens;
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(SymbolToken("("));
		tokens.push_back(SymbolToken("("));
		tokens.push_back(NameToken("y"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(NameToken("z"));
		tokens.push_back(SymbolToken(")"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);

		Assert::ExpectException<ParseException>([&]() { parser.parse(); });
	}

	TEST_METHOD(Test_UnmatchedParenthesis4) {
		// x + (y+z))
		vector<Token> tokens;
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(SymbolToken("("));
		tokens.push_back(NameToken("y"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(NameToken("z"));
		tokens.push_back(SymbolToken(")"));
		tokens.push_back(SymbolToken(")"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);

		Assert::ExpectException<ParseException>([&]() { parser.parse(); });
	}

	TEST_METHOD(Test_UnknownSymbol) {
		// x + y < z
		vector<Token> tokens;
		tokens.push_back(NameToken("x"));
		tokens.push_back(SymbolToken("+"));
		tokens.push_back(NameToken("y"));
		tokens.push_back(SymbolToken("<"));
		tokens.push_back(NameToken("z"));

		TokenIterator start = TokenIterator::cbegin(tokens);
		TokenIterator end = TokenIterator::cend(tokens);
		ExpressionParser parser = ExpressionParser(start, end);

		Assert::ExpectException<ParseException>([&]() { parser.parse(); });
	}
	};
}