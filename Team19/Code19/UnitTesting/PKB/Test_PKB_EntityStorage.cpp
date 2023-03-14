#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PKB/PKBFacade.h"
#include "../source/Entity/Entity.h"
#include "../source/Pattern/Pattern.h"
#include "../source/TypeAlias/TypeAlias.h"
#include "../source/SourceProcessor/Tokenizer/SPTokenizer.h"
#include "../source/SourceProcessor/Parser/Parser.h"
//#include "TNode.h"
#include <cstddef>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{


    TEST_CLASS(Test_PKB_Entity_Storage)
    {
    public: 
        TEST_METHOD(TestReadVariable) {
            PKBFacade pkbStub = PKBFacade();
            pkbStub.insertEntity(ReadStatement(1, "a"));
            pkbStub.insertEntity(CallStatement(2, "proc"));
            pkbStub.insertEntity(PrintStatement(3, "b"));
            pkbStub.insertEntity(WhileStatement(5));
            pkbStub.insertEntity(IfStatement(6));
            pkbStub.insertEntity(ReadStatement(7, "fs"));
            pkbStub.insertEntity(CallStatement(8, "fasc"));
            pkbStub.insertEntity(PrintStatement(9, "ho"));

            GetEntityRequest request = GetEntityRequest(EntityMode::READ);
            EntityResults allRead = pkbStub.get(request);
            int expected_size = 2;
            int actual_size = static_cast<int>(allRead.size());
            Assert::AreEqual(expected_size, actual_size);

            request = GetEntityRequest(EntityMode::READVAR);
            EntityResults allReadVar = pkbStub.get(request);
            expected_size = 2;
            actual_size = static_cast<int>(allReadVar.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesItemExists("a", allReadVar), true);
            Assert::AreEqual(doesItemExists("fs", allReadVar), true);
        }

        TEST_METHOD(TestPrintVariable) {
            PKBFacade pkbStub = PKBFacade();
            pkbStub.insertEntity(ReadStatement(1, "a"));
            pkbStub.insertEntity(CallStatement(2, "proc"));
            pkbStub.insertEntity(PrintStatement(3, "b"));
            pkbStub.insertEntity(WhileStatement(5));
            pkbStub.insertEntity(IfStatement(6));
            pkbStub.insertEntity(ReadStatement(7, "fs"));
            pkbStub.insertEntity(CallStatement(8, "fasc"));
            pkbStub.insertEntity(PrintStatement(9, "ho"));

            GetEntityRequest request = GetEntityRequest(EntityMode::PRINT);
            EntityResults allPrint = pkbStub.get(request);
            int expected_size = 2;
            int actual_size = static_cast<int>(allPrint.size());
            Assert::AreEqual(expected_size, actual_size);

            request = GetEntityRequest(EntityMode::PRINTVAR);
            EntityResults allPrintVar = pkbStub.get(request);
            expected_size = 2;
            actual_size = static_cast<int>(allPrintVar.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesItemExists("b", allPrintVar), true);
            Assert::AreEqual(doesItemExists("ho", allPrintVar), true);
        }

        TEST_METHOD(TestCallProc) {
            PKBFacade pkbStub = PKBFacade();
            pkbStub.insertEntity(ReadStatement(1, "a"));
            pkbStub.insertEntity(CallStatement(2, "proc"));
            pkbStub.insertEntity(PrintStatement(3, "b"));
            pkbStub.insertEntity(WhileStatement(5));
            pkbStub.insertEntity(IfStatement(6));
            pkbStub.insertEntity(ReadStatement(7, "fs"));
            pkbStub.insertEntity(CallStatement(8, "fasc"));
            pkbStub.insertEntity(PrintStatement(9, "ho"));

            GetEntityRequest request = GetEntityRequest(EntityMode::CALL);
            EntityResults allCalls = pkbStub.get(request);
            int expected_size = 2;
            int actual_size = static_cast<int>(allCalls.size());
            Assert::AreEqual(expected_size, actual_size);

            request = GetEntityRequest(EntityMode::CALLSPROC);
            EntityResults allCallsProc = pkbStub.get(request);
            expected_size = 2;
            actual_size = static_cast<int>(allCallsProc.size());
            Assert::AreEqual(expected_size, actual_size);

            Assert::AreEqual(doesItemExists("proc", allCallsProc), true);
            Assert::AreEqual(doesItemExists("fasc", allCallsProc), true);

            request = GetEntityRequest(EntityMode::CALLBYSTMT, "8");
            procRef correct_proc = *pkbStub.get(request).begin();
            Assert::AreEqual(procRef("fasc"), correct_proc);

            request = GetEntityRequest(EntityMode::CALLBYSTMT, "8");
            procRef wrong_proc = *pkbStub.get(request).begin();
            Assert::AreNotEqual(procRef("fasc"), wrong_proc);
        }

        bool doesItemExists(std::string item, EntityResults results) {
            if (results.find(item) != results.end()) {
                return true;
            }
            return false;
        }
    };
}