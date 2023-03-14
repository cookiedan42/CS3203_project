#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/SourceProcessor/AST/AST.h"

using namespace SP;
using namespace std;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestAST) {
public:
	TEST_METHOD(Test_ConstantNode) {
		auto c1 = make_unique<ConstantNode>("1");
		auto c2 = make_unique<ConstantNode>("1");
		auto c3 = make_unique<ConstantNode>("2");

		Assert::IsTrue(*c1 == *c2);
		Assert::IsFalse(*c1 == *c3);
	}

	TEST_METHOD(Test_VariableNode) {
		auto x1 = make_unique<VariableNode>("x");
		auto x2 = make_unique<VariableNode>("x");
		auto y = make_unique<VariableNode>("y");

		Assert::IsTrue(*x1 == *x2);
		Assert::IsFalse(*x1 == *y);
	}

	TEST_METHOD(Test_BinaryOperatorNode1) {
		// x+y
		auto x1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto plus1 = make_unique<BinaryOperatorNode>("+", move(x1), move(y1));

		auto x2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto plus2 = make_unique<BinaryOperatorNode>("+", move(x2), move(y2));

		// wrong operator
		auto x3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto wrongOperator = make_unique<BinaryOperatorNode>("-", move(x3), move(y3));

		// wrong LHS
		auto z = make_unique<VariableNode>("z");
		auto y4 = make_unique<VariableNode>("y");
		auto wrongLHS = make_unique<BinaryOperatorNode>("+", move(z), move(y4));

		// wrong RHS
		auto x4 = make_unique<VariableNode>("x");
		auto num = make_unique<ConstantNode>("1");
		auto wrongRHS = make_unique<BinaryOperatorNode>("+", move(x4), move(num));

		Assert::IsTrue(*plus1 == *plus2);
		Assert::IsFalse(*plus1 == *wrongOperator);
		Assert::IsFalse(*plus1 == *wrongLHS);
		Assert::IsFalse(*plus1 == *wrongRHS);
	}

	TEST_METHOD(Test_BinaryOperatorNode2) {
		// 1*2
		auto num1_1 = make_unique<ConstantNode>("1");
		auto num2_1 = make_unique<ConstantNode>("2");
		auto multiply1 = make_unique<BinaryOperatorNode>("*", move(num1_1), move(num2_1));

		auto num1_2 = make_unique<ConstantNode>("1");
		auto num2_2 = make_unique<ConstantNode>("2");
		auto multiply2 = make_unique<BinaryOperatorNode>("*", move(num1_2), move(num2_2));

		// wrong operator
		auto num1_3 = make_unique<ConstantNode>("1");
		auto num2_3 = make_unique<ConstantNode>("2");
		auto wrongOperator = make_unique<BinaryOperatorNode>("/", move(num1_3), move(num2_3));

		// wrong LHS
		auto x = make_unique<VariableNode>("x");
		auto num2_4 = make_unique<ConstantNode>("2");
		auto wrongLHS = make_unique<BinaryOperatorNode>("*", move(x), move(num2_4));

		// wrong RHS
		auto num1_4 = make_unique<ConstantNode>("1");
		auto num3 = make_unique<ConstantNode>("3");
		auto wrongRHS = make_unique<BinaryOperatorNode>("*", move(num1_4), move(num3));

		Assert::IsTrue(*multiply1 == *multiply2);
		Assert::IsFalse(*multiply1 == *wrongOperator);
		Assert::IsFalse(*multiply1 == *wrongLHS);
		Assert::IsFalse(*multiply1 == *wrongRHS);
	}

	TEST_METHOD(Test_BinaryOperatorNode3) {
		// x%y-z
		auto x1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto modulo1 = make_unique<BinaryOperatorNode>("%", move(x1), move(y1));
		auto z1 = make_unique<VariableNode>("z");
		auto minus1 = make_unique<BinaryOperatorNode>("-", move(modulo1), move(z1));

		auto x2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto modulo2 = make_unique<BinaryOperatorNode>("%", move(x2), move(y2));
		auto z2 = make_unique<VariableNode>("z");
		auto minus2 = make_unique<BinaryOperatorNode>("-", move(modulo2), move(z2));

		// wrong operator
		auto x3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto modulo3 = make_unique<BinaryOperatorNode>("%", move(x3), move(y3));
		auto z3 = make_unique<VariableNode>("z");
		auto wrongOperator = make_unique<BinaryOperatorNode>("%", move(modulo3), move(z3));

		// wrong LHS
		auto x4 = make_unique<VariableNode>("x");
		auto y4 = make_unique<VariableNode>("y");
		auto wrongLHSop = make_unique<BinaryOperatorNode>("*", move(x4), move(y4));
		auto z4 = make_unique<VariableNode>("z");
		auto wrongLHS = make_unique<BinaryOperatorNode>("-", move(wrongLHSop), move(z4));

		// wrong order
		auto x5 = make_unique<VariableNode>("x");
		auto y5 = make_unique<VariableNode>("y");
		auto modulo5 = make_unique<BinaryOperatorNode>("%", move(x5), move(y5));
		auto z5 = make_unique<VariableNode>("z");
		auto wrongOrder = make_unique<BinaryOperatorNode>("-", move(z5), move(modulo5));

		Assert::IsTrue(*minus1 == *minus2);
		Assert::IsFalse(*minus1 == *wrongOperator);
		Assert::IsFalse(*minus1 == *wrongLHS);
		Assert::IsFalse(*minus1 == *wrongOrder);
	}

	TEST_METHOD(Test_NotConditionalExpressionNode) {
		// !(x == y)
		auto x1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto equal1 = make_unique<RelationalExpressionNode>(move(x1), "==", move(y1));
		
		auto not1 = make_unique<NotConditionalExpressionNode>(move(equal1));

		auto x2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto equal2 = make_unique<RelationalExpressionNode>(move(x2), "==", move(y2));
		auto not2 = make_unique<NotConditionalExpressionNode>(move(equal2));

		auto x3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto notequal = make_unique<RelationalExpressionNode>(move(x3), "!=", move(y3));
		auto not3 = make_unique<NotConditionalExpressionNode>(move(notequal));

		Assert::IsTrue(*not1 == *not2);
		Assert::IsFalse(*not1 == *not3);
	}

	TEST_METHOD(Test_AndConditionalExpressionNode) {
		// (x <= y) && (a >= b)
		auto x1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto LHSleq1 = make_unique<RelationalExpressionNode>(move(x1), "<=", move(y1));
		auto a1 = make_unique<VariableNode>("a");
		auto b1 = make_unique<VariableNode>("b");
		auto RHSgeq1 = make_unique<RelationalExpressionNode>(move(a1), ">=", move(b1));
		auto and1 = make_unique<AndConditionalExpressionNode>(move(LHSleq1), move(RHSgeq1));

		auto x2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto LHSleq2 = make_unique<RelationalExpressionNode>(move(x2), "<=", move(y2));
		auto a2 = make_unique<VariableNode>("a");
		auto b2 = make_unique<VariableNode>("b");
		auto RHSgeq2 = make_unique<RelationalExpressionNode>(move(a2), ">=", move(b2));
		auto and2 = make_unique<AndConditionalExpressionNode>(move(LHSleq2), move(RHSgeq2));

		// wrong operator
		auto x3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto LHSleq3 = make_unique<RelationalExpressionNode>(move(x3), "<=", move(y3));
		auto a3 = make_unique<VariableNode>("a");
		auto b3 = make_unique<VariableNode>("b");
		auto RHSgeq3 = make_unique<RelationalExpressionNode>(move(a3), ">=", move(b3));
		auto wrongOperator = make_unique<OrConditionalExpressionNode>(move(LHSleq3), move(RHSgeq3));

		// wrong LHS
		auto x4 = make_unique<VariableNode>("x");
		auto y4 = make_unique<VariableNode>("y");
		auto LHSless = make_unique<RelationalExpressionNode>(move(x4), "<", move(y4));
		auto a4 = make_unique<VariableNode>("a");
		auto b4 = make_unique<VariableNode>("b");
		auto RHSgeq4 = make_unique<RelationalExpressionNode>(move(a4), ">=", move(b4));
		auto wrongLHS = make_unique<AndConditionalExpressionNode>(move(LHSless), move(RHSgeq4));

		// wrong RHS
		auto x5 = make_unique<VariableNode>("x");
		auto y5 = make_unique<VariableNode>("y");
		auto LHSleq5 = make_unique<RelationalExpressionNode>(move(x5), "<=", move(y5));
		auto a5 = make_unique<VariableNode>("a");
		auto b5 = make_unique<VariableNode>("b");
		auto RHSgreater = make_unique<RelationalExpressionNode>(move(a5), ">", move(b5));
		auto wrongRHS = make_unique<AndConditionalExpressionNode>(move(LHSleq5), move(RHSgreater));

		Assert::IsTrue(*and1 == *and2);
		Assert::IsFalse(*and1 == *wrongOperator);
		Assert::IsFalse(*and1 == *wrongLHS);
		Assert::IsFalse(*and1 == *wrongRHS);
	}

	TEST_METHOD(Test_OrConditionalExpressionNode) {
		// (x < y) || (a > b)
		auto x1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto LHSless1 = make_unique<RelationalExpressionNode>(move(x1), "<", move(y1));
		auto a1 = make_unique<VariableNode>("a");
		auto b1 = make_unique<VariableNode>("b");
		auto RHSgreater1 = make_unique<RelationalExpressionNode>(move(a1), ">", move(b1));
		auto or1 = make_unique<OrConditionalExpressionNode>(move(LHSless1), move(RHSgreater1));

		auto x2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto LHSless2 = make_unique<RelationalExpressionNode>(move(x2), "<", move(y2));
		auto a2 = make_unique<VariableNode>("a");
		auto b2 = make_unique<VariableNode>("b");
		auto RHSgreater2 = make_unique<RelationalExpressionNode>(move(a2), ">", move(b2));
		auto or2 = make_unique<OrConditionalExpressionNode>(move(LHSless2), move(RHSgreater2));

		// wrong operator
		auto x3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto LHSless3 = make_unique<RelationalExpressionNode>(move(x3), "<", move(y3));
		auto a3 = make_unique<VariableNode>("a");
		auto b3 = make_unique<VariableNode>("b");
		auto RHSgreater3 = make_unique<RelationalExpressionNode>(move(a3), ">", move(b3));
		auto wrongOperator = make_unique<AndConditionalExpressionNode>(move(LHSless3), move(RHSgreater3));

		// wrong LHS
		auto x4 = make_unique<VariableNode>("x");
		auto y4 = make_unique<VariableNode>("y");
		auto LHSnotEqual = make_unique<RelationalExpressionNode>(move(x4), "!=", move(y4));
		auto a4 = make_unique<VariableNode>("a");
		auto b4 = make_unique<VariableNode>("b");
		auto RHSgreater4 = make_unique<RelationalExpressionNode>(move(a4), ">", move(b4));
		auto wrongLHS = make_unique<OrConditionalExpressionNode>(move(LHSnotEqual), move(RHSgreater4));

		// wrong RHS
		auto x5 = make_unique<VariableNode>("x");
		auto y5 = make_unique<VariableNode>("y");
		auto LHSless5 = make_unique<RelationalExpressionNode>(move(x5), "<", move(y5));
		auto a5 = make_unique<VariableNode>("a");
		auto b5 = make_unique<VariableNode>("b");
		auto RHSnotEqual = make_unique<RelationalExpressionNode>(move(a5), "!=", move(b5));
		auto wrongRHS = make_unique<OrConditionalExpressionNode>(move(LHSless5), move(RHSnotEqual));

		Assert::IsTrue(*or1 == *or2);
		Assert::IsFalse(*or1 == *wrongOperator);
		Assert::IsFalse(*or1 == *wrongLHS);
		Assert::IsFalse(*or1 == *wrongRHS);
	}

	TEST_METHOD(Test_RelationalExpressionNode1) {
		// x == y
		auto x1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto equal1 = make_unique<RelationalExpressionNode>(move(x1), "==", move(y1));

		auto x2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto equal2 = make_unique<RelationalExpressionNode>(move(x2), "==", move(y2));

		// wrong operator
		auto x3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto wrongOperator = make_unique<RelationalExpressionNode>(move(x3), "!=", move(y3));

		// wrong LHS
		auto z = make_unique<VariableNode>("z");
		auto y4 = make_unique<VariableNode>("y");
		auto wrongLHS = make_unique<RelationalExpressionNode>(move(z), "==", move(y4));

		// wrong RHS
		auto x5 = make_unique<VariableNode>("x");
		auto num = make_unique<ConstantNode>("1");
		auto wrongRHS = make_unique<RelationalExpressionNode>(move(x5), "==", move(num));

		Assert::IsTrue(*equal1 == *equal2);
		Assert::IsFalse(*equal1 == *wrongOperator);
		Assert::IsFalse(*equal1 == *wrongLHS);
		Assert::IsFalse(*equal1 == *wrongRHS);
	}

	TEST_METHOD(Test_RelationalExpressionNode2) {
		// 1 <= 2
		auto num1_1 = make_unique<ConstantNode>("1");
		auto num2_1 = make_unique<ConstantNode>("2");
		auto leq1 = make_unique<RelationalExpressionNode>(move(num1_1), "<=", move(num2_1));

		auto num1_2 = make_unique<ConstantNode>("1");
		auto num2_2 = make_unique<ConstantNode>("2");
		auto leq2 = make_unique<RelationalExpressionNode>(move(num1_2), "<=", move(num2_2));

		// wrong operator
		auto num1_3 = make_unique<ConstantNode>("1");
		auto num2_3 = make_unique<ConstantNode>("2");
		auto wrongOperator = make_unique<RelationalExpressionNode>(move(num1_3), "<", move(num2_3));

		// wrong LHS
		auto x = make_unique<VariableNode>("x");
		auto num2_4 = make_unique<ConstantNode>("2");
		auto wrongLHS = make_unique<RelationalExpressionNode>(move(x), "<=", move(num2_4));

		// wrong RHS
		auto num1_5 = make_unique<ConstantNode>("1");
		auto num3 = make_unique<ConstantNode>("3");
		auto wrongRHS = make_unique<RelationalExpressionNode>(move(num1_5), "<=", move(num3));

		Assert::IsTrue(*leq1 == *leq2);
		Assert::IsFalse(*leq1 == *wrongOperator);
		Assert::IsFalse(*leq1 == *wrongLHS);
		Assert::IsFalse(*leq1 == *wrongRHS);
	}

	TEST_METHOD(Test_RelationalExpressionNode3) {
		// x >= y + z
		auto x1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto z1 = make_unique<VariableNode>("z");
		auto plus1 = make_unique<BinaryOperatorNode>("+", move(y1), move(z1));
		auto geq1 = make_unique<RelationalExpressionNode>(move(x1), ">=", move(plus1));

		auto x2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto z2 = make_unique<VariableNode>("z");
		auto plus2 = make_unique<BinaryOperatorNode>("+", move(y2), move(z2));
		auto geq2 = make_unique<RelationalExpressionNode>(move(x2), ">=", move(plus2));

		// wrong operator
		auto x3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto z3 = make_unique<VariableNode>("z");
		auto plus3 = make_unique<BinaryOperatorNode>("+", move(y3), move(z3));
		auto wrongOperator = make_unique<RelationalExpressionNode>(move(x3), ">", move(plus3));

		// wrong RHS
		auto x4 = make_unique<VariableNode>("x");
		auto y4 = make_unique<VariableNode>("y");
		auto z4 = make_unique<VariableNode>("z");
		auto wrongRHSop = make_unique<BinaryOperatorNode>("-", move(y4), move(z4));
		auto wrongRHS = make_unique<RelationalExpressionNode>(move(x4), ">=", move(wrongRHSop));

		// wrong order
		auto x5 = make_unique<VariableNode>("x");
		auto y5 = make_unique<VariableNode>("y");
		auto z5 = make_unique<VariableNode>("z");
		auto plus5 = make_unique<BinaryOperatorNode>("+", move(y5), move(z5));
		auto wrongOrder = make_unique<RelationalExpressionNode>(move(plus5), ">=", move(x5));

		Assert::IsTrue(*geq1 == *geq2);
		Assert::IsFalse(*geq1 == *wrongOperator);
		Assert::IsFalse(*geq1 == *wrongRHS);
		Assert::IsFalse(*geq1 == *wrongRHS);
	}

	TEST_METHOD(Test_ReadStatementNode) {
		// read x
		auto x1 = make_unique<VariableNode>("x");
		auto read1 = make_unique<ReadStatementNode>(1, move(x1));

		auto x2 = make_unique<VariableNode>("x");
		auto read2 = make_unique<ReadStatementNode>(1, move(x2));

		// wrong stmtNo
		auto x3 = make_unique<VariableNode>("x");
		auto read3 = make_unique<ReadStatementNode>(2, move(x3));

		// wrong variable
		auto y = make_unique<VariableNode>("y");
		auto read4 = make_unique<ReadStatementNode>(1, move(y));

		Assert::IsTrue(*read1 == *read2);
		Assert::IsFalse(*read1 == *read3);
		Assert::IsFalse(*read1 == *read4);
	}

	TEST_METHOD(Test_PrintStatementNode) {
		// print x
		auto x1 = make_unique<VariableNode>("x");
		auto print1 = make_unique<PrintStatementNode>(1, move(x1));

		auto x2 = make_unique<VariableNode>("x");
		auto print2 = make_unique<PrintStatementNode>(1, move(x2));

		// wrong stmtNo
		auto x3 = make_unique<VariableNode>("x");
		auto print3 = make_unique<PrintStatementNode>(2, move(x3));

		// wrong variable
		auto y = make_unique<VariableNode>("y");
		auto print4 = make_unique<PrintStatementNode>(1, move(y));

		Assert::IsTrue(*print1 == *print2);
		Assert::IsFalse(*print1 == *print3);
		Assert::IsFalse(*print1 == *print4);
	}

	TEST_METHOD(Test_CallStatementNode) {
		// call p
		auto call1 = make_unique<CallStatementNode>(1, "p");
		auto call2 = make_unique<CallStatementNode>(1, "p");
		auto call3 = make_unique<CallStatementNode>(1, "q");

		Assert::IsTrue(*call1 == *call2);
		Assert::IsFalse(*call1 == *call3);
	}

	TEST_METHOD(Test_WhileStatementNode) {
		// while (x == y) { print a; read b; }
		auto x1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto cond1 = make_unique<RelationalExpressionNode>(move(x1), "==", move(y1));
		auto a1 = make_unique<VariableNode>("a");
		auto print1 = make_unique<PrintStatementNode>(2, move(a1));
		auto b1 = make_unique<VariableNode>("b");
		auto read1 = make_unique<ReadStatementNode>(3, move(b1));
		vector<StatementNode> statements1;
		statements1.push_back(move(print1));
		statements1.push_back(move(read1));
		auto while1 = make_unique<WhileStatementNode>(1, move(cond1), move(statements1));

		auto x2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto cond2 = make_unique<RelationalExpressionNode>(move(x2), "==", move(y2));
		auto a2 = make_unique<VariableNode>("a");
		auto print2 = make_unique<PrintStatementNode>(2, move(a2));
		auto b2 = make_unique<VariableNode>("b");
		auto read2 = make_unique<ReadStatementNode>(3, move(b2));
		vector<StatementNode> statements2;
		statements2.push_back(move(print2));
		statements2.push_back(move(read2));
		auto while2 = make_unique<WhileStatementNode>(1, move(cond2), move(statements2));

		// wrong stmtNo
		auto x3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto cond3 = make_unique<RelationalExpressionNode>(move(x3), "==", move(y3));
		auto a3 = make_unique<VariableNode>("a");
		auto print3 = make_unique<PrintStatementNode>(2, move(a3));
		auto b3 = make_unique<VariableNode>("b");
		auto read3 = make_unique<ReadStatementNode>(3, move(b3));
		vector<StatementNode> statements3;
		statements3.push_back(move(print3));
		statements3.push_back(move(read3));
		auto wrongStmtNo = make_unique<WhileStatementNode>(4, move(cond3), move(statements3));

		// wrong condition
		auto x4 = make_unique<VariableNode>("x");
		auto y4 = make_unique<VariableNode>("y");
		auto cond4 = make_unique<RelationalExpressionNode>(move(x4), "!=", move(y4));
		auto a4 = make_unique<VariableNode>("a");
		auto print4 = make_unique<PrintStatementNode>(2, move(a4));
		auto b4 = make_unique<VariableNode>("b");
		auto read4 = make_unique<ReadStatementNode>(3, move(b4));
		vector<StatementNode> statements4;
		statements4.push_back(move(print4));
		statements4.push_back(move(read4));
		auto wrongCondition = make_unique<WhileStatementNode>(1, move(cond4), move(statements4));

		// wrong statements
		auto x5 = make_unique<VariableNode>("x");
		auto y5 = make_unique<VariableNode>("y");
		auto cond5 = make_unique<RelationalExpressionNode>(move(x5), "==", move(y5));
		auto a5 = make_unique<VariableNode>("a");
		auto print5 = make_unique<PrintStatementNode>(2, move(a5));
		auto b5 = make_unique<VariableNode>("b");
		auto read5 = make_unique<ReadStatementNode>(3, move(b5));
		vector<StatementNode> statements5;
		statements5.push_back(move(read5));
		statements5.push_back(move(print5));
		auto wrongStatements = make_unique<WhileStatementNode>(1, move(cond5), move(statements5));

		Assert::IsTrue(*while1 == *while2);
		Assert::IsFalse(*while1 == *wrongStmtNo);
		Assert::IsFalse(*while1 == *wrongCondition);
		Assert::IsFalse(*while1 == *wrongStatements);
	}

	TEST_METHOD(Test_IfStatementNode) {
		// if (x == y) then { print a; } else { read b; }
		auto x1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto cond1 = make_unique<RelationalExpressionNode>(move(x1), "==", move(y1));
		auto a1 = make_unique<VariableNode>("a");
		auto print1 = make_unique<PrintStatementNode>(2, move(a1));
		auto b1 = make_unique<VariableNode>("b");
		auto read1 = make_unique<ReadStatementNode>(3, move(b1));
		vector<StatementNode> thenStatements1;
		vector<StatementNode> elseStatements1;
		thenStatements1.push_back(move(print1));
		elseStatements1.push_back(move(read1));
		auto if1 = make_unique<IfStatementNode>(1, move(cond1), move(thenStatements1), move(elseStatements1));

		auto x2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto cond2 = make_unique<RelationalExpressionNode>(move(x2), "==", move(y2));
		auto a2 = make_unique<VariableNode>("a");
		auto print2 = make_unique<PrintStatementNode>(2, move(a2));
		auto b2 = make_unique<VariableNode>("b");
		auto read2 = make_unique<ReadStatementNode>(3, move(b2));
		vector<StatementNode> thenStatements2;
		vector<StatementNode> elseStatements2;
		thenStatements2.push_back(move(print2));
		elseStatements2.push_back(move(read2));
		auto if2 = make_unique<IfStatementNode>(1, move(cond2), move(thenStatements2), move(elseStatements2));

		// wrong stmtNo
		auto x3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto cond3 = make_unique<RelationalExpressionNode>(move(x3), "==", move(y3));
		auto a3 = make_unique<VariableNode>("a");
		auto print3 = make_unique<PrintStatementNode>(2, move(a3));
		auto b3 = make_unique<VariableNode>("b");
		auto read3 = make_unique<ReadStatementNode>(3, move(b3));
		vector<StatementNode> thenStatements3;
		vector<StatementNode> elseStatements3;
		thenStatements3.push_back(move(print3));
		elseStatements3.push_back(move(read3));
		auto wrongStmtNo = make_unique<IfStatementNode>(4, move(cond3), move(thenStatements3), move(elseStatements3));

		// wrong condition
		auto x4 = make_unique<VariableNode>("x");
		auto y4 = make_unique<VariableNode>("y");
		auto cond4 = make_unique<RelationalExpressionNode>(move(x4), "!=", move(y4));
		auto a4 = make_unique<VariableNode>("a");
		auto print4 = make_unique<PrintStatementNode>(2, move(a4));
		auto b4 = make_unique<VariableNode>("b");
		auto read4 = make_unique<ReadStatementNode>(3, move(b4));
		vector<StatementNode> thenStatements4;
		vector<StatementNode> elseStatements4;
		thenStatements4.push_back(move(print4));
		elseStatements4.push_back(move(read4));
		auto wrongCondition = make_unique<IfStatementNode>(1, move(cond4), move(thenStatements4), move(elseStatements4));

		// wrong statements
		auto x5 = make_unique<VariableNode>("x");
		auto y5 = make_unique<VariableNode>("y");
		auto cond5 = make_unique<RelationalExpressionNode>(move(x5), "==", move(y5));
		auto a5 = make_unique<VariableNode>("a");
		auto print5 = make_unique<PrintStatementNode>(2, move(a5));
		auto b5 = make_unique<VariableNode>("b");
		auto read5 = make_unique<ReadStatementNode>(3, move(b5));
		vector<StatementNode> thenStatements5;
		vector<StatementNode> elseStatements5;
		thenStatements5.push_back(move(read5));
		elseStatements5.push_back(move(print5));
		auto wrongStatements = make_unique<IfStatementNode>(1, move(cond5), move(thenStatements5), move(elseStatements5));

		Assert::IsTrue(*if1 == *if2);
		Assert::IsFalse(*if1 == *wrongStmtNo);
		Assert::IsFalse(*if1 == *wrongCondition);
		Assert::IsFalse(*if1 == *wrongStatements);
	}

	TEST_METHOD(Test_AssignStatementNode) {
		// x = y + 1
		auto LHS1 = make_unique<VariableNode>("x");
		auto y1 = make_unique<VariableNode>("y");
		auto num1 = make_unique<ConstantNode>("1");
		auto RHS1 = make_unique<BinaryOperatorNode>("+", move(y1), move(num1));
		auto assign1 = make_unique<AssignStatementNode>(1, move(LHS1), move(RHS1));

		auto LHS2 = make_unique<VariableNode>("x");
		auto y2 = make_unique<VariableNode>("y");
		auto num2 = make_unique<ConstantNode>("1");
		auto RHS2 = make_unique<BinaryOperatorNode>("+", move(y2), move(num2));
		auto assign2 = make_unique<AssignStatementNode>(1, move(LHS2), move(RHS2));

		// wrong stmtNo
		auto LHS3 = make_unique<VariableNode>("x");
		auto y3 = make_unique<VariableNode>("y");
		auto num3 = make_unique<ConstantNode>("1");
		auto RHS3 = make_unique<BinaryOperatorNode>("+", move(y3), move(num3));
		auto wrongStmtNo = make_unique<AssignStatementNode>(2, move(LHS3), move(RHS3));

		// wrong LHS
		auto LHS4 = make_unique<VariableNode>("z");
		auto y4 = make_unique<VariableNode>("y");
		auto num4 = make_unique<ConstantNode>("1");
		auto RHS4 = make_unique<BinaryOperatorNode>("+", move(y4), move(num4));
		auto wrongLHS = make_unique<AssignStatementNode>(1, move(LHS4), move(RHS4));

		// wrong RHS
		auto LHS5 = make_unique<VariableNode>("x");
		auto y5 = make_unique<VariableNode>("y");
		auto num5 = make_unique<ConstantNode>("1");
		auto RHS5 = make_unique<BinaryOperatorNode>("-", move(y5), move(num5));
		auto wrongRHS = make_unique<AssignStatementNode>(1, move(LHS5), move(RHS5));

		Assert::IsTrue(*assign1 == *assign2);
		Assert::IsFalse(*assign1 == *wrongStmtNo);
		Assert::IsFalse(*assign1 == *wrongLHS);
		Assert::IsFalse(*assign1 == *wrongRHS);
	}

	TEST_METHOD(Test_ProcedureNode) {
		// procedure p { print a; read b; }
		auto a1 = make_unique<VariableNode>("a");
		auto print1 = make_unique<PrintStatementNode>(1, move(a1));
		auto b1 = make_unique<VariableNode>("b");
		auto read1 = make_unique<ReadStatementNode>(2, move(b1));
		vector<StatementNode> statements1;
		statements1.push_back(move(print1));
		statements1.push_back(move(read1));
		auto p1 = make_unique<ProcedureNode>("p", move(statements1));

		auto a2 = make_unique<VariableNode>("a");
		auto print2 = make_unique<PrintStatementNode>(1, move(a2));
		auto b2 = make_unique<VariableNode>("b");
		auto read2 = make_unique<ReadStatementNode>(2, move(b2));
		vector<StatementNode> statements2;
		statements2.push_back(move(print2));
		statements2.push_back(move(read2));
		auto p2 = make_unique<ProcedureNode>("p", move(statements2));

		// wrong name
		auto a3 = make_unique<VariableNode>("a");
		auto print3 = make_unique<PrintStatementNode>(1, move(a3));
		auto b3 = make_unique<VariableNode>("b");
		auto read3 = make_unique<ReadStatementNode>(2, move(b3));
		vector<StatementNode> statements3;
		statements3.push_back(move(print3));
		statements3.push_back(move(read3));
		auto wrongName = make_unique<ProcedureNode>("q", move(statements3));

		// wrong statements
		auto a4 = make_unique<VariableNode>("a");
		auto print4 = make_unique<PrintStatementNode>(1, move(a4));
		auto b4 = make_unique<VariableNode>("b");
		auto read4 = make_unique<ReadStatementNode>(2, move(b4));
		vector<StatementNode> statements4;
		statements4.push_back(move(read4));
		statements4.push_back(move(print4));
		auto wrongStatements = make_unique<ProcedureNode>("p", move(statements4));

		Assert::IsTrue(*p1 == *p2);
		Assert::IsFalse(*p1 == *wrongName);
		Assert::IsFalse(*p1 == *wrongStatements);
	}

	TEST_METHOD(Test_ProgramNode) {
		// procedure p { print a; } procedure q { read b; }
		auto a1 = make_unique<VariableNode>("a");
		auto print1 = make_unique<PrintStatementNode>(1, move(a1));
		auto b1 = make_unique<VariableNode>("b");
		auto read1 = make_unique<ReadStatementNode>(2, move(b1));
		vector<StatementNode> pstatements1;
		pstatements1.push_back(move(print1));
		vector<StatementNode> qstatements1;
		qstatements1.push_back(move(read1));
		auto p1 = make_unique<ProcedureNode>("p", move(pstatements1));
		auto q1 = make_unique<ProcedureNode>("q", move(qstatements1));
		vector<unique_ptr<ProcedureNode>> procedures1;
		procedures1.push_back(move(p1));
		procedures1.push_back(move(q1));
		auto prog1 = make_unique<ProgramNode>(move(procedures1));

		auto a2 = make_unique<VariableNode>("a");
		auto print2 = make_unique<PrintStatementNode>(1, move(a2));
		auto b2 = make_unique<VariableNode>("b");
		auto read2 = make_unique<ReadStatementNode>(2, move(b2));
		vector<StatementNode> pstatements2;
		pstatements2.push_back(move(print2));
		vector<StatementNode> qstatements2;
		qstatements2.push_back(move(read2));
		auto p2 = make_unique<ProcedureNode>("p", move(pstatements2));
		auto q2 = make_unique<ProcedureNode>("q", move(qstatements2));
		vector<unique_ptr<ProcedureNode>> procedures2;
		procedures2.push_back(move(p2));
		procedures2.push_back(move(q2));
		auto prog2 = make_unique<ProgramNode>(move(procedures2));

		// wrong procedures
		auto a3 = make_unique<VariableNode>("a");
		auto print3 = make_unique<PrintStatementNode>(1, move(a3));
		auto b3 = make_unique<VariableNode>("b");
		auto read3 = make_unique<ReadStatementNode>(2, move(b3));
		vector<StatementNode> pstatements3;
		pstatements3.push_back(move(print3));
		vector<StatementNode> qstatements3;
		qstatements3.push_back(move(read3));
		auto p3 = make_unique<ProcedureNode>("p", move(pstatements3));
		auto q3 = make_unique<ProcedureNode>("q", move(qstatements3));
		vector<unique_ptr<ProcedureNode>> procedures3;
		procedures3.push_back(move(q3));
		procedures3.push_back(move(p3));
		auto wrongProcedures = make_unique<ProgramNode>(move(procedures3));

		Assert::IsTrue(*prog1 == *prog2);
		Assert::IsFalse(*prog1 == *wrongProcedures);
	}
	};
}