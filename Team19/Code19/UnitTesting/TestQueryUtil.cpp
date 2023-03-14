#include "stdafx.h"
#include "../source/QP/QueryEvaluator/QueryUtil.h"
#include <codecvt>
#include <locale>
#include <string>


namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<>
			static std::wstring ToString(const enum ArgsType& type)
			{
				switch (type) {
				case ArgsType::STMTNO_STMTNO: return L"1";
				case ArgsType::STMTNO_SYN: return L"2";
				case ArgsType::STMTNO_WILDCARD: return L"3";
				case ArgsType::STMTNO_QUOTE: return L"4";
				case ArgsType::SYN_STMTNO: return L"5";
				case ArgsType::SYN_SYN: return L"6";
				case ArgsType::SYN_WILDCARD: return L"7";
				case ArgsType::SYN_QUOTE: return L"8";
				case ArgsType::WILDCARD_STMTNO: return L"9";
				case ArgsType::WILDCARD_SYN: return L"10";
				case ArgsType::WILDCARD_WILDCARD: return L"11";
				}

				return std::to_wstring(static_cast<int>(type));
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting

{
	TEST_CLASS(Test_QueryUtil)
	{
	public:

		TEST_METHOD(Test_removeSubQuotes)
		{
			std::string expected = "y";
			std::string arg = "_\"y\"_";
			std::string removed = QueryUtil::removeSubQuotes(arg);
			Assert::AreEqual(expected, removed);
		}

		TEST_METHOD(Test_validWithRefStmtNo)
		{
			std::vector<Declaration> inputDec = {
				Declaration(make_shared<DE::Statement>(), "s1")
			};
			std::string arg = "s1.stmt#";
			Assert::IsTrue(QueryUtil::isValidWithRef(arg, inputDec));
		}

		TEST_METHOD(Test_validWithRefVarName)
		{
			std::vector<Declaration> inputDec = {
				Declaration(make_shared<DE::Variable>(), "v1")
			};
			std::string arg = "v1.varName";
			Assert::IsTrue(QueryUtil::isValidWithRef(arg, inputDec));
		}

		TEST_METHOD(Test_validWithRefProcName)
		{
			std::vector<Declaration> inputDec = {
				Declaration(make_shared<DE::Read>(), "p1")
			};
			std::string arg = "p1.procName";
			Assert::IsTrue(QueryUtil::isValidWithRef(arg, inputDec));
		}

		TEST_METHOD(Test_validWithRefValue)
		{
			std::vector<Declaration> inputDec = {
				Declaration(make_shared<DE::If>(), "ifs")
			};
			std::string arg = "ifs.value";
			Assert::IsTrue(QueryUtil::isValidWithRef(arg, inputDec));
		}

		TEST_METHOD(Test_invalidWithRefSynName)
		{
			std::vector<Declaration> inputDec = {
				Declaration(make_shared<DE::While>(), "ws")
			};
			std::string arg = "const.value";
			Assert::IsFalse(QueryUtil::isValidWithRef(arg, inputDec));
		}

		TEST_METHOD(Test_invalidWithRefAttrName)
		{
			std::vector<Declaration> inputDec = {
				Declaration(make_shared<DE::Assign>(), "a1")
			};
			std::string arg = "a1.variableName";
			Assert::IsFalse(QueryUtil::isValidWithRef(arg, inputDec));
		}

		TEST_METHOD(Test_validWithRefInt)
		{
			std::vector<Declaration> inputDec = {};
			std::string arg = "3";
			Assert::IsTrue(QueryUtil::isValidWithRef(arg, inputDec));
		}

		TEST_METHOD(Test_validWithRefValidIdent)
		{
			std::vector<Declaration> inputDec = {};
			std::string arg = "\"testVar\"";
			Assert::IsTrue(QueryUtil::isValidWithRef(arg, inputDec));
		}

		TEST_METHOD(Test_invalidWithRefInvalidIdent)
		{
			std::vector<Declaration> inputDec = {};
			std::string arg = "\"123\"";
			Assert::IsFalse(QueryUtil::isValidWithRef(arg, inputDec));
		}

		TEST_METHOD(Test_invalidWithRefMissingDelim)
		{
			std::vector<Declaration> inputDec = {
				Declaration(make_shared<DE::Statement>(), "s1")
			};
			std::string arg = "s1stmt#";
			Assert::IsFalse(QueryUtil::isValidWithRef(arg, inputDec));
		}
	};
}