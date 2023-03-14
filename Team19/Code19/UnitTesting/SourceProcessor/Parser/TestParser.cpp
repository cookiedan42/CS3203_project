#include "stdafx.h"
#include "CppUnitTest.h"
#include <vector>
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/AST/AST.h"
#include "../source/SourceProcessor/Parser/Parser.h"
#include "../source/SourceProcessor/Parser/ExpressionParser.h"
#include "../source/SourceProcessor/Parser/ParseException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace std;

namespace UnitTesting {
	TEST_CLASS(TestParser) {
	public:
		TEST_METHOD(Test_No_Procedure) {
			auto source = ""; // empty file
			auto ast = Parser(TokenSplitter(source).tokenize()).parseTokens();

			Assert::IsNull(ast.get());
		}

		TEST_METHOD(Test_Multiple_Procedure) {
			auto source = "procedure main1 { print x; } procedure main2 { read y; }";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements1;
			vector<StatementNode> statements2;

			auto print = make_unique<PrintStatementNode>(1, make_unique<VariableNode>("x"));
			statements1.push_back(move(print));
			auto procedure1 = make_unique<ProcedureNode>("main1", move(statements1));

			auto read = make_unique<ReadStatementNode>(2, make_unique<VariableNode>("y"));
			statements2.push_back(move(read));
			auto procedure2 = make_unique<ProcedureNode>("main2", move(statements2));

			procedures.push_back(move(procedure1));
			procedures.push_back(move(procedure2));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_Read) {
			auto source = "procedure main { read x; }";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<StatementNode> statements;
			vector<unique_ptr<ProcedureNode>> procedures;

			auto read = make_unique<ReadStatementNode>(1, make_unique<VariableNode>("x"));
			statements.push_back(move(read));
			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));
			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_Print) {
			auto source = "procedure main { print x; }";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements;

			auto print = make_unique<PrintStatementNode>(1, make_unique<VariableNode>("x"));
			statements.push_back(move(print));

			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_Call) {
			auto source = "procedure main { call p; }";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements;

			auto call = make_unique<CallStatementNode>(1, "p");
			statements.push_back(move(call));

			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_While) {
			auto source = "procedure main {"
							"while (x > 0) { print y; }"
						  "}";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements;
			vector<StatementNode> whileStatements;

			auto x = make_unique<VariableNode>("x");
			auto num = make_unique<ConstantNode>("0");
			auto condition = make_unique<RelationalExpressionNode>(move(x), ">", move(num));
			
			auto print = make_unique<PrintStatementNode>(2, make_unique<VariableNode>("y"));
			whileStatements.push_back(move(print));

			auto whiles = make_unique<WhileStatementNode>(1, move(condition), move(whileStatements));
			statements.push_back(move(whiles));

			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_While_ComplicatedCondition) {
			auto source = "procedure main {"
							"while (((x+y/z) > 0) && (!(a <= b*c-1))) { print y; }"
						  "}";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements;
			vector<StatementNode> whileStatements;

			auto y = make_unique<VariableNode>("y");
			auto z = make_unique<VariableNode>("z");
			auto divide = make_unique<BinaryOperatorNode>("/", move(y), move(z));
			auto x = make_unique<VariableNode>("x");
			auto plus = make_unique<BinaryOperatorNode>("+", move(x), move(divide));
			auto num0 = make_unique<ConstantNode>("0");
			auto greater = make_unique<RelationalExpressionNode>(move(plus), ">", move(num0));

			auto a = make_unique<VariableNode>("a");
			auto b = make_unique<VariableNode>("b");
			auto c = make_unique<VariableNode>("c");
			auto multiply = make_unique<BinaryOperatorNode>("*", move(b), move(c));
			auto num1 = make_unique<ConstantNode>("1");
			auto minus = make_unique<BinaryOperatorNode>("-", move(multiply), move(num1));
			auto lessEqual = make_unique<RelationalExpressionNode>(move(a), "<=", move(minus));
			auto not = make_unique<NotConditionalExpressionNode>(move(lessEqual));

			auto condition = make_unique<AndConditionalExpressionNode>(move(greater), move(not));

			auto print = make_unique<PrintStatementNode>(2, make_unique<VariableNode>("y"));
			whileStatements.push_back(move(print));

			auto whiles = make_unique<WhileStatementNode>(1, move(condition), move(whileStatements));
			statements.push_back(move(whiles));

			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_While_Nested) {
			auto source = "procedure main {"
							"while (x > 0) {"
								"while (y > 0) {"
									"print z;"
								"}"
							"}"
						  "}";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements;
			vector<StatementNode> outerWhileStatements;
			vector<StatementNode> innerWhileStatements;

			auto y = make_unique<VariableNode>("y");
			auto num1 = make_unique<ConstantNode>("0");
			auto innerCondition = make_unique<RelationalExpressionNode>(move(y), ">", move(num1));
			auto print = make_unique<PrintStatementNode>(3, make_unique<VariableNode>("z"));
			innerWhileStatements.push_back(move(print));
			auto innerWhile = make_unique<WhileStatementNode>(2, move(innerCondition), move(innerWhileStatements));

			auto x = make_unique<VariableNode>("x");
			auto num2 = make_unique<ConstantNode>("0");
			auto outerCondition = make_unique<RelationalExpressionNode>(move(x), ">", move(num2));
			outerWhileStatements.push_back(move(innerWhile));
			auto outerWhile = make_unique<WhileStatementNode>(1, move(outerCondition), move(outerWhileStatements));
			
			statements.push_back(move(outerWhile));

			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_While_NoStatement) {
			auto source = "procedure main {"
							"while (x > 0) {}"
						  "}";
			auto ast = Parser(TokenSplitter(source).tokenize()).parseTokens();

			Assert::IsNull(ast.get());
		}

		TEST_METHOD(Test_If) {
			auto source = "procedure main {"
							"if (x > 0) then { print y; } else { read z; }"
						  "}";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements;
			vector<StatementNode> thenStatements;
			vector<StatementNode> elseStatements;

			auto x = make_unique<VariableNode>("x");
			auto num = make_unique<ConstantNode>("0");
			auto condition = make_unique<RelationalExpressionNode>(move(x), ">", move(num));

			auto print = make_unique<PrintStatementNode>(2, make_unique<VariableNode>("y"));
			thenStatements.push_back(move(print));

			auto read = make_unique<ReadStatementNode>(3, make_unique<VariableNode>("z"));
			elseStatements.push_back(move(read));

			auto ifs = make_unique<IfStatementNode>(1, move(condition), move(thenStatements), move(elseStatements));
			statements.push_back(move(ifs));

			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_If_Nested) {
			auto source = "procedure main {"
							"if (x > 0) then {"
								"if (y < z) then {"
									"print a;"
								"} else {"
									"read b;"
								"}"
							"} else {"
								"c = d;"
							"}"
						  "}";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements;
			vector<StatementNode> outerThenStatements;
			vector<StatementNode> outerElseStatements;
			vector<StatementNode> innerThenStatements;
			vector<StatementNode> innerElseStatements;

			auto y = make_unique<VariableNode>("y");
			auto z = make_unique<VariableNode>("z");
			auto innerCondition = make_unique<RelationalExpressionNode>(move(y), "<", move(z));
			auto print = make_unique<PrintStatementNode>(3, make_unique<VariableNode>("a"));
			innerThenStatements.push_back(move(print));
			auto read = make_unique<ReadStatementNode>(4, make_unique<VariableNode>("b"));
			innerElseStatements.push_back(move(read));
			auto innerIf = make_unique<IfStatementNode>(2, move(innerCondition), move(innerThenStatements), move(innerElseStatements));

			auto x = make_unique<VariableNode>("x");
			auto num = make_unique<ConstantNode>("0");
			auto outerCondition = make_unique<RelationalExpressionNode>(move(x), ">", move(num));
			outerThenStatements.push_back(move(innerIf));
			auto c = make_unique<VariableNode>("c");
			auto d = make_unique<VariableNode>("d");
			auto assign = make_unique<AssignStatementNode>(5, move(c), move(d));
			outerElseStatements.push_back(move(assign));
			auto outerIf = make_unique<IfStatementNode>(1, move(outerCondition), move(outerThenStatements), move(outerElseStatements));

			statements.push_back(move(outerIf));

			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_If_NoThenStatement) {
			auto source = "procedure main {"
							"if (x > 0) then { } else { read z; }"
						  "}";
			auto ast = Parser(TokenSplitter(source).tokenize()).parseTokens();

			Assert::IsNull(ast.get());
		}

		TEST_METHOD(Test_If_NoElseStatement) {
			auto source = "procedure main {"
							"if (x > 0) then { print y; } else { }"
						  "}";
			auto ast = Parser(TokenSplitter(source).tokenize()).parseTokens();

			Assert::IsNull(ast.get());
		}

		TEST_METHOD(Test_Assign) {
			auto source = "procedure main {"
							"x = x + y + z + 1;"
						  "}";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements;

			auto LHS = make_unique<VariableNode>("x");

			auto x = make_unique<VariableNode>("x");
			auto y = make_unique<VariableNode>("y");
			auto plus1 = make_unique<BinaryOperatorNode>("+", move(x), move(y));
			auto z = make_unique<VariableNode>("z");
			auto plus2 = make_unique<BinaryOperatorNode>("+", move(plus1), move(z));
			auto num1 = make_unique<ConstantNode>("1");
			auto RHS = make_unique<BinaryOperatorNode>("+", move(plus2), move(num1));

			auto assign = make_unique<AssignStatementNode>(1, move(LHS), move(RHS));
			statements.push_back(move(assign));

			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}

		TEST_METHOD(Test_Multiple_Statements) {
			auto source = "procedure main {"
							"read x;"
							"print y;"
							"z = z + 1;"
							"while (a <= b) {"
								"read c;"
							"}"
							"if (d >= e) then {"
								"print f;"
							"} else {"
								"g = h;"
							"}"
						  "}";
			auto actual = Parser(TokenSplitter(source).tokenize()).parseTokens();

			vector<unique_ptr<ProcedureNode>> procedures;
			vector<StatementNode> statements;
			vector<StatementNode> whileStatements;
			vector<StatementNode> thenStatements;
			vector<StatementNode> elseStatements;

			auto read = make_unique<ReadStatementNode>(1, make_unique<VariableNode>("x"));
			auto print = make_unique<PrintStatementNode>(2, make_unique<VariableNode>("y"));

			auto assignLHS = make_unique<VariableNode>("z");
			auto z = make_unique<VariableNode>("z");
			auto num1 = make_unique<ConstantNode>("1");
			auto assignRHS = make_unique<BinaryOperatorNode>("+", move(z), move(num1));
			auto assign = make_unique<AssignStatementNode>(3, move(assignLHS), move(assignRHS));

			auto whileCondition = make_unique<RelationalExpressionNode>(make_unique<VariableNode>("a"), "<=", make_unique<VariableNode>("b"));
			whileStatements.push_back(make_unique<ReadStatementNode>(5, make_unique<VariableNode>("c")));
			auto whiles = make_unique<WhileStatementNode>(4, move(whileCondition), move(whileStatements));

			auto ifCondition = make_unique<RelationalExpressionNode>(make_unique<VariableNode>("d"), ">=", make_unique<VariableNode>("e"));
			thenStatements.push_back(make_unique<PrintStatementNode>(7, make_unique<VariableNode>("f")));
			elseStatements.push_back(make_unique<AssignStatementNode>(8, make_unique<VariableNode>("g"), make_unique<VariableNode>("h")));
			auto ifs = make_unique<IfStatementNode>(6, move(ifCondition), move(thenStatements), move(elseStatements));

			statements.push_back(move(read));
			statements.push_back(move(print));
			statements.push_back(move(assign));
			statements.push_back(move(whiles));
			statements.push_back(move(ifs));

			auto procedure = make_unique<ProcedureNode>("main", move(statements));
			procedures.push_back(move(procedure));

			auto expected = make_unique<ProgramNode>(move(procedures));

			Assert::IsTrue(*expected == *actual);
		}
		TEST_METHOD(Test_No_Statement) {
			auto source = "procedure main { }";
			auto ast = Parser(TokenSplitter(source).tokenize()).parseTokens();

			Assert::IsNull(ast.get());
		}
	};
}