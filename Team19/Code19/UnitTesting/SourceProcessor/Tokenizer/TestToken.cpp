#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SourceProcessor/Tokenizer/Token.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(Test_Token) {
public:

	TEST_METHOD(Test_Eq) {
		Assert::IsTrue(SymbolToken("{") == SymbolToken("{"));
		Assert::IsTrue(SymbolToken("{") != SymbolToken("}"));
		Assert::IsFalse(NameToken("{") == SymbolToken("{"));
	}

	TEST_METHOD(Test_Eq_Base) {
		Token a = SymbolToken("{");
		Token b = SymbolToken("{");
		Assert::IsTrue(a == b);
		Assert::IsTrue(a.isSymbolToken());

		b = SymbolToken("}");
		Assert::IsTrue(a != b);
		Assert::IsFalse(a == b);

		b = NameToken("{");
		Assert::IsFalse(a == b);
		Assert::IsTrue(a != b);
	}

	TEST_METHOD(Test_boolVals_base) {
		Token t = NameToken("{");
		Assert::IsTrue(t.isNameToken());
		Assert::IsFalse(t.isIntegerToken());
		Assert::IsFalse(t.isSymbolToken());

		t = IntegerToken("{");
		Assert::IsFalse(t.isNameToken());
		Assert::IsTrue(t.isIntegerToken());
		Assert::IsFalse(t.isSymbolToken());

		t = SymbolToken("{");
		Assert::IsFalse(t.isNameToken());
		Assert::IsFalse(t.isIntegerToken());
		Assert::IsTrue(t.isSymbolToken());

	}




	};
}