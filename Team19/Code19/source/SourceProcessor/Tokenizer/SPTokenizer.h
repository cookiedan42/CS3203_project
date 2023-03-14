#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include "./Token.h"

namespace TokenizerConstants {
	const std::unordered_set<char> singleSymbol = { 
		'+','-', '/','%','*',	// math
		';', '{' ,'}','(',')',	// control
		',','#','.','_','\"'	// query
	};
	const std::unordered_set<char> whitespace = { ' ','\n','\t' };
	const std::unordered_set<char> pairs = { '&','|' };
	const std::unordered_set<char> pair_heads = { '!','<','>', '=' }; // second char will be a =
}

/*
Splits SIMPLE code into a list of Token objects for further processing
*/
class TokenSplitter {
	std::string source_code;
	std::string::const_iterator it = source_code.cbegin();
	std::string::const_iterator srcEnd = source_code.cend();
	std::string buffer = "";
	std::vector<Token> tokens {};
	void bufferFlush();
	void bufferAppend();

	bool isSingleSymbol();
	bool isWhitespace();
	bool isPair();
	bool isPairHead();
	bool nextHasVal(char c);

	void tokenizeSingleSymbol();
	void tokenizeWhitespace();
	void tokenizePair();
	void tokenizePairHead();


public:
	TokenSplitter(std::string source_code) :
		source_code(source_code) {}

	std::vector<Token> tokenize();
};