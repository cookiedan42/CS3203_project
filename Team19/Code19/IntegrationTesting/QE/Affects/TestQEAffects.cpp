#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../AffectsContext.h"
#include "../source/QP/QueryEvaluator/AffectsEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Test_QE_Affects {
	TEST_CLASS(Test_QEAffects) {

	shared_ptr<DA::AffectsRel> makeAffects(stmtNo_t stmt1, stmtNo_t stmt2) {
		Argument stmtNo1 = { StmtNo(std::to_string(stmt1)) };
		Argument stmtNo2 = { StmtNo(std::to_string(stmt2)) };
		return make_shared<DA::AffectsRel>(stmtNo1, stmtNo2);
	}
	shared_ptr<DA::AffectsRel> makeAffects(stmtNo_t stmt, string syn) {
		Argument stmtNo = { StmtNo(std::to_string(stmt)) };
		Argument s = { Declaration(make_shared<DE::Statement>(), syn) };
		return make_shared<DA::AffectsRel>(stmtNo, s);
	}
	shared_ptr<DA::AffectsRel> makeAffects(string syn, stmtNo_t stmt) {
		Argument s = { Declaration(make_shared<DE::Statement>(), syn) };
		Argument stmtNo = { StmtNo(std::to_string(stmt)) };
		return make_shared<DA::AffectsRel>(s, stmtNo);
	}
	shared_ptr<DA::AffectsRel> makeAffects(string syn1, string syn2) {
		Argument s1 = { Declaration(make_shared<DE::Statement>(), syn1) };
		Argument s2 = { Declaration(make_shared<DE::Statement>(), syn2) };
		return make_shared<DA::AffectsRel>(s1, s2);
	}
	shared_ptr<DA::AffectsRel> makeAffects(string syn, WildCard wildcard) {
		Argument s = { Declaration(make_shared<DE::Statement>(), syn) };
		Argument wc = { wildcard };
		return make_shared<DA::AffectsRel>(s, wc);
	}
	shared_ptr<DA::AffectsRel> makeAffects(WildCard wildcard, string syn) {
		Argument wc = { wildcard };
		Argument s = { Declaration(make_shared<DE::Statement>(), syn) };
		return make_shared<DA::AffectsRel>(wc, s);
	}
	shared_ptr<DA::AffectsRel> makeAffects(WildCard wildcard1, WildCard wildcard2) {
		Argument wc1 = { wildcard1 };
		Argument wc2 = { wildcard2 };
		return make_shared<DA::AffectsRel>(wc1, wc2);
	}
	shared_ptr<DA::AffectsRel> makeAffects(WildCard wildcard, stmtNo_t stmt) {
		Argument wc = { wildcard };
		Argument stmtNo = { StmtNo(std::to_string(stmt)) };
		return make_shared<DA::AffectsRel>(wc, stmtNo);
	}
	shared_ptr<DA::AffectsRel> makeAffects(stmtNo_t stmt, WildCard wildcard) {
		Argument stmtNo = { StmtNo(std::to_string(stmt)) };
		Argument wc = { wildcard };
		return make_shared<DA::AffectsRel>(stmtNo, wc);
	}

	void assertTableEqual(Table& expected, Table& actual) {
		auto expectedHeader = expected.getTableHeader();
		auto actualHeader = actual.getTableHeader();
		auto expectedRows = expected.getTableRows();
		auto actualRows = actual.getTableRows();
		
		Assert::IsTrue(expectedHeader == actualHeader);
		Assert::IsTrue(expectedRows.size() == expectedRows.size());
		if (expectedHeader.size() == 0) {
			Assert::IsTrue(expectedRows == actualRows);
		}
		else if (expectedHeader.size() == 1) {
			unordered_set<string> expectedRowSet;
			unordered_set<string> actualRowSet;
			for (const auto& column : expectedRows) expectedRowSet.insert(*column.cbegin());
			for (const auto& column : actualRows) actualRowSet.insert(*column.cbegin());

			Assert::IsTrue(expectedRowSet == actualRowSet);
		}
		else {
			unordered_set<pair<string, string>, stringPairHash> expectedRowSet;
			unordered_set<pair<string, string>, stringPairHash> actualRowSet;
			for (const auto& column : expectedRows) expectedRowSet.insert(pair(*column.cbegin(), *column.rbegin()));
			for (const auto& column : actualRows) actualRowSet.insert(pair(*column.cbegin(), *column.rbegin()));

			Assert::IsTrue(expectedRowSet == actualRowSet);
		}
	}

public:

	TEST_METHOD(TestBasicAffects) {
		auto source = "procedure proc {a=1;b=a;}";
		Context context = Context(source);
		auto getFacade = context.getFacade;

		auto affects1 = makeAffects(1, 2);
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(2, 2);
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(WildCard(), WildCard());
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(1, WildCard());
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(2, WildCard());
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(WildCard(), 2);
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(1, "s");
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects("s", 2);
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects("s1", "s2");
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects(WildCard(), "s");
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects("s", WildCard());
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);

		bool res1 = e1.evaluateClauseWithNoSyn();
		bool res2 = e2.evaluateClauseWithNoSyn();
		bool res3 = e3.evaluateClauseWithNoSyn();
		bool res4 = e4.evaluateClauseWithNoSyn();
		bool res5 = e5.evaluateClauseWithNoSyn();
		bool res6 = e6.evaluateClauseWithNoSyn();
		Table res7 = e7.evaluateClauseWithSyn();
		Table res8 = e8.evaluateClauseWithSyn();
		Table res9 = e9.evaluateClauseWithSyn();
		Table res10 = e10.evaluateClauseWithSyn();
		Table res11 = e11.evaluateClauseWithSyn();

		Assert::IsTrue(res1);
		Assert::IsFalse(res2);
		Assert::IsTrue(res3);
		Assert::IsTrue(res4);
		Assert::IsFalse(res5);
		Assert::IsTrue(res6);

		Table expected;
		expected = Table({ "s" }, { { "2" } });
		assertTableEqual(expected, res7);
		expected = Table({ "s" }, { { "1" } });
		assertTableEqual(expected, res8);
		expected = Table({ "s1", "s2" }, { {"1", "2"} });
		assertTableEqual(expected, res9);
		expected = Table({ "s" }, { { "2" } });
		assertTableEqual(expected, res10);
		expected = Table({ "s" }, { { "1" } });
		assertTableEqual(expected, res11);
	}

	TEST_METHOD(TestMultipleAffects) {
		auto source = "procedure proc {a=1; b=a; c=b; d=b; b=e; f=b;}";
		Context context = Context(source);
		auto getFacade = context.getFacade;

		auto affects1 = makeAffects(1, 2);
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(2, 3);
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(2, 4);
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(5, 6);
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(2, 6); // false
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(WildCard(), WildCard());
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(2, WildCard());
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects(3, WildCard()); // false
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects(WildCard(), 4);
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects(2, "s");
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects("s", 6);
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);
		auto affects12 = makeAffects("s1", "s2");
		auto e12 = AffectsEvaluator(affects12->arg1, affects12->arg2, getFacade);

		bool res1 = e1.evaluateClauseWithNoSyn();
		bool res2 = e2.evaluateClauseWithNoSyn();
		bool res3 = e3.evaluateClauseWithNoSyn();
		bool res4 = e4.evaluateClauseWithNoSyn();
		bool res5 = e5.evaluateClauseWithNoSyn();
		bool res6 = e6.evaluateClauseWithNoSyn();
		bool res7 = e7.evaluateClauseWithNoSyn();
		bool res8 = e8.evaluateClauseWithNoSyn();
		bool res9 = e9.evaluateClauseWithNoSyn();
		Table res10 = e10.evaluateClauseWithSyn();
		Table res11 = e11.evaluateClauseWithSyn();
		Table res12 = e12.evaluateClauseWithSyn();

		Assert::IsTrue(res1);
		Assert::IsTrue(res2);
		Assert::IsTrue(res3);
		Assert::IsTrue(res4);
		Assert::IsFalse(res5);
		Assert::IsTrue(res6);
		Assert::IsTrue(res7);
		Assert::IsFalse(res8);
		Assert::IsTrue(res9);

		Table expected;
		expected = Table({ "s" }, { { "3" }, { "4" } });
		assertTableEqual(expected, res10);
		expected = Table({ "s" }, { { "5" } });
		assertTableEqual(expected, res11);
		expected = Table({ "s1", "s2" }, { {"1", "2"}, {"2", "3"}, {"2", "4"}, {"5", "6"} });
		assertTableEqual(expected, res12);
	}

	TEST_METHOD(TestAffectsWithOtherStatements) {
		auto source = "procedure p { a=1; b=b; read c; call q; d=b; e=c; }"
					  "procedure q { b=3; }"; // no affects
		Context context = Context(source);
		auto getFacade = context.getFacade;

		auto affects1 = makeAffects(1, 2);
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(2, 5);
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(4, 5);
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(3, 6);
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(WildCard(), WildCard());
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(2, WildCard());
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(4, WildCard());
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects(WildCard(), 6);
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects(3, "s");
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects("s", 5);
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects("s1", "s2");
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);

		bool res1 = e1.evaluateClauseWithNoSyn();
		bool res2 = e2.evaluateClauseWithNoSyn();
		bool res3 = e3.evaluateClauseWithNoSyn();
		bool res4 = e4.evaluateClauseWithNoSyn();
		bool res5 = e5.evaluateClauseWithNoSyn();
		bool res6 = e6.evaluateClauseWithNoSyn();
		bool res7 = e7.evaluateClauseWithNoSyn();
		bool res8 = e8.evaluateClauseWithNoSyn();
		Table res9 = e9.evaluateClauseWithSyn();
		Table res10 = e10.evaluateClauseWithSyn();
		Table res11 = e11.evaluateClauseWithSyn();

		Assert::IsFalse(res1);
		Assert::IsFalse(res2);
		Assert::IsFalse(res3);
		Assert::IsFalse(res4);
		Assert::IsFalse(res5);
		Assert::IsFalse(res6);
		Assert::IsFalse(res7);
		Assert::IsFalse(res8);

		Table expected;
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res9);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res10);
		expected = Table({ "s1", "s2" }, { });
		assertTableEqual(expected, res11);
	}

	TEST_METHOD(TestWhileBoolean) {
		auto source = "procedure proc {"
						"a = 1;"			// 1
						"while (a == 0) {"	// 2
							"b = a;"		// 3
							"b = b;"		// 4
							"c = b;"		// 5
							"d = a + c;"	// 6
							"a = b + c;"	// 7
							"e = e + a;"	// 8
						"}"
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;
		// (1,3), (1,6), (1,8), (3,4), (4,5), (4,7), (5,6), (5,7), (7,8), (7,6), (8,8)
		auto affects1 = makeAffects(1, 3);
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(3, 4);
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(1, 6);
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(4, 7);
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(7, 6);
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(8, 8);
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(4, 4); // false
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects(1, 2); // false
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects(7, 2); // false
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects(WildCard(), WildCard());
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects(7, WildCard());
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);
		auto affects12 = makeAffects(8, WildCard());
		auto e12 = AffectsEvaluator(affects12->arg1, affects12->arg2, getFacade);
		auto affects13 = makeAffects(2, WildCard()); // false
		auto e13 = AffectsEvaluator(affects13->arg1, affects13->arg2, getFacade);
		auto affects14 = makeAffects(WildCard(), 2); // false
		auto e14 = AffectsEvaluator(affects14->arg1, affects14->arg2, getFacade);

		bool res1 = e1.evaluateClauseWithNoSyn();
		bool res2 = e2.evaluateClauseWithNoSyn();
		bool res3 = e3.evaluateClauseWithNoSyn();
		bool res4 = e4.evaluateClauseWithNoSyn();
		bool res5 = e5.evaluateClauseWithNoSyn();
		bool res6 = e6.evaluateClauseWithNoSyn();
		bool res7 = e7.evaluateClauseWithNoSyn();
		bool res8 = e8.evaluateClauseWithNoSyn();
		bool res9 = e9.evaluateClauseWithNoSyn();
		bool res10 = e10.evaluateClauseWithNoSyn();
		bool res11 = e11.evaluateClauseWithNoSyn();
		bool res12 = e12.evaluateClauseWithNoSyn();
		bool res13 = e13.evaluateClauseWithNoSyn();
		bool res14 = e14.evaluateClauseWithNoSyn();

		Assert::IsTrue(res1);
		Assert::IsTrue(res2);
		Assert::IsTrue(res3);
		Assert::IsTrue(res4);
		Assert::IsTrue(res5);
		Assert::IsTrue(res6);
		Assert::IsFalse(res7);
		Assert::IsFalse(res8);
		Assert::IsFalse(res9);
		Assert::IsTrue(res10);
		Assert::IsTrue(res11);
		Assert::IsTrue(res12);
		Assert::IsFalse(res13);
		Assert::IsFalse(res14);
	}

	TEST_METHOD(TestWhileResults) {
		auto source = "procedure proc {"
						"a = 1;"			// 1
						"while (a == 0) {"	// 2
							"b = a;"		// 3
							"b = b;"		// 4
							"c = b;"		// 5
							"d = a + c;"	// 6
							"a = b + c;"	// 7
							"e = e + a;"	// 8
						"}"
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;
		// (1,3), (1,6), (3,4), (4,5), (4,7), (5,6), (5,7), (7,8), (7,3), (7,6), (8,8)

		auto affects1 = makeAffects(1, "s");
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(8, "s");
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(2, "s"); // false
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects("s", 6);
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects("s", 8);
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects("s", 2); // false
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects("s1", "s2");
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);

		Table res1 = e1.evaluateClauseWithSyn();
		Table res2 = e2.evaluateClauseWithSyn();
		Table res3 = e3.evaluateClauseWithSyn();
		Table res4 = e4.evaluateClauseWithSyn();
		Table res5 = e5.evaluateClauseWithSyn();
		Table res6 = e6.evaluateClauseWithSyn();
		Table res7 = e7.evaluateClauseWithSyn();

		Table expected;
		expected = Table({ "s" }, { { "3" }, { "6" } });
		assertTableEqual(expected, res1);
		expected = Table({ "s" }, { { "8" } });
		assertTableEqual(expected, res2);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res3);
		expected = Table({ "s" }, { { "1" }, { "5" }, { "7" } });
		assertTableEqual(expected, res4);
		expected = Table({ "s" }, { { "7" }, { "8" } });
		assertTableEqual(expected, res5);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res6);
		expected = Table({ "s1", "s2" }, {
			{ "1", "3" },
			{ "1", "6" },
			{ "3", "4" },
			{ "4", "5" },
			{ "4", "7" },
			{ "5", "6" },
			{ "5", "7" },
			{ "7", "3" },
			{ "7", "8" },
			{ "7", "6" },
			{ "8", "8" },
			});
		assertTableEqual(expected, res7);
	}

	TEST_METHOD(TestNestedWhileBoolean) {
		auto source = "procedure proc {"
						"a = 1;"				// 1
						"while (a == 0) {"		// 2
							"b = a;"			// 3
							"c = c - b;"		// 4
							"while (b != c) {"	// 5
								"b = a + b;"	// 6
								"b = b;"		// 7
								"c = b;"		// 8
								"d = a + c;"	// 9
								"a = b + c;"	// 10
							"}"
							"e = e + a + b;"	// 11
						"}"
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;
		// (1,3), (1,6), (1,9), (1,11)
		// (3,4), (3,6), (3,11), (4,4), (6,7)
		// (7,6), (7,8), (7,10), (7,11)
		// (8,9), (8,10), (8,4)
		// (10,11), (10,6), (10,9), (10,3), (11,11)
		auto affects1 = makeAffects(1, 6);
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(1, 9);
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(1, 11);
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(3, 4);
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(3, 6);
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(3, 11);
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(4, 4);
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects(6, 7);
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects(7, 6);
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects(7, 8);
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects(7, 10);
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);
		auto affects12 = makeAffects(7, 11);
		auto e12 = AffectsEvaluator(affects12->arg1, affects12->arg2, getFacade);
		auto affects13 = makeAffects(8, 10);
		auto e13 = AffectsEvaluator(affects13->arg1, affects13->arg2, getFacade);
		auto affects14 = makeAffects(8, 4);
		auto e14 = AffectsEvaluator(affects14->arg1, affects14->arg2, getFacade);
		auto affects15 = makeAffects(10, 11);
		auto e15 = AffectsEvaluator(affects15->arg1, affects15->arg2, getFacade);
		auto affects16 = makeAffects(10, 6);
		auto e16 = AffectsEvaluator(affects16->arg1, affects16->arg2, getFacade);
		auto affects17 = makeAffects(10, 9);
		auto e17 = AffectsEvaluator(affects17->arg1, affects17->arg2, getFacade);
		auto affects18 = makeAffects(10, 3);
		auto e18 = AffectsEvaluator(affects18->arg1, affects18->arg2, getFacade);
		auto affects19 = makeAffects(11, 11);
		auto e19 = AffectsEvaluator(affects19->arg1, affects19->arg2, getFacade);
		auto affects20 = makeAffects(6, 6); // false
		auto e20 = AffectsEvaluator(affects20->arg1, affects20->arg2, getFacade);
		auto affects21 = makeAffects(7, 7); // false
		auto e21 = AffectsEvaluator(affects21->arg1, affects21->arg2, getFacade);
		auto affects22 = makeAffects(7, 4); // false
		auto e22 = AffectsEvaluator(affects22->arg1, affects22->arg2, getFacade);
		auto affects23 = makeAffects(WildCard(), WildCard());
		auto e23 = AffectsEvaluator(affects23->arg1, affects23->arg2, getFacade);
		auto affects24 = makeAffects(7, WildCard());
		auto e24 = AffectsEvaluator(affects24->arg1, affects24->arg2, getFacade);
		auto affects25 = makeAffects(10, WildCard());
		auto e25 = AffectsEvaluator(affects25->arg1, affects25->arg2, getFacade);
		auto affects26 = makeAffects(5, WildCard()); // false
		auto e26 = AffectsEvaluator(affects26->arg1, affects26->arg2, getFacade);
		auto affects27 = makeAffects(9, WildCard()); // false
		auto e27 = AffectsEvaluator(affects27->arg1, affects27->arg2, getFacade);
		auto affects28 = makeAffects(WildCard(), 6);
		auto e28 = AffectsEvaluator(affects28->arg1, affects28->arg2, getFacade);
		auto affects29 = makeAffects(WildCard(), 11);
		auto e29 = AffectsEvaluator(affects29->arg1, affects29->arg2, getFacade);
		auto affects30 = makeAffects(WildCard(), 2); // false
		auto e30 = AffectsEvaluator(affects30->arg1, affects30->arg2, getFacade);

		bool res1 = e1.evaluateClauseWithNoSyn();
		bool res2 = e2.evaluateClauseWithNoSyn();
		bool res3 = e3.evaluateClauseWithNoSyn();
		bool res4 = e4.evaluateClauseWithNoSyn();
		bool res5 = e5.evaluateClauseWithNoSyn();
		bool res6 = e6.evaluateClauseWithNoSyn();
		bool res7 = e7.evaluateClauseWithNoSyn();
		bool res8 = e8.evaluateClauseWithNoSyn();
		bool res9 = e9.evaluateClauseWithNoSyn();
		bool res10 = e10.evaluateClauseWithNoSyn();
		bool res11 = e11.evaluateClauseWithNoSyn();
		bool res12 = e12.evaluateClauseWithNoSyn();
		bool res13 = e13.evaluateClauseWithNoSyn();
		bool res14 = e14.evaluateClauseWithNoSyn();
		bool res15 = e15.evaluateClauseWithNoSyn();
		bool res16 = e16.evaluateClauseWithNoSyn();
		bool res17 = e17.evaluateClauseWithNoSyn();
		bool res18 = e18.evaluateClauseWithNoSyn();
		bool res19 = e19.evaluateClauseWithNoSyn();
		bool res20 = e20.evaluateClauseWithNoSyn();
		bool res21 = e21.evaluateClauseWithNoSyn();
		bool res22 = e22.evaluateClauseWithNoSyn();
		bool res23 = e23.evaluateClauseWithNoSyn();
		bool res24 = e24.evaluateClauseWithNoSyn();
		bool res25 = e25.evaluateClauseWithNoSyn();
		bool res26 = e26.evaluateClauseWithNoSyn();
		bool res27 = e27.evaluateClauseWithNoSyn();
		bool res28 = e28.evaluateClauseWithNoSyn();
		bool res29 = e29.evaluateClauseWithNoSyn();
		bool res30 = e30.evaluateClauseWithNoSyn();

		Assert::IsTrue(res1);
		Assert::IsTrue(res2);
		Assert::IsTrue(res3);
		Assert::IsTrue(res4);
		Assert::IsTrue(res5);
		Assert::IsTrue(res6);
		Assert::IsTrue(res7);
		Assert::IsTrue(res8);
		Assert::IsTrue(res9);
		Assert::IsTrue(res10);
		Assert::IsTrue(res11);
		Assert::IsTrue(res12);
		Assert::IsTrue(res13);
		Assert::IsTrue(res14);
		Assert::IsTrue(res15);
		Assert::IsTrue(res16);
		Assert::IsTrue(res17);
		Assert::IsTrue(res18);
		Assert::IsTrue(res19);
		Assert::IsFalse(res20);
		Assert::IsFalse(res21);
		Assert::IsFalse(res22);
		Assert::IsTrue(res23);
		Assert::IsTrue(res24);
		Assert::IsTrue(res25);
		Assert::IsFalse(res26);
		Assert::IsFalse(res27);
		Assert::IsTrue(res28);
		Assert::IsTrue(res29);
		Assert::IsFalse(res30);
	}

	TEST_METHOD(TestNestedWhileResults) {
		auto source = "procedure proc {"
						"a = 1;"				// 1
						"while (a == 0) {"		// 2
							"b = a;"			// 3
							"c = c - b;"		// 4
							"while (b != c) {"	// 5
								"b = a + b;"	// 6
								"b = b;"		// 7
								"c = b;"		// 8
								"d = a + c;"	// 9
								"a = b + c;"	// 10
							"}"
							"e = e + a + b;"	// 11
						"}"
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;
		// (1,3), (1,6), (1,9), (1,11)
		// (3,4), (3,6), (3,11), (4,4), (6,7)
		// (7,6), (7,8), (7,10), (7,11)
		// (8,9), (8,10), (8,4)
		// (10,11), (10,6), (10,9), (10,3), (11,11)
		auto affects1 = makeAffects(1, "s");
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(3, "s");
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(4, "s");
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(6, "s");
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(7, "s");
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(8, "s");
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(10, "s");
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects(11, "s");
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects(5, "s"); // empty
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects(9, "s"); // empty
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects("s", 3);
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);
		auto affects12 = makeAffects("s", 4);
		auto e12 = AffectsEvaluator(affects12->arg1, affects12->arg2, getFacade);
		auto affects13 = makeAffects("s", 6);
		auto e13 = AffectsEvaluator(affects13->arg1, affects13->arg2, getFacade);
		auto affects14 = makeAffects("s", 7);
		auto e14 = AffectsEvaluator(affects14->arg1, affects14->arg2, getFacade);
		auto affects15 = makeAffects("s", 8);
		auto e15 = AffectsEvaluator(affects15->arg1, affects15->arg2, getFacade);
		auto affects16 = makeAffects("s", 9);
		auto e16 = AffectsEvaluator(affects16->arg1, affects16->arg2, getFacade);
		auto affects17 = makeAffects("s", 10);
		auto e17 = AffectsEvaluator(affects17->arg1, affects17->arg2, getFacade);
		auto affects18 = makeAffects("s", 11);
		auto e18 = AffectsEvaluator(affects18->arg1, affects18->arg2, getFacade);
		auto affects19 = makeAffects("s", 1); // empty
		auto e19 = AffectsEvaluator(affects19->arg1, affects19->arg2, getFacade);
		auto affects20 = makeAffects("s", 2); // empty
		auto e20 = AffectsEvaluator(affects20->arg1, affects20->arg2, getFacade);
		auto affects21 = makeAffects("s1", "s2");
		auto e21 = AffectsEvaluator(affects21->arg1, affects21->arg2, getFacade);

		Table res1 = e1.evaluateClauseWithSyn();
		Table res2 = e2.evaluateClauseWithSyn();
		Table res3 = e3.evaluateClauseWithSyn();
		Table res4 = e4.evaluateClauseWithSyn();
		Table res5 = e5.evaluateClauseWithSyn();
		Table res6 = e6.evaluateClauseWithSyn();
		Table res7 = e7.evaluateClauseWithSyn();
		Table res8 = e8.evaluateClauseWithSyn();
		Table res9 = e9.evaluateClauseWithSyn();
		Table res10 = e10.evaluateClauseWithSyn();
		Table res11 = e11.evaluateClauseWithSyn();
		Table res12 = e12.evaluateClauseWithSyn();
		Table res13 = e13.evaluateClauseWithSyn();
		Table res14 = e14.evaluateClauseWithSyn();
		Table res15 = e15.evaluateClauseWithSyn();
		Table res16 = e16.evaluateClauseWithSyn();
		Table res17 = e17.evaluateClauseWithSyn();
		Table res18 = e18.evaluateClauseWithSyn();
		Table res19 = e19.evaluateClauseWithSyn();
		Table res20 = e20.evaluateClauseWithSyn();
		Table res21 = e21.evaluateClauseWithSyn();

		Table expected;
		expected = Table({ "s" }, { { "3" }, { "6" }, { "9" }, { "11" } });
		assertTableEqual(expected, res1);
		expected = Table({ "s" }, { { "4" }, { "6" }, { "11" } });
		assertTableEqual(expected, res2);
		expected = Table({ "s" }, { { "4" } });
		assertTableEqual(expected, res3);
		expected = Table({ "s" }, { { "7" } });
		assertTableEqual(expected, res4);
		expected = Table({ "s" }, { { "6" }, { "8" }, { "10" }, { "11" } });
		assertTableEqual(expected, res5);
		expected = Table({ "s" }, { { "9" }, { "10" }, { "4" } });
		assertTableEqual(expected, res6);
		expected = Table({ "s" }, { { "11" }, { "6" }, { "9" }, { "3" } });
		assertTableEqual(expected, res7);
		expected = Table({ "s" }, { { "11" } });
		assertTableEqual(expected, res8);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res9);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res10);
		expected = Table({ "s" }, { { "1" }, { "10" } });
		assertTableEqual(expected, res11);
		expected = Table({ "s" }, { { "3" }, { "4" }, { "8" } });
		assertTableEqual(expected, res12);
		expected = Table({ "s" }, { { "1" }, { "3" }, { "7" }, { "10" } });
		assertTableEqual(expected, res13);
		expected = Table({ "s" }, { { "6" } });
		assertTableEqual(expected, res14);
		expected = Table({ "s" }, { { "7" } });
		assertTableEqual(expected, res15);
		expected = Table({ "s" }, { { "1" }, { "8" }, { "10" } });
		assertTableEqual(expected, res16);
		expected = Table({ "s" }, { { "7" }, { "8" } });
		assertTableEqual(expected, res17);
		expected = Table({ "s" }, { { "1" }, { "3" }, { "7" }, { "10" }, { "11" } });
		assertTableEqual(expected, res18);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res19);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res20);
		expected = Table({ "s1", "s2" }, {
			{ "1", "3" },
			{ "1", "6" },
			{ "1", "9" },
			{ "1", "11" },
			{ "3", "4" },
			{ "3", "6" },
			{ "3", "11" },
			{ "4", "4" },
			{ "6", "7" },
			{ "7", "6" },
			{ "7", "8" },
			{ "7", "10" },
			{ "7", "11" },
			{ "8", "9" },
			{ "8", "10" },
			{ "8", "4" },
			{ "10", "11" },
			{ "10", "6" },
			{ "10", "9" },
			{ "10", "3" },
			{ "11", "11" },
			});
		assertTableEqual(expected, res21);
	}

	TEST_METHOD(TestNestedIfBoolean) {
		auto source = "procedure proc {"
						"a = 1;"					// 1
						"if (a == 0) then {"		// 2
							"b = a;"				// 3
							"c = c - b;"			// 4
						"} else {"
							"b = a + b;"			// 5
							"b = b;"				// 6
							"if (c == a) then {"	// 7
								"c = b;"			// 8
								"d = a + c;"		// 9
							"} else {"
								"a = b + c;"		// 10
							"}"
							"b = c + a;"			// 11
							"a = a;"				// 12
						"}"
						"e = e + a + b;"			// 13
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;
		// (1,3), (1,5), (1,11), (1,12), (1,13)
		// (3,4), (3,13), (5,6), (6,8), (6,10)
		// (8,9), (8,11), (10,11), (10,12)
		// (11,13), (12,13)
		auto affects1 = makeAffects(1, 3);
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(1, 5);
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(1, 11);
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(1, 12);
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(1, 13);
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(3, 4);
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(3, 13);
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects(5, 6);
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects(6, 8);
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects(6, 10);
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects(8, 9);
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);
		auto affects12 = makeAffects(8, 11);
		auto e12 = AffectsEvaluator(affects12->arg1, affects12->arg2, getFacade);
		auto affects13 = makeAffects(10, 11);
		auto e13 = AffectsEvaluator(affects13->arg1, affects13->arg2, getFacade);
		auto affects14 = makeAffects(10, 12);
		auto e14 = AffectsEvaluator(affects14->arg1, affects14->arg2, getFacade);
		auto affects15 = makeAffects(11, 13);
		auto e15 = AffectsEvaluator(affects15->arg1, affects15->arg2, getFacade);
		auto affects16 = makeAffects(12, 13);
		auto e16 = AffectsEvaluator(affects16->arg1, affects16->arg2, getFacade);
		auto affects17 = makeAffects(3, 5); // false
		auto e17 = AffectsEvaluator(affects17->arg1, affects17->arg2, getFacade);
		auto affects18 = makeAffects(4, 11); // false
		auto e18 = AffectsEvaluator(affects18->arg1, affects18->arg2, getFacade);
		auto affects19 = makeAffects(8, 10); // false
		auto e19 = AffectsEvaluator(affects19->arg1, affects19->arg2, getFacade);
		auto affects20 = makeAffects(WildCard(), WildCard());
		auto e20 = AffectsEvaluator(affects20->arg1, affects20->arg2, getFacade);
		auto affects21 = makeAffects(1, WildCard());
		auto e21 = AffectsEvaluator(affects21->arg1, affects21->arg2, getFacade);
		auto affects22 = makeAffects(3, WildCard());
		auto e22 = AffectsEvaluator(affects22->arg1, affects22->arg2, getFacade);
		auto affects23 = makeAffects(8, WildCard());
		auto e23 = AffectsEvaluator(affects23->arg1, affects23->arg2, getFacade);
		auto affects24 = makeAffects(10, WildCard());
		auto e24 = AffectsEvaluator(affects24->arg1, affects24->arg2, getFacade);
		auto affects25 = makeAffects(7, WildCard()); // false
		auto e25 = AffectsEvaluator(affects25->arg1, affects25->arg2, getFacade);
		auto affects26 = makeAffects(9, WildCard()); // false
		auto e26 = AffectsEvaluator(affects26->arg1, affects26->arg2, getFacade);
		auto affects27 = makeAffects(WildCard(), 5);
		auto e27 = AffectsEvaluator(affects27->arg1, affects27->arg2, getFacade);
		auto affects28 = makeAffects(WildCard(), 9);
		auto e28 = AffectsEvaluator(affects28->arg1, affects28->arg2, getFacade);
		auto affects29 = makeAffects(WildCard(), 12);
		auto e29 = AffectsEvaluator(affects29->arg1, affects29->arg2, getFacade);
		auto affects30 = makeAffects(WildCard(), 7); // false
		auto e30 = AffectsEvaluator(affects30->arg1, affects30->arg2, getFacade);

		bool res1 = e1.evaluateClauseWithNoSyn();
		bool res2 = e2.evaluateClauseWithNoSyn();
		bool res3 = e3.evaluateClauseWithNoSyn();
		bool res4 = e4.evaluateClauseWithNoSyn();
		bool res5 = e5.evaluateClauseWithNoSyn();
		bool res6 = e6.evaluateClauseWithNoSyn();
		bool res7 = e7.evaluateClauseWithNoSyn();
		bool res8 = e8.evaluateClauseWithNoSyn();
		bool res9 = e9.evaluateClauseWithNoSyn();
		bool res10 = e10.evaluateClauseWithNoSyn();
		bool res11 = e11.evaluateClauseWithNoSyn();
		bool res12 = e12.evaluateClauseWithNoSyn();
		bool res13 = e13.evaluateClauseWithNoSyn();
		bool res14 = e14.evaluateClauseWithNoSyn();
		bool res15 = e15.evaluateClauseWithNoSyn();
		bool res16 = e16.evaluateClauseWithNoSyn();
		bool res17 = e17.evaluateClauseWithNoSyn();
		bool res18 = e18.evaluateClauseWithNoSyn();
		bool res19 = e19.evaluateClauseWithNoSyn();
		bool res20 = e20.evaluateClauseWithNoSyn();
		bool res21 = e21.evaluateClauseWithNoSyn();
		bool res22 = e22.evaluateClauseWithNoSyn();
		bool res23 = e23.evaluateClauseWithNoSyn();
		bool res24 = e24.evaluateClauseWithNoSyn();
		bool res25 = e25.evaluateClauseWithNoSyn();
		bool res26 = e26.evaluateClauseWithNoSyn();
		bool res27 = e27.evaluateClauseWithNoSyn();
		bool res28 = e28.evaluateClauseWithNoSyn();
		bool res29 = e29.evaluateClauseWithNoSyn();
		bool res30 = e30.evaluateClauseWithNoSyn();

		Assert::IsTrue(res1);
		Assert::IsTrue(res2);
		Assert::IsTrue(res3);
		Assert::IsTrue(res4);
		Assert::IsTrue(res5);
		Assert::IsTrue(res6);
		Assert::IsTrue(res7);
		Assert::IsTrue(res8);
		Assert::IsTrue(res9);
		Assert::IsTrue(res10);
		Assert::IsTrue(res11);
		Assert::IsTrue(res12);
		Assert::IsTrue(res13);
		Assert::IsTrue(res14);
		Assert::IsTrue(res15);
		Assert::IsTrue(res16);
		Assert::IsFalse(res17);
		Assert::IsFalse(res18);
		Assert::IsFalse(res19);
		Assert::IsTrue(res20);
		Assert::IsTrue(res21);
		Assert::IsTrue(res22);
		Assert::IsTrue(res23);
		Assert::IsTrue(res24);
		Assert::IsFalse(res25);
		Assert::IsFalse(res26);
		Assert::IsTrue(res27);
		Assert::IsTrue(res28);
		Assert::IsTrue(res29);
		Assert::IsFalse(res30);
	}

	TEST_METHOD(TestNestedIfResults) {
		auto source = "procedure proc {"
						"a = 1;"					// 1
						"if (a == 0) then {"		// 2
							"b = a;"				// 3
							"c = c - b;"			// 4
						"} else {"
							"b = a + b;"			// 5
							"b = b;"				// 6
							"if (c == a) then {"	// 7
								"c = b;"			// 8
								"d = a + c;"		// 9
							"} else {"
								"a = b + c;"		// 10
							"}"
							"b = c + a;"			// 11
							"a = a;"				// 12
						"}"
						"e = e + a + b;"			// 13
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;
		// (1,3), (1,5), (1,9), (1,11), (1,12), (1,13)
		// (3,4), (3,13), (5,6), (6,8), (6,10)
		// (8,9), (8,11), (10,11), (10,12)
		// (11,13), (12,13)
		auto affects1 = makeAffects(1, "s");
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(3, "s");
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(5, "s");
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(6, "s");
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(8, "s");
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(10, "s");
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(11, "s");
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects(12, "s");
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects(4, "s"); // empty
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects(7, "s"); // empty
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects("s", 3);
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);
		auto affects12 = makeAffects("s", 4);
		auto e12 = AffectsEvaluator(affects12->arg1, affects12->arg2, getFacade);
		auto affects13 = makeAffects("s", 5);
		auto e13 = AffectsEvaluator(affects13->arg1, affects13->arg2, getFacade);
		auto affects14 = makeAffects("s", 6);
		auto e14 = AffectsEvaluator(affects14->arg1, affects14->arg2, getFacade);
		auto affects15 = makeAffects("s", 8);
		auto e15 = AffectsEvaluator(affects15->arg1, affects15->arg2, getFacade);
		auto affects16 = makeAffects("s", 9);
		auto e16 = AffectsEvaluator(affects16->arg1, affects16->arg2, getFacade);
		auto affects17 = makeAffects("s", 10);
		auto e17 = AffectsEvaluator(affects17->arg1, affects17->arg2, getFacade);
		auto affects18 = makeAffects("s", 11);
		auto e18 = AffectsEvaluator(affects18->arg1, affects18->arg2, getFacade);
		auto affects19 = makeAffects("s", 12);
		auto e19 = AffectsEvaluator(affects19->arg1, affects19->arg2, getFacade);
		auto affects20 = makeAffects("s", 13);
		auto e20 = AffectsEvaluator(affects20->arg1, affects20->arg2, getFacade);
		auto affects21 = makeAffects("s", 2); // empty
		auto e21 = AffectsEvaluator(affects21->arg1, affects21->arg2, getFacade);
		auto affects22 = makeAffects("s", 7); // empty
		auto e22 = AffectsEvaluator(affects22->arg1, affects22->arg2, getFacade);
		auto affects23 = makeAffects("s1", "s2");
		auto e23 = AffectsEvaluator(affects23->arg1, affects23->arg2, getFacade);

		Table res1 = e1.evaluateClauseWithSyn();
		Table res2 = e2.evaluateClauseWithSyn();
		Table res3 = e3.evaluateClauseWithSyn();
		Table res4 = e4.evaluateClauseWithSyn();
		Table res5 = e5.evaluateClauseWithSyn();
		Table res6 = e6.evaluateClauseWithSyn();
		Table res7 = e7.evaluateClauseWithSyn();
		Table res8 = e8.evaluateClauseWithSyn();
		Table res9 = e9.evaluateClauseWithSyn();
		Table res10 = e10.evaluateClauseWithSyn();
		Table res11 = e11.evaluateClauseWithSyn();
		Table res12 = e12.evaluateClauseWithSyn();
		Table res13 = e13.evaluateClauseWithSyn();
		Table res14 = e14.evaluateClauseWithSyn();
		Table res15 = e15.evaluateClauseWithSyn();
		Table res16 = e16.evaluateClauseWithSyn();
		Table res17 = e17.evaluateClauseWithSyn();
		Table res18 = e18.evaluateClauseWithSyn();
		Table res19 = e19.evaluateClauseWithSyn();
		Table res20 = e20.evaluateClauseWithSyn();
		Table res21 = e21.evaluateClauseWithSyn();
		Table res22 = e22.evaluateClauseWithSyn();
		Table res23 = e23.evaluateClauseWithSyn();

		Table expected;
		expected = Table({ "s" }, { { "3" }, { "5" }, { "9" }, { "11" }, { "12" }, { "13" } });
		assertTableEqual(expected, res1);
		expected = Table({ "s" }, { { "4" }, { "13" } });
		assertTableEqual(expected, res2);
		expected = Table({ "s" }, { { "6" } });
		assertTableEqual(expected, res3);
		expected = Table({ "s" }, { { "8" }, { "10" } });
		assertTableEqual(expected, res4);
		expected = Table({ "s" }, { { "9" }, { "11" } });
		assertTableEqual(expected, res5);
		expected = Table({ "s" }, { { "11" }, { "12" } });
		assertTableEqual(expected, res6);
		expected = Table({ "s" }, { { "13" } });
		assertTableEqual(expected, res7);
		expected = Table({ "s" }, { { "13" } });
		assertTableEqual(expected, res8);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res9);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res10);
		expected = Table({ "s" }, { { "1" } });
		assertTableEqual(expected, res11);
		expected = Table({ "s" }, { { "3" } });
		assertTableEqual(expected, res12);
		expected = Table({ "s" }, { { "1" } });
		assertTableEqual(expected, res13);
		expected = Table({ "s" }, { { "5" } });
		assertTableEqual(expected, res14);
		expected = Table({ "s" }, { { "6" } });
		assertTableEqual(expected, res15);
		expected = Table({ "s" }, { { "1" }, { "8" } });
		assertTableEqual(expected, res16);
		expected = Table({ "s" }, { { "6" } });
		assertTableEqual(expected, res17);
		expected = Table({ "s" }, { { "1" }, { "8" }, { "10" } });
		assertTableEqual(expected, res18);
		expected = Table({ "s" }, { { "1" }, { "10" } });
		assertTableEqual(expected, res19);
		expected = Table({ "s" }, { { "1" }, { "3" }, { "11" }, { "12" } });
		assertTableEqual(expected, res20);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res21);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res22);
		expected = Table({ "s1", "s2" }, {
			{ "1", "3" },
			{ "1", "5" },
			{ "1", "9" },
			{ "1", "11" },
			{ "1", "12" },
			{ "1", "13" },
			{ "3", "4" },
			{ "3", "13" },
			{ "5", "6" },
			{ "6", "8" },
			{ "6", "10" },
			{ "8", "9" },
			{ "8", "11" },
			{ "10", "11" },
			{ "10", "12" },
			{ "11", "13" },
			{ "12", "13" },
			});
		assertTableEqual(expected, res23);
	}

	TEST_METHOD(TestWhileIf) {
		auto source = "procedure p {"
						"a = b * c + d;"
						"while(c > 0) {"
							"if (d > 0) then {"
								"b = a - 1;"
							"} else {"
								"c = b * d;"	//5
							"}"
							"d = b + c;"	//6
							"c = d * a;"	//7
							"b = d - c;"	//8
						"}"
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;


		auto affects = makeAffects("s1", "s2");
		auto e = AffectsEvaluator(affects->arg1, affects->arg2, getFacade);
		Table expected = e.evaluateClauseWithSyn();
		auto actual = Table({ "s1", "s2" }, {
			{ "1", "4" },
			{ "4", "6" },
			{ "5", "6" },
			{ "1", "7" },
			{ "6", "7" },
			{ "6", "8" },
			{ "7", "8" },
			{ "8", "5" },// 6:5
			{ "6", "5" },
			{ "8", "6" },
			{ "7", "6" },
			});
		assertTableEqual(expected, actual);
	}

	TEST_METHOD(TestFullProgramBoolean) {
		auto source = "procedure p {"
						"a = b * c + d;"			// 1
						"print a;"					// 2
						"while(c > 0) {"			// 3
							"if (d > 0) then {"		// 4
								"b = a - 1;"		// 5
								"read a;"			// 6
							"} else {"
								"c = b * d;"		// 7
								"while (a == b) {"	// 8
									"a = a + c;"	// 9
									"call q;"		// 10
									"d = b + a;"	// 11
								"}"
							"}"
							"print a;"				// 12
							"read c;"				// 13
							"d = b + c;"			// 14
							"c = d * a;"			// 15
							"b = d - c;"			// 16
						"}"
					  "}"
					  "procedure q {"
						"print a;"					// 17
						"read b;"					// 18
						"c = b;"					// 19
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;

		// (1,5), (1,9), (1,15), (5,14), (7,9), (9,9), (9,11), (9,15), (9,5)
		// (14,15), (14,16), (14,7), (15,16), (16,7), (16,14)

		// False: (7,14), (7,16), (9,12), (11,15), (15,9), (16,11)
		auto affects1 = makeAffects(1, 5);
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(1, 9);
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(1, 15);
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(5, 14);
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(7, 9);
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(9, 9);
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(9, 11);
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects(9, 15);
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects(9, 5);
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects(14, 15);
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects(14, 16);
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);
		auto affects12 = makeAffects(14, 7);
		auto e12 = AffectsEvaluator(affects12->arg1, affects12->arg2, getFacade);
		auto affects13 = makeAffects(15, 16);
		auto e13 = AffectsEvaluator(affects13->arg1, affects13->arg2, getFacade);
		auto affects14 = makeAffects(16, 7);
		auto e14 = AffectsEvaluator(affects14->arg1, affects14->arg2, getFacade);
		auto affects15 = makeAffects(16, 14);
		auto e15 = AffectsEvaluator(affects15->arg1, affects15->arg2, getFacade);
		auto affects16 = makeAffects(7, 14); // false
		auto e16 = AffectsEvaluator(affects16->arg1, affects16->arg2, getFacade);
		auto affects17 = makeAffects(7, 16); // false
		auto e17 = AffectsEvaluator(affects17->arg1, affects17->arg2, getFacade);
		auto affects18 = makeAffects(9, 12); // false
		auto e18 = AffectsEvaluator(affects18->arg1, affects18->arg2, getFacade);
		auto affects19 = makeAffects(11, 15); // false
		auto e19 = AffectsEvaluator(affects19->arg1, affects19->arg2, getFacade);
		auto affects20 = makeAffects(15, 9); // false
		auto e20 = AffectsEvaluator(affects20->arg1, affects20->arg2, getFacade);
		auto affects21 = makeAffects(16, 11); // false
		auto e21 = AffectsEvaluator(affects21->arg1, affects21->arg2, getFacade);
		auto affects22 = makeAffects(WildCard(), WildCard());
		auto e22 = AffectsEvaluator(affects22->arg1, affects22->arg2, getFacade);
		auto affects23 = makeAffects(1, WildCard());
		auto e23 = AffectsEvaluator(affects23->arg1, affects23->arg2, getFacade);
		auto affects24 = makeAffects(5, WildCard());
		auto e24 = AffectsEvaluator(affects24->arg1, affects24->arg2, getFacade);
		auto affects25 = makeAffects(7, WildCard());
		auto e25 = AffectsEvaluator(affects25->arg1, affects25->arg2, getFacade);
		auto affects26 = makeAffects(9, WildCard());
		auto e26 = AffectsEvaluator(affects26->arg1, affects26->arg2, getFacade);
		auto affects27 = makeAffects(14, WildCard());
		auto e27 = AffectsEvaluator(affects27->arg1, affects27->arg2, getFacade);
		auto affects28 = makeAffects(15, WildCard());
		auto e28 = AffectsEvaluator(affects28->arg1, affects28->arg2, getFacade);
		auto affects29 = makeAffects(16, WildCard());
		auto e29 = AffectsEvaluator(affects29->arg1, affects29->arg2, getFacade);
		auto affects30 = makeAffects(11, WildCard()); // false
		auto e30 = AffectsEvaluator(affects30->arg1, affects30->arg2, getFacade);
		auto affects31 = makeAffects(WildCard(), 9);
		auto e31 = AffectsEvaluator(affects31->arg1, affects31->arg2, getFacade);
		auto affects32 = makeAffects(WildCard(), 15);
		auto e32 = AffectsEvaluator(affects32->arg1, affects32->arg2, getFacade);
		auto affects33 = makeAffects(WildCard(), 16);
		auto e33 = AffectsEvaluator(affects33->arg1, affects33->arg2, getFacade);
		auto affects34 = makeAffects(WildCard(), 1); // false
		auto e34 = AffectsEvaluator(affects34->arg1, affects34->arg2, getFacade);
		auto affects35 = makeAffects(WildCard(), 3); // false
		auto e35 = AffectsEvaluator(affects35->arg1, affects35->arg2, getFacade);

		bool res1 = e1.evaluateClauseWithNoSyn();
		bool res2 = e2.evaluateClauseWithNoSyn();
		bool res3 = e3.evaluateClauseWithNoSyn();
		bool res4 = e4.evaluateClauseWithNoSyn();
		bool res5 = e5.evaluateClauseWithNoSyn();
		bool res6 = e6.evaluateClauseWithNoSyn();
		bool res7 = e7.evaluateClauseWithNoSyn();
		bool res8 = e8.evaluateClauseWithNoSyn();
		bool res9 = e9.evaluateClauseWithNoSyn();
		bool res10 = e10.evaluateClauseWithNoSyn();
		bool res11 = e11.evaluateClauseWithNoSyn();
		bool res12 = e12.evaluateClauseWithNoSyn();
		bool res13 = e13.evaluateClauseWithNoSyn();
		bool res14 = e14.evaluateClauseWithNoSyn();
		bool res15 = e15.evaluateClauseWithNoSyn();
		bool res16 = e16.evaluateClauseWithNoSyn();
		bool res17 = e17.evaluateClauseWithNoSyn();
		bool res18 = e18.evaluateClauseWithNoSyn();
		bool res19 = e19.evaluateClauseWithNoSyn();
		bool res20 = e20.evaluateClauseWithNoSyn();
		bool res21 = e21.evaluateClauseWithNoSyn();
		bool res22 = e22.evaluateClauseWithNoSyn();
		bool res23 = e23.evaluateClauseWithNoSyn();
		bool res24 = e24.evaluateClauseWithNoSyn();
		bool res25 = e25.evaluateClauseWithNoSyn();
		bool res26 = e26.evaluateClauseWithNoSyn();
		bool res27 = e27.evaluateClauseWithNoSyn();
		bool res28 = e28.evaluateClauseWithNoSyn();
		bool res29 = e29.evaluateClauseWithNoSyn();
		bool res30 = e30.evaluateClauseWithNoSyn();
		bool res31 = e31.evaluateClauseWithNoSyn();
		bool res32 = e32.evaluateClauseWithNoSyn();
		bool res33 = e33.evaluateClauseWithNoSyn();
		bool res34 = e34.evaluateClauseWithNoSyn();
		bool res35 = e35.evaluateClauseWithNoSyn();

		Assert::IsTrue(res1);
		Assert::IsTrue(res2);
		Assert::IsTrue(res3);
		Assert::IsTrue(res4);
		Assert::IsTrue(res5);
		Assert::IsTrue(res6);
		Assert::IsTrue(res7);
		Assert::IsTrue(res8);
		Assert::IsTrue(res9);
		Assert::IsTrue(res10);
		Assert::IsTrue(res11);
		Assert::IsTrue(res12);
		Assert::IsTrue(res13);
		Assert::IsTrue(res14);
		Assert::IsTrue(res15);
		Assert::IsFalse(res16);
		Assert::IsFalse(res17);
		Assert::IsFalse(res18);
		Assert::IsFalse(res19);
		Assert::IsFalse(res20);
		Assert::IsFalse(res21);
		Assert::IsTrue(res22);
		Assert::IsTrue(res23);
		Assert::IsTrue(res24);
		Assert::IsTrue(res25);
		Assert::IsTrue(res26);
		Assert::IsTrue(res27);
		Assert::IsTrue(res28);
		Assert::IsTrue(res29);
		Assert::IsFalse(res30);
		Assert::IsTrue(res31);
		Assert::IsTrue(res32);
		Assert::IsTrue(res33);
		Assert::IsFalse(res34);
		Assert::IsFalse(res35);
	}

	TEST_METHOD(TestFullProgramResults) {
		auto source = "procedure p {"
						"a = b * c + d;"			// 1
						"print a;"					// 2
						"while(c > 0) {"			// 3
							"if (d > 0) then {"		// 4
								"b = a - 1;"		// 5
								"read a;"			// 6
							"} else {"
								"c = b * d;"		// 7
								"while (a == b) {"	// 8
									"a = a + c;"	// 9
									"call q;"		// 10
									"d = b + a;"	// 11
								"}"
							"}"
							"print a;"				// 12
							"read c;"				// 13
							"d = b + c;"			// 14
							"c = d * a;"			// 15
							"b = d - c;"			// 16
						"}"
					  "}"
					  "procedure q {"
						"print a;"					// 17
						"read b;"					// 18
						"c = b;"					// 19
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;

		// (1,5), (1,9), (1,15), (5,14), (7,9), (9,9), (9,11), (9,15), (9,5)
		// (14,15), (14,16), (14,7), (15,16), (16,7), 
		// (16,14) modify b--> 3, else, skip while,12,13,14

		// False: (1,12), (7,14), (7,16), (9,12), (11,15), (15,9), (16,11)
		auto affects1 = makeAffects(1, "s");
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(5, "s");
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(7, "s");
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(9, "s");
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(14, "s");
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(15, "s");
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(16, "s");
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects("s", 5);
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects("s", 7);
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects("s", 9);
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects("s", 11);
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);
		auto affects12 = makeAffects("s", 14);
		auto e12 = AffectsEvaluator(affects12->arg1, affects12->arg2, getFacade);
		auto affects13 = makeAffects("s", 15);
		auto e13 = AffectsEvaluator(affects13->arg1, affects13->arg2, getFacade);
		auto affects14 = makeAffects("s", 16);
		auto e14 = AffectsEvaluator(affects14->arg1, affects14->arg2, getFacade);
		auto affects15 = makeAffects("s1", "s2");
		auto e15 = AffectsEvaluator(affects15->arg1, affects15->arg2, getFacade);

		Table res1 = e1.evaluateClauseWithSyn();
		Table res2 = e2.evaluateClauseWithSyn();
		Table res3 = e3.evaluateClauseWithSyn();
		Table res4 = e4.evaluateClauseWithSyn();
		Table res5 = e5.evaluateClauseWithSyn();
		Table res6 = e6.evaluateClauseWithSyn();
		Table res7 = e7.evaluateClauseWithSyn();
		Table res8 = e8.evaluateClauseWithSyn();
		Table res9 = e9.evaluateClauseWithSyn();
		Table res10 = e10.evaluateClauseWithSyn();
		Table res11 = e11.evaluateClauseWithSyn();
		Table res12 = e12.evaluateClauseWithSyn();
		Table res13 = e13.evaluateClauseWithSyn();
		Table res14 = e14.evaluateClauseWithSyn();
		Table res15 = e15.evaluateClauseWithSyn();

		Table expected;
		expected = Table({ "s" }, { { "5" }, { "9" }, { "15" } });
		assertTableEqual(expected, res1);
		expected = Table({ "s" }, { { "14" } });
		assertTableEqual(expected, res2);
		expected = Table({ "s" }, { { "9" } });
		assertTableEqual(expected, res3);
		expected = Table({ "s" }, { { "5" }, { "9" }, { "11" }, { "15" } });
		assertTableEqual(expected, res4);
		expected = Table({ "s" }, { { "7" }, { "15" }, { "16" } });
		assertTableEqual(expected, res5);
		expected = Table({ "s" }, { { "16" } });
		assertTableEqual(expected, res6);
		expected = Table({ "s" }, { { "7"}, {"14"} });
		assertTableEqual(expected, res7);
		expected = Table({ "s" }, { { "1" }, { "9" } });
		assertTableEqual(expected, res8);
		expected = Table({ "s" }, { { "14" }, { "16" } });
		assertTableEqual(expected, res9);
		expected = Table({ "s" }, { { "1" }, { "7" }, { "9" } });
		assertTableEqual(expected, res10);
		expected = Table({ "s" }, { { "9" } });
		assertTableEqual(expected, res11);
		expected = Table({ "s" }, { { "5" }, {"16"} });
		assertTableEqual(expected, res12);
		expected = Table({ "s" }, { { "1" }, { "9" }, { "14" } });
		assertTableEqual(expected, res13);
		expected = Table({ "s" }, { { "14" }, { "15" } });
		assertTableEqual(expected, res14);
		expected = Table({ "s1", "s2" }, {
			{ "1", "5" },
			{ "1", "9" },
			{ "1", "15" },
			{ "5", "14" },
			{ "7", "9" },
			{ "9", "5" },
			{ "9", "9" },
			{ "9", "11" },
			{ "9", "15" },
			{ "14", "7" },
			{ "14", "15" },
			{ "14", "16" },
			{ "15", "16" },
			{ "16", "7" },
			{ "16", "14" },
			});
		assertTableEqual(expected, res15);
	}

	TEST_METHOD(TestInvalidAffects) {
		auto source = "procedure p {"
						"a = 1;"			// 1
						"b = 1;"			// 2
						"c = 1;"			// 3
						"call q;"			// 4
						"read a;"			// 5
						"c = 2;"			// 6
						"d = a + b + c;"	// 7
					  "}"
					  "procedure q {"
						"b = a;"			// 8
					  "}";
		Context context = Context(source);
		auto getFacade = context.getFacade;

		auto affects1 = makeAffects(1, 7); // a modified by read
		auto e1 = AffectsEvaluator(affects1->arg1, affects1->arg2, getFacade);
		auto affects2 = makeAffects(1, 8); // different procedures
		auto e2 = AffectsEvaluator(affects2->arg1, affects2->arg2, getFacade);
		auto affects3 = makeAffects(1, 10); // out of range
		auto e3 = AffectsEvaluator(affects3->arg1, affects3->arg2, getFacade);
		auto affects4 = makeAffects(10, 8); // out of range
		auto e4 = AffectsEvaluator(affects4->arg1, affects4->arg2, getFacade);
		auto affects5 = makeAffects(2, 7); // modified by call
		auto e5 = AffectsEvaluator(affects5->arg1, affects5->arg2, getFacade);
		auto affects6 = makeAffects(3, 7); // modified by assign
		auto e6 = AffectsEvaluator(affects6->arg1, affects6->arg2, getFacade);
		auto affects7 = makeAffects(1, WildCard()); // modified by read
		auto e7 = AffectsEvaluator(affects7->arg1, affects7->arg2, getFacade);
		auto affects8 = makeAffects(2, WildCard()); // modified by call
		auto e8 = AffectsEvaluator(affects8->arg1, affects8->arg2, getFacade);
		auto affects9 = makeAffects(3, WildCard()); // modified by assign
		auto e9 = AffectsEvaluator(affects9->arg1, affects9->arg2, getFacade);
		auto affects10 = makeAffects(10, WildCard()); // out of range
		auto e10 = AffectsEvaluator(affects10->arg1, affects10->arg2, getFacade);
		auto affects11 = makeAffects(WildCard(), 8); // different procedures
		auto e11 = AffectsEvaluator(affects11->arg1, affects11->arg2, getFacade);
		auto affects12 = makeAffects(WildCard(), 10); // out of range
		auto e12 = AffectsEvaluator(affects12->arg1, affects12->arg2, getFacade);
		auto affects13 = makeAffects("s", 7); // only 6
		auto e13 = AffectsEvaluator(affects13->arg1, affects13->arg2, getFacade);
		auto affects14 = makeAffects("s", 10); // out of range
		auto e14 = AffectsEvaluator(affects14->arg1, affects14->arg2, getFacade);
		auto affects15 = makeAffects(1, "s"); // modified by read
		auto e15 = AffectsEvaluator(affects15->arg1, affects15->arg2, getFacade);
		auto affects16 = makeAffects(2, "s"); // modified by call
		auto e16 = AffectsEvaluator(affects16->arg1, affects16->arg2, getFacade);
		auto affects17 = makeAffects(3, "s"); // modified by assign
		auto e17 = AffectsEvaluator(affects17->arg1, affects17->arg2, getFacade);
		auto affects18 = makeAffects(10, "s"); // out of range
		auto e18 = AffectsEvaluator(affects18->arg1, affects18->arg2, getFacade);
		auto affects19 = makeAffects("s1", "s2"); // only (6,7)
		auto e19 = AffectsEvaluator(affects19->arg1, affects19->arg2, getFacade);

		bool res1 = e1.evaluateClauseWithNoSyn();
		bool res2 = e2.evaluateClauseWithNoSyn();
		bool res3 = e3.evaluateClauseWithNoSyn();
		bool res4 = e4.evaluateClauseWithNoSyn();
		bool res5 = e5.evaluateClauseWithNoSyn();
		bool res6 = e6.evaluateClauseWithNoSyn();
		bool res7 = e7.evaluateClauseWithNoSyn();
		bool res8 = e8.evaluateClauseWithNoSyn();
		bool res9 = e9.evaluateClauseWithNoSyn();
		bool res10 = e10.evaluateClauseWithNoSyn();
		bool res11 = e11.evaluateClauseWithNoSyn();
		bool res12 = e12.evaluateClauseWithNoSyn();
		Table res13 = e13.evaluateClauseWithSyn();
		Table res14 = e14.evaluateClauseWithSyn();
		Table res15 = e15.evaluateClauseWithSyn();
		Table res16 = e16.evaluateClauseWithSyn();
		Table res17 = e17.evaluateClauseWithSyn();
		Table res18 = e18.evaluateClauseWithSyn();
		Table res19 = e19.evaluateClauseWithSyn();

		Assert::IsFalse(res1);
		Assert::IsFalse(res2);
		Assert::IsFalse(res3);
		Assert::IsFalse(res4);
		Assert::IsFalse(res5);
		Assert::IsFalse(res6);
		Assert::IsFalse(res7);
		Assert::IsFalse(res8);
		Assert::IsFalse(res9);
		Assert::IsFalse(res10);
		Assert::IsFalse(res11);
		Assert::IsFalse(res12);

		Table expected;
		expected = Table({ "s" }, { { "6" } });
		assertTableEqual(expected, res13);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res14);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res15);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res16);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res17);
		expected = Table({ "s" }, { });
		assertTableEqual(expected, res18);
		expected = Table({ "s1", "s2" }, {
			{ "6", "7" },
			});
		assertTableEqual(expected, res19);
	}
	};
}