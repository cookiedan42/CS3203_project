#include "stdafx.h"
#include "CppUnitTest.h"
#include <iostream>
#include "../source/SourceProcessor/Parser/Operator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std;

namespace UnitTesting {
	TEST_CLASS(TestOperator) {
public:
	TEST_METHOD(Test_Getter) {
		PlusOperator plus;
		Assert::IsTrue(plus.getSymbol() == OperatorSymbol::PLUS);

		MinusOperator minus;
		Assert::IsTrue(minus.getSymbol() == OperatorSymbol::MINUS);

		MultiplyOperator multiply;
		Assert::IsTrue(multiply.getSymbol() == OperatorSymbol::MULTIPLY);

		DivideOperator divide;
		Assert::IsTrue(divide.getSymbol() == OperatorSymbol::DIVIDE);

		ModuloOperator modulo;
		Assert::IsTrue(modulo.getSymbol() == OperatorSymbol::MODULO);
	}

	TEST_METHOD(Test_CompareSamePrecedence) {
		PlusOperator plus;
		MinusOperator minus;

		Assert::AreEqual(plus.comparePrecedence(plus), 0);
		Assert::AreEqual(minus.comparePrecedence(minus), 0);
		Assert::AreEqual(plus.comparePrecedence(minus), 0);

		MultiplyOperator multiply;
		DivideOperator divide;
		ModuloOperator modulo;

		Assert::AreEqual(multiply.comparePrecedence(multiply), 0);
		Assert::AreEqual(divide.comparePrecedence(divide), 0);
		Assert::AreEqual(modulo.comparePrecedence(modulo), 0);
		Assert::AreEqual(multiply.comparePrecedence(divide), 0);
		Assert::AreEqual(multiply.comparePrecedence(modulo), 0);
		Assert::AreEqual(divide.comparePrecedence(modulo), 0);
	}

	TEST_METHOD(Test_CompareDifferentPrecedence) {
		PlusOperator plus;
		MinusOperator minus;
		MultiplyOperator multiply;
		DivideOperator divide;
		ModuloOperator modulo;

		Assert::AreEqual(plus.comparePrecedence(multiply), -1);
		Assert::AreEqual(plus.comparePrecedence(divide), -1);
		Assert::AreEqual(plus.comparePrecedence(modulo), -1);

		Assert::AreEqual(minus.comparePrecedence(multiply), -1);
		Assert::AreEqual(minus.comparePrecedence(divide), -1);
		Assert::AreEqual(minus.comparePrecedence(modulo), -1);

		Assert::AreEqual(multiply.comparePrecedence(plus), 1);
		Assert::AreEqual(multiply.comparePrecedence(minus), 1);

		Assert::AreEqual(divide.comparePrecedence(plus), 1);
		Assert::AreEqual(divide.comparePrecedence(minus), 1);

		Assert::AreEqual(modulo.comparePrecedence(plus), 1);
		Assert::AreEqual(modulo.comparePrecedence(minus), 1);
	}
	};
}