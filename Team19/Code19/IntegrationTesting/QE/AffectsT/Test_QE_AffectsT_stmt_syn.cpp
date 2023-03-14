#include "stdafx.h"
#include "CppUnitTest.h"
#include <unordered_set>

#include "../AffectsContext.h"
#include "../source/QP/QueryEvaluator/AffectsTEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Test_QE_AffectsT {
	TEST_CLASS(Test_QEAffectsT_stmt_syn) {


		shared_ptr<DA::AffectsRel> makeAffects(stmtNo_t stmt) {
			Argument stmtNo = { StmtNo(std::to_string(stmt)) };
			Argument s = { Declaration(make_shared<DE::Statement>(), "syn") };
			return make_shared<DA::AffectsRel>(stmtNo, s);
		}

		Table makeTable(RelationshipResults result) {
			return QueryUtil::createTableArg2("syn", result);
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
			} else if (expectedHeader.size() == 1) {
				unordered_set<string> expectedRowSet;
				unordered_set<string> actualRowSet;
				for (const auto& column : expectedRows) expectedRowSet.insert(*column.cbegin());
				for (const auto& column : actualRows) actualRowSet.insert(*column.cbegin());

				Assert::IsTrue(expectedRowSet == actualRowSet);
			} else {
				unordered_set<pair<string, string>, stringPairHash> expectedRowSet;
				unordered_set<pair<string, string>, stringPairHash> actualRowSet;
				for (const auto& column : expectedRows) expectedRowSet.insert(pair(*column.cbegin(), *column.rbegin()));
				for (const auto& column : actualRows) actualRowSet.insert(pair(*column.cbegin(), *column.rbegin()));

				Assert::IsTrue(expectedRowSet == actualRowSet);
			}
		}
public:

	TEST_METHOD(Test_direct_affect) {
		Context context = Context::direct();
		auto getFacade = context.getFacade;
		{
			auto affects = makeAffects(1);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( { {"1","2"} });
			assertTableEqual(expected, actual);
		}
	}

	TEST_METHOD(Test_carried_affect) {
		Context context = Context::carried();
		auto getFacade = context.getFacade;
		{
			auto affects = makeAffects(1);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( { {"1","2"},{"1","3"} });
			assertTableEqual(expected, actual);
		}
		{
			auto affects = makeAffects(2);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( { {"2","3"} });
			assertTableEqual(expected, actual);
		}
	}

	TEST_METHOD(Test_self) {
		Context context = Context::self();
		auto getFacade = context.getFacade;
		{
			auto affects = makeAffects(2);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( { {"2","2"}});
			assertTableEqual(expected, actual);

		}
	}

	TEST_METHOD(Test_while) {
		Context context = Context::three_rounds();
		auto getFacade = context.getFacade;
		{
			auto affects = makeAffects(2);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( {});
			assertTableEqual(expected, actual);

			affects = makeAffects(3);
			actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			expected = makeTable({ {"3","2"}});
			assertTableEqual(expected, actual);

			affects = makeAffects(4);
			actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			expected = makeTable({ {"4","2"} , {"4","3"} });
			assertTableEqual(expected, actual);
		}
	}

	TEST_METHOD(Test_blocked_read) {
		Context context = Context::blocked_read();
		auto getFacade = context.getFacade;
		{
			auto affects = makeAffects(1);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( {});
			assertTableEqual(expected, actual);
		}
	}

	TEST_METHOD(Test_unblocked_print) {
		Context context = Context::unblocked_print();
		auto getFacade = context.getFacade;
		{
			auto affects = makeAffects(1);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( { {"1","3"},{"1","4"} });
			assertTableEqual(expected, actual);
		}
	}
	TEST_METHOD(Test_blocked_call) {
		Context context = Context::blocked_call();
		auto getFacade = context.getFacade;
		{
			auto affects = makeAffects(1);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( {});
			assertTableEqual(expected, actual);
		}
	}

	TEST_METHOD(Test_unblocked_call) {
		Context context = Context::unblocked_call();
		auto getFacade = context.getFacade;
		{
			auto affects = makeAffects(1);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( { {"1","3"},{"1","4"}, });
			assertTableEqual(expected, actual);
		}
	}

	TEST_METHOD(Test_carried_while) {
		Context context = Context::spiral_while();
		auto getFacade = context.getFacade;
		{
			auto affects = makeAffects(6);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( {
				{"6","2"},{"6","3"},{"6","5"},{"6","7"},{"6","8"},
				});
			assertTableEqual(expected, actual);
		}
	}

	TEST_METHOD(Test_stmt_syn_whileBackNest) {
		auto source =
			"procedure proc {"
			"while (a == a){ "	//1
			"	c=b;"			//2	
			"	while(a==a){"	//3
			"		c = b; "	//4
			"		b = a;"		//5
			"		a = 1;"		//6
			"	} "
			"}"
			"read b; "			//7
			"c=b;"				//8
			"}";
		Context context = Context(source);
		auto getFacade = context.getFacade;

		{
			auto affects = makeAffects(7);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( {});
			assertTableEqual(expected, actual);
		}
		{
			auto affects = makeAffects(6);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( { {"6","2"},{"6","4"},{"6","5"}, });
			assertTableEqual(expected, actual);
		}
		{
			auto affects = makeAffects(5);
			Table actual = AffectsTEvaluator(affects->arg1, affects->arg2, getFacade).evaluateClauseWithSyn();
			Table expected = makeTable( { {"5","2"},{"5","4"}, });
			assertTableEqual(expected, actual);
		}
	}

	


	};
};