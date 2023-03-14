#include "stdafx.h"
#include "../source/QP/QueryManager.h"
#include "../source/QP/QueryObject/Declaration.h"
#include <codecvt>
#include <locale>
#include <string>

namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {
			template<>
			static std::wstring ToString<Declaration>(const Declaration& dec) {
				std::string result = "Declaration with var " + dec.synName;
				return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(result);
			}
		}
	}
}