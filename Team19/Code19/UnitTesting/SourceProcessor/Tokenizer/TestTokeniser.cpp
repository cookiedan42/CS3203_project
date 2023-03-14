#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template<typename T>
void assertSize(size_t size, T to_test) {
	Assert::AreEqual(size, to_test.size());
}

namespace UnitTesting
{		
	TEST_CLASS(TestTokeniser)
	{
	public:

		TEST_METHOD(Test_Tokenise_words)
		{
			std::vector<Token> actual;
			std::vector<Token> expected;

			actual = TokenSplitter("abc def").tokenize();
			assertSize(2,actual);

			actual = TokenSplitter("abc d12").tokenize();
			expected = { NameToken("abc"),NameToken("d12") };
			Assert::IsTrue(actual == expected);
			assertSize(2, actual);
		}

		TEST_METHOD(Test_Tokenise_number) {
			std::vector<Token> actual;
			std::vector<Token> expected;

			actual = TokenSplitter("1111111111111111 0999999").tokenize();
			assertSize(2, actual);
			
			// tokenizer does not validate integer
			expected = { IntegerToken("1111111111111111"),IntegerToken("0999999") };
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(Test_Tokenise_bracket) {
			std::vector<Token> actual;

			actual = TokenSplitter("(){}").tokenize();
			assertSize(4, actual);
		}

		TEST_METHOD(Test_Tokenise_symbols) {
			std::vector<Token> actual;

			//illegal but still needs to be tokenized before validation
			actual = TokenSplitter("===").tokenize();
			assertSize(2, actual);
		}

		TEST_METHOD(Test_Tokenize_bracket_Exact)
		{
			auto actual = TokenSplitter("abc (def)").tokenize();
			assertSize(4, actual);
			
			std::vector <Token> expected { 
				NameToken{ "abc" }, 
				SymbolToken{ "(" }, 
				NameToken{ "def" }, 
				SymbolToken{ ")" } 
			};
			for (size_t i = 0; i < expected.size(); i++)
			{
				std::vector<Token>::iterator aa = expected.begin();
				std::vector<Token>::iterator bb = actual.begin();
				Assert::IsTrue(*aa==*bb);
				aa++;
				bb++;
			}
		}
	
		TEST_METHOD(Test_Tokenise_boolean)
		{
			std::vector<Token> actual;

			actual = TokenSplitter("abc&&def").tokenize();
			assertSize(3, actual);
			actual = TokenSplitter("abc||def").tokenize();
			assertSize(3, actual);
			
			actual = TokenSplitter("!(abc)").tokenize();
			assertSize(4, actual);
			actual = TokenSplitter("!abc").tokenize();
			assertSize(2, actual);
		}

		TEST_METHOD(Test_Tokenise_compare)
		{
			std::vector<Token> actual;
			actual = TokenSplitter("abc<def").tokenize();
			assertSize(3, actual);
			actual = TokenSplitter("abc<=def").tokenize();
			assertSize(3, actual);
			actual = TokenSplitter("abc>def").tokenize();
			assertSize(3, actual);
			actual = TokenSplitter("abc>=def").tokenize();
			assertSize(3, actual);
			actual = TokenSplitter("abc==def").tokenize();
			assertSize(3, actual);
			actual = TokenSplitter("abc!=def").tokenize();
			assertSize(3, actual);
		}

		TEST_METHOD(Test_Tokenise_terminal_comp)
		{
			std::vector<Token> actual;
			actual = TokenSplitter("abc<").tokenize();
			assertSize(2,actual);
			actual = TokenSplitter("abc>").tokenize();
			assertSize(2, actual);
			actual = TokenSplitter("abc=").tokenize();
			assertSize(2,actual);
			actual = TokenSplitter("abc&").tokenize();
			assertSize(2, actual);
			actual = TokenSplitter("abc!").tokenize();
			assertSize(2, actual);
		}

		TEST_METHOD(Test_Tokenise_differentObject)
		{
			// trying to ensure that multiple calls to tokenize will give a new vector object

			TokenSplitter ts = TokenSplitter("abc def");
			Assert::IsTrue(
				ts.tokenize().front()==
				ts.tokenize().front()
			);
			Assert::IsFalse(
				(&(ts.tokenize().front())) ==
				(&(ts.tokenize().front()))
			);
		}

		TEST_METHOD(Test_NewLine) {
			std::vector<Token> actual;
			actual = TokenSplitter("read var;").tokenize();
			assertSize(3,actual);
			actual = TokenSplitter("read var;\nread var;").tokenize();
			assertSize(6,actual);
			actual = TokenSplitter("read var;\n\nread var;").tokenize();
			assertSize(6, actual);
			actual = TokenSplitter("read\nvar;\n\nread\nvar;").tokenize();
			assertSize(6, actual);
		}

		TEST_METHOD(Test_Spaces) {
			std::vector<Token> actual;
			actual = TokenSplitter("read var;").tokenize();
			assertSize(3, actual);
			actual = TokenSplitter("read var;     read var;").tokenize();
			assertSize(6, actual);
			actual = TokenSplitter("read      var;    read var;").tokenize();
			assertSize(6, actual);
			actual = TokenSplitter("     read   var;read var      ;      ").tokenize();
			assertSize(6, actual);
		}

		TEST_METHOD(Test_Tabs) {
			std::vector<Token> actual;
			actual = TokenSplitter("read var;").tokenize();
			assertSize(3, actual);
			actual = TokenSplitter("	read	var	;	read	var	;	").tokenize();
			assertSize(6, actual);
			actual = TokenSplitter("read      var;    read var;").tokenize();
			assertSize(6, actual);
			actual = TokenSplitter("\tread\tvar\t\t;\t\tread\t\tvar\t;\t").tokenize();
			assertSize(6, actual);
		}

		TEST_METHOD(Test_Whitespace) {
			std::vector<Token> actual;
			actual = TokenSplitter("read\n\tvar;").tokenize();
			assertSize(3, actual);
		}	

	};
}