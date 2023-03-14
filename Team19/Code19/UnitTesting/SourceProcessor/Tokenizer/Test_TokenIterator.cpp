#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SourceProcessor/Tokenizer/TokenIterator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(Test_TokenIterator) {
public:

	TEST_METHOD(Test_Eq) {
		std::vector<Token> tokens{
			NameToken("a"),
			NameToken("b"),
			NameToken("c"),
		};

		Assert::IsTrue(
			TokenIterator::cbegin(tokens)
			== TokenIterator::cbegin(tokens)
		);

		auto start = TokenIterator::cbegin(tokens);
		start.advanceTo("c");
		start.advance();
		Assert::IsTrue(
			start== TokenIterator::cend(tokens)
		);

	}
	TEST_METHOD(Test_IsValue) {
		std::vector<Token> tokens{
			NameToken("a"),
			NameToken("b"),
			IntegerToken("12"),
		};
		auto start = TokenIterator::cbegin(tokens);
		Assert::IsTrue(start.valueIs("a"));
		start.advance();
		Assert::IsTrue(start.valueIs("b"));
		start.advance();
		Assert::IsTrue(start.valueIs("12"));
		start.advance();
	}


	TEST_METHOD(Test_IsType) {
		{
			std::vector<Token> tokens{
				NameToken("a"),
				NameToken("a12"),
			};
			auto start = TokenIterator::cbegin(tokens);
			auto end = TokenIterator::cend(tokens);
			while (start != end) {
				Assert::IsTrue(start.isName());
				start.advance();
			}
		}

		{
			std::vector<Token> tokens{
			SymbolToken("||"),
			SymbolToken("&&"),
			};
			auto start = TokenIterator::cbegin(tokens);
			auto end = TokenIterator::cend(tokens);
			while (start != end) {
				Assert::IsTrue(start.isBoolean());
				start.advance();
			}
		}

		{
			std::vector<Token> tokens{
			IntegerToken("0"),
			IntegerToken("12"),
			};
			auto start = TokenIterator::cbegin(tokens);
			auto end = TokenIterator::cend(tokens);
			while (start != end) {
				Assert::IsTrue(start.isInteger());
				start.advance();
			}
		}
		
		{
			std::vector<Token> tokens{
			SymbolToken("+"),
			SymbolToken("-"),
			SymbolToken("*"),
			SymbolToken("/"),
			SymbolToken("%"),
			};
			auto start = TokenIterator::cbegin(tokens);
			auto end = TokenIterator::cend(tokens);
			while (start != end) {
				Assert::IsTrue(start.isMathOperator());
				start.advance();
			}
		}
		
		{
			std::vector<Token> tokens{
			SymbolToken(">"),
			SymbolToken(">="),
			SymbolToken("<"),
			SymbolToken("<="),
			SymbolToken("=="),
			SymbolToken("!="),
			};
			auto start = TokenIterator::cbegin(tokens);
			auto end = TokenIterator::cend(tokens);
			while (start != end) {
				Assert::IsTrue(start.isComparator());
				start.advance();
			}
		}

	}


	TEST_METHOD(Test_subVector) {
		std::vector<Token> tokens{
			NameToken("a"),
			NameToken("b"),
			IntegerToken("12"),
		};

		auto start = TokenIterator::cbegin(tokens);
		start.advance();
		auto end = TokenIterator::cend(tokens);
		end.back();

		auto actual = TokenIterator::copySubVector(start, end);
		std::vector<Token> expected{ NameToken("b"), };

		Assert::IsTrue(actual == expected);

	}



	};
}