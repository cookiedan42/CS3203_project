#include "stdafx.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include "../source/SourceProcessor/Validator/CondValidator.h"
#include "../source/SourceProcessor/Tokenizer/TokenIterator.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
TEST_CLASS(Test_SP_CondValidator) {
public:

	TEST_METHOD(Test_BasicBoolean_valid) {
		std::vector<std::string> valid_conds = {
			"!(a<b)",
			"(a<b) || (a>b)",
			"(a<b) && (a>b)",
		};
		for each (auto to_test in valid_conds) {
			auto t = TokenSplitter(to_test).tokenize();
			CondValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validate();
		}
	}

	TEST_METHOD(Test_BasicBoolean_invalid) {
		std::vector<std::string> invalid_conds = {
			"a","1",
			"!a<b",
			"a<b || (a>b)", "(a<b) || a>b",
			"a<b && (a>b)", "(a<b) && a>b",
			"a<b || a>b","a<b && a>b",
		};
		for each (auto to_test in invalid_conds) {
			Assert::ExpectException<validation_error>([to_test]() {
				auto t = TokenSplitter(to_test).tokenize();
				CondValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validate();
				});
		}
	}

	TEST_METHOD(Test_NestedBoolean_valid) {
		std::vector<std::string> valid_conds = {
			"((a<b) && (a>b)) || (a>b)",
			"(a>b) || ((a<b) && (a>b))",
			"(a>b) || (!(a>b))",
			" (!(a>b)) || (a>b)",
			"!((a<b) && (a>b))",
		};
		for each (auto to_test in valid_conds) {
			auto t = TokenSplitter(to_test).tokenize();
			CondValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validate();
		}
	}

	TEST_METHOD(Test_NestedBoolean_invalid) {
		std::vector<std::string> invalid_conds = {
			"(a>b) && (a>b) && (a>b)",
			"!(a>b) && (a>b)"
		};
		for each (auto to_test in invalid_conds) {
			Assert::ExpectException<validation_error>([to_test]() {
				auto t = TokenSplitter(to_test).tokenize();
				CondValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validate();
				});
		}
	}

	TEST_METHOD(Test_BasicComparison_valid) {
		std::vector<std::string> valid_rels = {
			"a<b" ,"a<1" ,"1<b" ,"1<2" ,
			"a<=b","a<=1","1<=b","1<=2",
			"a>b" ,"a>1" ,"1>b" ,"1>2" ,
			"a>=b","a>=1","1>=b","1>=2",
			"a==b","a==1","1==b","1==2",
			"a!=b","a!=1","1!=b","1!=2",
		};
		for each (auto to_test in valid_rels) {
			auto t = TokenSplitter(to_test).tokenize();
			CondValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validate();
		}
	}

	TEST_METHOD(Test_OperatorsComparison_valid) {
		std::vector<std::string> valid_rels = {
			"a     >  b+2",
			"a-1   >= b",
			"a*1   <  b/2",
			"(a+1) <= b",
			"a     == (1-b)",
			"(a%1) != (b%1)",
			"1+(1+(1+(a)+1)+1)+1 == 1*(1*(b)-1)",
		};
		for each (auto to_test in valid_rels) {
			auto t = TokenSplitter(to_test).tokenize();
			CondValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validate();		}
	}

	TEST_METHOD(Test_Operators_Compariosn_invalid) {
		std::vector<std::string> invalid_rels = {
			"",

			"a<","a<=","a>","a>=","a==","a!=",
			"1<","1<=","1>","1>=","1==","1!=",
			"<a","<=a",">a",">=a","==a","!=a",
			"<1","<=1",">1",">=1","==1","!=1",
			
			"(a==b)","(1==b)","(a==1)","(1==1)",
			"((a==b))","((1==b))","((a==1))","((1==1))",
		};
		for each (auto to_test in invalid_rels) {
			Assert::ExpectException<validation_error>([to_test]() {
				auto t = TokenSplitter(to_test).tokenize();
				CondValidator(TokenIterator::cbegin(t), TokenIterator::cend(t)).validate();
				});
		}
	}



};
}