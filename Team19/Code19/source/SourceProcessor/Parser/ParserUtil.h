#pragma once
#include <string>
#include "../../Definitions/Definitions.h"
#include "../Tokenizer/TokenIterator.h"
#include "ParseException.h"

using namespace std;

class ParserUtil {
public:
	static bool isEndOfFile(TokenIterator& head, TokenIterator& end);

	/**
	 * @brief Expect token at current index to have value str.
	 *
	 * Increments iterator if true, and throws exception otherwise.
	 */
	static void expect(TokenIterator& head, string str);

	static TokenIterator findRelationalFactorEndIndex(TokenIterator& start);
};