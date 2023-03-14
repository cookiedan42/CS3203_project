#include "stdafx.h"
#include "../source/QP/QueryObject/Declaration.h"
#include "../source/QP/QueryParser/QueryParserResult.h"
#include "../source/Optimizer/Optimizer.h"

#include <codecvt>
#include <locale>
#include <string>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(Test_Optimizer)
	{

	std::deque<ClauseGroup> makeGroups(std::vector<std::shared_ptr<Clause>> clauses) {
		std::deque<ClauseGroup> out {};
		for (auto& clause : clauses) {
			ClauseGroup inner{};
			inner.addClauseToGroup(clause);
			out.push_back(inner);
		}
		return out;
	}

	ClauseGroup makeGroup(std::vector<std::shared_ptr<Clause>> clauses) {
		ClauseGroup out {};
		for (auto& clause : clauses) {
			out.addClauseToGroup(clause);
		}
		return out;
	}




	public:
		TEST_METHOD(Test_Group_And_Sort)
		{
			Declaration d1 = Declaration(make_shared<DE::Assign>(), "a1");
			Declaration d2 = Declaration(make_shared<DE::Statement>(), "s1");
			Declaration d3 = Declaration(make_shared<DE::Statement>(), "s2");
			Declaration d4 = Declaration(make_shared<DE::Statement>(), "s3");
			Declaration d5 = Declaration(make_shared<DE::Variable>(), "v");
			Declaration d6 = Declaration(make_shared<DE::Procedure>(), "proc");
			Declaration d7 = Declaration(make_shared<DE::Procedure>(), "main");
			Declaration d8 = Declaration(make_shared<DE::Variable>(), "v2");

			Argument wildcard = Argument{ WildCard() };
			Argument syn_s1 = Argument{ d2 };
			Argument syn_s2 = Argument{ d3 };
			Argument syn_s3 = Argument{ d4 };
			Argument syn_a1 = Argument{ d1 };
			Argument syn_v = Argument{ d5 };
			Argument syn_v2 = Argument{ d8 };
			Argument syn_proc = Argument{ d6 };
			Argument syn_main = Argument{ d7 };
			Argument stmtno_3 = Argument{ StmtNo("3") };
			Argument stmtno_5 = Argument{ StmtNo("5") };
			Argument stmtno_6 = Argument{ StmtNo("6") };

			SuchThatClause st1 = SuchThatClause(make_shared<DA::FollowsRel>(syn_s1, stmtno_3)); //Follows(s1, "3")
			SuchThatClause st2 = SuchThatClause(make_shared<DA::ModifiesRel>(syn_s2, wildcard)); //Modifies(s2, _)
			SuchThatClause st3 = SuchThatClause(make_shared<DA::ParentTRel>(syn_a1, syn_s3)); //Parent*(a1, s3)
			SuchThatClause st4 = SuchThatClause(make_shared<DA::FollowsRel>(stmtno_5, stmtno_6)); //Follows("5", "6")
			SuchThatClause st5 = SuchThatClause(make_shared<DA::UsesRel>(syn_s2, syn_v)); //Uses(s2, v)
			SuchThatClause st6 = SuchThatClause(make_shared<DA::CallsTRel>(syn_proc, wildcard)); // Calls*(proc, _)
			SuchThatClause st7 = SuchThatClause(make_shared<DA::UsesRel>(syn_proc, syn_v2)); //Uses(proc, v2)
			SuchThatClause st8 = SuchThatClause(make_shared<DA::UsesRel>(syn_main, wildcard)); //Uses(main, _)

			PatternClause pt1 = PatternClause(d1, syn_v, wildcard); //pattern a1(v, _)
			PatternClause pt2 = PatternClause(d1, syn_v, wildcard); //pattern a1(v, _)
			PatternClause pt3 = PatternClause(d1, syn_v, wildcard); //pattern a1(v, _)

			//WithClause w1 = WithClause

			//Test for singular synonym return
			SelectObject so = SelectObject({ {d1, ArgType::SYN} }); //Select a1
			QueryParserResult result = QueryParserResult({ d1 }, so, { st4 }, { st1, st2, st3, st5, st6, st7, st8 }, {}, { pt1, pt2, pt3 }, {}, {});

			//expected grouping:						expected sorted:
			// Modifies(s2, _)                          Modifies(s2, _)        
			// Parent* (a1, s3)							Uses(s2, v)	
			// Uses(s2, v)								pattern a1(v, _)
			// pattern a1(v, _)							pattern a1(v, _)
			// pattern a1(v, _)							pattern a1(v, _)
			// pattern a1(v, _)							Parent* (a1, s3)
			// ==> {v, a1, s2, s3}  [related]		

			// Follows(s1, "3")
			// ==> {s1}

			// Uses(proc, v2)
			// Calls*(proc, _)
			// ==> {proc, v2}

			// Uses(main, _)
			// ==> {main}

			// Follows("5", "6")
			std::unordered_set<std::string> synSet1{ "v", "a1", "s2", "s3" };
			std::unordered_set<std::string> synSet2{ "s1" };
			std::unordered_set<std::string> synSet3{ "proc", "v2" };
			std::unordered_set<std::string> synSet4{ "main" };

			QueryObject grouped = Optimizer::group(result);

			Assert::AreEqual(isSynSetInGroupVector(grouped.clauseGroupsRelatedToSelect, synSet1), true);
			Assert::AreEqual(isSynSetInGroupVector(grouped.clauseGroupsNotRelatedToSelect, synSet2), true);

			Assert::AreEqual(isSynSetInGroupVector(grouped.clauseGroupsNotRelatedToSelect, synSet3), true);
			Assert::AreEqual(isSynSetInGroupVector(grouped.clauseGroupsNotRelatedToSelect, synSet4), true);

			Assert::AreEqual(static_cast<int>(grouped.clauseGroupsRelatedToSelect.size()), 1);
			Assert::AreEqual(static_cast<int>(grouped.clauseGroupsNotRelatedToSelect.size()), 3);


			//checked sorted in ascending order
			QueryObject sorted = Optimizer::sort(grouped);
			for (auto& group : sorted.clauseGroupsRelatedToSelect) {
				Assert::AreEqual(isAscendingOrder(group), true);
			}

			for (auto& group : sorted.clauseGroupsNotRelatedToSelect) {
				Assert::AreEqual(isAscendingOrder(group), true);
			}


			//Test for tuple synonym returns
			so = SelectObject({ {d1, ArgType::SYN}, {d8, ArgType::SYN} }); //<a1, v2>
			result = QueryParserResult({ d1 }, so, { st4 }, { st1, st2, st3, st5, st6, st7, st8 }, {}, { pt1, pt2, pt3 }, {}, {});
			grouped = Optimizer::group(result);

			Assert::AreEqual(isSynSetInGroupVector(grouped.clauseGroupsRelatedToSelect, synSet1), true);
			Assert::AreEqual(isSynSetInGroupVector(grouped.clauseGroupsRelatedToSelect, synSet3), true);

			Assert::AreEqual(isSynSetInGroupVector(grouped.clauseGroupsNotRelatedToSelect, synSet2), true);
			Assert::AreEqual(isSynSetInGroupVector(grouped.clauseGroupsNotRelatedToSelect, synSet4), true);

			Assert::AreEqual(static_cast<int>(grouped.clauseGroupsRelatedToSelect.size()), 2);
			Assert::AreEqual(static_cast<int>(grouped.clauseGroupsNotRelatedToSelect.size()), 2);


			sorted = Optimizer::sort(grouped);
			for (auto& group : sorted.clauseGroupsRelatedToSelect) {
				Assert::AreEqual(isAscendingOrder(group), true);
			}

			for (auto& group : sorted.clauseGroupsNotRelatedToSelect) {
				Assert::AreEqual(isAscendingOrder(group), true);
			}


		}

		bool isAscendingOrder(ClauseGroup group) {
			int prevPenalty = -100;
			for (auto& cl : group.getRelatedClauses()) {
				if (cl->penalty < prevPenalty) {
					return false;
				}
				prevPenalty = cl->penalty;
			}
			return true;
		}

		bool isSynSetInGroupVector(std::vector<ClauseGroup> vec, std::unordered_set<std::string> synSet) {
			for (auto& group : vec) {
				if (synSet == group.getGroupSynNames()) {
					return true;
				}
			}
			return false;
		}



		struct rewrite_helper {
			Declaration a1 = Declaration(make_shared<DE::Assign>(), "a1");
			Declaration a2 = Declaration(make_shared<DE::Assign>(), "a2");
			Declaration s1 = Declaration(make_shared<DE::Statement>(), "s1");
			Declaration s2 = Declaration(make_shared<DE::Statement>(), "s2");
			Declaration s3 = Declaration(make_shared<DE::Statement>(), "s3");
			Declaration v = Declaration(make_shared<DE::Variable>(), "v");

			Argument wildcard = Argument { WildCard() };
			Argument syn_s1 = Argument { s1 };
			Argument syn_s2 = Argument { s2 };
			Argument syn_s3 = Argument { s3 };
			Argument syn_a1 = Argument { a1 };
			Argument syn_a2 = Argument { a2 };
			Argument syn_v = Argument { v };
			Argument stmtno_3 = Argument { StmtNo("3") };
			Argument stmtno_5 = Argument { StmtNo("5") };
			Argument stmtno_6 = Argument { StmtNo("6") };
			Argument attr_s1_no = Argument { SynAttr{s1, ArgType::ATTR_REF_INT} };

			//Next*(s1, "3")
			shared_ptr<Clause> st1 = make_shared<SuchThatClause>(SuchThatClause(make_shared<DA::NextTRel>(syn_s1, stmtno_3)));
			//Next*(_, "3")
			shared_ptr<Clause> st2 = make_shared<SuchThatClause>(SuchThatClause(make_shared<DA::NextTRel>(wildcard, stmtno_3)));

			//Next*(a1, "3")
			shared_ptr<Clause> st3 = make_shared<SuchThatClause>(SuchThatClause(make_shared<DA::NextTRel>(syn_a1, stmtno_3)));
			//Next*(a2, "3")
			shared_ptr<Clause> st4 = make_shared<SuchThatClause>(SuchThatClause(make_shared<DA::NextTRel>(syn_a2, stmtno_3)));

			//Affectst*(s1, "3")
			shared_ptr<Clause> st5 = make_shared<SuchThatClause>(SuchThatClause(make_shared<DA::AffectsTRel>(syn_s1, stmtno_3)));
			//Affects*(_, "3")
			shared_ptr<Clause> st6 = make_shared<SuchThatClause>(SuchThatClause(make_shared<DA::AffectsTRel>(wildcard, stmtno_3)));

			//Affects*(a1, "3")
			shared_ptr<Clause> st7 = make_shared<SuchThatClause>(SuchThatClause(make_shared<DA::AffectsTRel>(syn_a1, stmtno_3)));
			//Affects*(_, "3")
			shared_ptr<Clause> st8 = make_shared<SuchThatClause>(SuchThatClause(make_shared<DA::AffectsTRel>(wildcard, stmtno_3)));




			shared_ptr<Clause> pt1 = make_shared<PatternClause>(PatternClause(a1, syn_v, wildcard)); //pattern a1(v, _)
			shared_ptr<Clause> pt2 = make_shared<PatternClause>(PatternClause(a1, syn_v, wildcard)); //pattern a1(v, _)
			shared_ptr<Clause> pt3 = make_shared<PatternClause>(PatternClause(a1, syn_v, wildcard)); //pattern a1(v, _)

			// with s1.stmt# = s1.stmt#
			shared_ptr<Clause> w1 = make_shared<WithClause>(WithClause(attr_s1_no, attr_s1_no));

			//Test for singular synonym return
			SelectObject so = SelectObject({ {a2, ArgType::SYN} }); //Select a1
			SelectObject so_s1 = SelectObject({ {s1, ArgType::SYN} }); //Select s1
		};
		rewrite_helper r = rewrite_helper();


		TEST_METHOD(Test_Rewrite_Next_unused) {
			rewrite_helper r = rewrite_helper();
		
			QueryParserResult initial = {{},r.so,
					makeGroup({}),
					makeGroups({r.st1,r.pt1,r.st3})
				};

			QueryParserResult expected = {{},r.so,
					makeGroup({r.st2}),
					makeGroups({r.pt1,r.st3})
				};
				
				
			QueryParserResult actual = Optimizer::rewriteSuchThat(initial);
				// st3 is unchanged because a1 used in pt1
				// st1 is changed to st2 because unused
				Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(Test_Rewrite_Next_used) {
			rewrite_helper r = rewrite_helper();

			QueryParserResult initial = { {},r.so,
					makeGroup({}),
					makeGroups({r.st4})
			};

			QueryParserResult expected = { {},r.so,
				makeGroup({}),
				makeGroups({r.st4})
			};
			QueryParserResult actual = Optimizer::rewriteSuchThat(initial);
			// st4 is unchanged because of a2 being used in select Object
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(Test_Rewrite_Next_assignType) {
			rewrite_helper r = rewrite_helper();

			QueryParserResult initial = { {},r.so,
					makeGroup({}),
					makeGroups({r.st3})
				};

			QueryParserResult expected = { {},r.so,
					makeGroup({}),
					makeGroups({r.st4})
				};

			QueryParserResult actual = Optimizer::rewriteSuchThat(initial);
				// st3 is changed because of assign type in next*
				Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(Test_Rewrite_Affects_assignType) {
			rewrite_helper r = rewrite_helper();

			QueryParserResult initial = { {},r.so,
					makeGroup({}),
					makeGroups({r.st7})
			};

			QueryParserResult expected = { {},r.so,
				makeGroup({r.st8}),
				makeGroups({})
			};

			QueryParserResult actual = Optimizer::rewriteSuchThat(initial);
			// st7 is changed because of assign type in affects*
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(Test_Rewrite_Next_usedInClause) {
			rewrite_helper r = rewrite_helper();

			QueryParserResult initial = { {},r.so,
					makeGroup({}),
					makeGroups({r.st1, r.w1})
			};

			QueryParserResult expected = { {},r.so,
				makeGroup({}),
				makeGroups({r.st1, r.w1})
			};

			QueryParserResult actual = Optimizer::rewriteSuchThat(initial);
			// st1 is unchanged because of w1 using s1 too
			Assert::IsTrue(actual == expected);
		}

		TEST_METHOD(Test_Rewrite_stmtInSelect) {
			rewrite_helper r = rewrite_helper();

			QueryParserResult initial = { {},r.so_s1,
					makeGroup({}),
					makeGroups({r.st1})
			};

			QueryParserResult expected = { {},r.so_s1,
				makeGroup({}),
				makeGroups({r.st1})
			};

			QueryParserResult actual = Optimizer::rewriteSuchThat(initial);
			// st1 is unchanged because used in select
			Assert::IsTrue(actual == expected);
		}



};
}
