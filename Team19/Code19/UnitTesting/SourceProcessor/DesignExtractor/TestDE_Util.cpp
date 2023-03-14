#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/DesignExtractor/DesignExtractorUtil.h"

using namespace std;
using namespace SP;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestDE_Util) {
public:
	TEST_METHOD(TestPreOrderSimple) {
		// x - y + z
		// PreOrder: +-xyz
		string expected = "(+(-(x)(y))(z))";

		auto dummy = make_unique<VariableNode>("");
		auto x = make_unique<VariableNode>("x");
		auto y = make_unique<VariableNode>("y");
		auto minus = make_unique<BinaryOperatorNode>("-", move(x), move(y));
		auto z = make_unique<VariableNode>("z");
		auto expression = make_unique<BinaryOperatorNode>("+", move(minus), move(z));
		auto assign = make_unique<AssignStatementNode>(0, move(dummy), move(expression));

		auto actual = DesignExtractorUtil::getPreOrderTraversal(assign->getRHS());

		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(TestPreOrderComplicated) {
		// (a + b) * c / (d - e)
		// PreOrder: /*+abc-de
		string expected = "(/(*(+(a)(b))(c))(-(d)(e)))";

		auto dummy = make_unique<VariableNode>("");
		auto a = make_unique<VariableNode>("a");
		auto b = make_unique<VariableNode>("b");
		auto plus = make_unique<BinaryOperatorNode>("+", move(a), move(b));
		auto c = make_unique<VariableNode>("c");
		auto multiply = make_unique<BinaryOperatorNode>("*", move(plus), move(c));
		auto d = make_unique<VariableNode>("d");
		auto e = make_unique<VariableNode>("e");
		auto minus = make_unique<BinaryOperatorNode>("-", move(d), move(e));
		auto expression = make_unique<BinaryOperatorNode>("/", move(multiply), move(minus));
		auto assign = make_unique<AssignStatementNode>(0, move(dummy), move(expression));

		auto actual = DesignExtractorUtil::getPreOrderTraversal(assign->getRHS());

		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(TestInOrderSimple) {
		// x - y + z
		string expected = "(((x)-(y))+(z))";

		auto dummy = make_unique<VariableNode>("");
		auto x = make_unique<VariableNode>("x");
		auto y = make_unique<VariableNode>("y");
		auto minus = make_unique<BinaryOperatorNode>("-", move(x), move(y));
		auto z = make_unique<VariableNode>("z");
		auto expression = make_unique<BinaryOperatorNode>("+", move(minus), move(z));
		auto assign = make_unique<AssignStatementNode>(0, move(dummy), move(expression));

		auto actual = DesignExtractorUtil::getInOrderTraversal(assign->getRHS());

		Assert::AreEqual(expected, actual);
	}

	TEST_METHOD(TestInOrderComplicated) {
		// (a + b) * c / (d - e)
		string expected = "((((a)+(b))*(c))/((d)-(e)))";

		auto dummy = make_unique<VariableNode>("");
		auto a = make_unique<VariableNode>("a");
		auto b = make_unique<VariableNode>("b");
		auto plus = make_unique<BinaryOperatorNode>("+", move(a), move(b));
		auto c = make_unique<VariableNode>("c");
		auto multiply = make_unique<BinaryOperatorNode>("*", move(plus), move(c));
		auto d = make_unique<VariableNode>("d");
		auto e = make_unique<VariableNode>("e");
		auto minus = make_unique<BinaryOperatorNode>("-", move(d), move(e));
		auto expression = make_unique<BinaryOperatorNode>("/", move(multiply), move(minus));
		auto assign = make_unique<AssignStatementNode>(0, move(dummy), move(expression));

		auto actual = DesignExtractorUtil::getInOrderTraversal(assign->getRHS());

		Assert::AreEqual(expected, actual);
	}
	};
}