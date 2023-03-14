#include "stdafx.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include "../source/SourceProcessor/Validator/ExpValidator.h"
#include "../source/SourceProcessor/Tokenizer/TokenIterator.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {

/*
Expressions are math statements
RHS of assign or one side of comparison
*/
TEST_CLASS(Test_SP_ExpValidator) {
public:
	TEST_METHOD(Test_BasicFactor_Valid) {
		std::vector<std::string> valid_exp = {
		"1","123",
		"a","A","abc",
		"a12","A12",
		"(a)","(A)","(1)",
		"((((1))))",
		};
	for each (auto expr in valid_exp) {
			auto t = TokenSplitter(expr).tokenize();
			ExpValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validateExpression();
		}
	}

	TEST_METHOD(Test_BasicFactor_Invalid) {
		std::vector<std::string> invalid_exp = {
		"1a","1A",
		"a a","1 a","a 1","1 1",
		"-1","01","0123",
		"(a","a)","(1","1)",
		"{a","a}","{1","1}",
		"a=a","a=1","1=1",
		};
		for each (auto expr in invalid_exp) {
			Assert::ExpectException<validation_error>([expr]() {
				auto t = TokenSplitter(expr).tokenize();
				ExpValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validateExpression();
			});
		}
	}

	TEST_METHOD(Test_SingleOperatorExpressions_valid) {
		std::vector<std::string> valid_exp = {
			"a+b","a-b","a*b","a/b","a%b",
			"1+b","1-b","1*b","1/b","1%b",
			"a+1","a-1","a*1","a/1","a%1",
			"1+1","1-1","1*1","1/1","1%1",

			"(a)+b","a-(b)",
			"(1)+b","1-(b)",
			"(a)+1","a-(1)",
			"(1)+1","1-(1)", 
		};
		for each (auto expr in valid_exp) {
			auto t = TokenSplitter(expr).tokenize();
			ExpValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validateExpression();
		}
	}

	TEST_METHOD(Test_SingleOperatorExpressions_invalid) {
		std::vector<std::string> invalid_exp = {
			"a<a","a<=a","a>a","a>=a","a==a","a!=a",
			"a++a","a--a","a*=a",
		};
		for each (auto expr in invalid_exp) {
			Assert::ExpectException<validation_error>([expr]() {
				auto t = TokenSplitter(expr).tokenize();
				ExpValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validateExpression();
				});
		}
	}

	TEST_METHOD(Test_MultiOperatorExpressions_valid) {
		std::vector<std::string> valid_exp = {
			"a+b-c","d*e/f", "g%h%i",
			"a+(b+c)","(1+c)+3",
			"(a+(b)+c)","(1+c)+3",

			"1+28-a*f/A%d12",
			"(1+28)-a*f/A%d12",
			"1+28-(a*f/A)%d12",
			"1+28-a*(f/A)%d12",
			"(1+28)-a*(f/A)%d12",
		};
		for each (auto expr in valid_exp) {
			auto t = TokenSplitter(expr).tokenize();
			ExpValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validateExpression();
		}
	
	}

};
}