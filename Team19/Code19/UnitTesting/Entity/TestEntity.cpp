#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../source/Entity/Entity.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestEntity) {
public:
	TEST_METHOD(TestGetters) {
		string varName = "var";
		string constName = "7";
		string procName = "proc";
		string read = "read";
		Ent::Variable variable{ varName };
		Ent::Constant constant{ constName };
		Ent::Procedure procedure{ procName };
		Ent::Statement statement{ 1, read };

		Assert::AreEqual(variable.getName(), varName);
		Assert::AreEqual(constant.getName(), constName);
		Assert::AreEqual(procedure.getName(), procName);
		Assert::AreEqual(statement.getName(), read);

		Assert::IsTrue(variable.getEntityType() == Ent::var_entityType);
		Assert::IsTrue(constant.getEntityType() == Ent::const_entityType);
		Assert::IsTrue(procedure.getEntityType() == Ent::proc_entityType);
		Assert::IsTrue(statement.getEntityType() == Ent::stmt_entityType);

		Assert::IsTrue(statement.getStmtNo() == 1);
	}

	TEST_METHOD(TestOperator) {
		Ent::Variable variable{ "v1" };
		Ent::Variable variable1{ "v1" };
		Ent::Variable variable2{ "v2" };
		Ent::Constant constant{ "c1" };
		Ent::Constant constant1{ "c1" };
		Ent::Constant constant2{ "c2" };
		Ent::Procedure procedure{ "p1" };
		Ent::Procedure procedure1{ "p1" };
		Ent::Procedure procedure2{ "p2" };
		Ent::Statement statement{ 1, Ent::read_stmtType };
		Ent::Statement statement1{ 1, Ent::read_stmtType };
		Ent::Statement statement2{ 2, Ent::read_stmtType };
		Ent::Statement statement3{ 1, Ent::print_stmtType };

		Assert::IsTrue(variable == variable1);
		Assert::IsTrue(variable != variable2);
		Assert::IsTrue(variable != constant);
		Assert::IsTrue(variable != procedure);
		Assert::IsTrue(variable != statement);

		Assert::IsTrue(constant == constant1);
		Assert::IsTrue(constant != constant2);
		Assert::IsTrue(constant != procedure);
		Assert::IsTrue(constant != statement);

		Assert::IsTrue(procedure == procedure1);
		Assert::IsTrue(procedure != procedure2);
		Assert::IsTrue(procedure != statement);

		Assert::IsTrue(statement == statement1);
		Assert::IsTrue(statement != statement2);
		Assert::IsTrue(statement != statement3);
	}
	};
}