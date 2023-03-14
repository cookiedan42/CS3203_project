#include "SPTokenizer.h"

void TokenSplitter::bufferFlush() {
	if (buffer.empty()) return;
	if (isalnum(buffer.at(0)) == 0) { // Symbol
		tokens.push_back(SymbolToken ( buffer ));
	} else if (isdigit(buffer.at(0)) == 0) { // Name
		tokens.push_back(NameToken ( buffer ));
	} else { // Integer
		tokens.push_back(IntegerToken ( buffer ));
	}
	buffer = "";
};

void TokenSplitter::bufferAppend() {
	buffer.append(std::string(1, *it));
};

bool TokenSplitter::isSingleSymbol() {
	return TokenizerConstants::singleSymbol.find(*it) 
		!= TokenizerConstants::singleSymbol.end();
}
bool TokenSplitter::isWhitespace() {
	return TokenizerConstants::whitespace.find(*it) 
		!= TokenizerConstants::whitespace.end();
}
bool TokenSplitter::isPair() {
	return TokenizerConstants::pairs.find(*it) 
		!= TokenizerConstants::pairs.end();
}
bool TokenSplitter::isPairHead() {
	return TokenizerConstants::pair_heads.find(*it) 
		!= TokenizerConstants::pair_heads.end();
}
bool TokenSplitter::nextHasVal(char c) {
	// bounds check before value check
	return (it + 1) != source_code.cend() && *(it + 1) == c ;
}

void TokenSplitter::tokenizeSingleSymbol() {
	//Tokenize things in buffer, then self
	bufferFlush();
	bufferAppend();
	bufferFlush();
}
void TokenSplitter::tokenizeWhitespace() {
	//buffer only, ignore whitespace characters in Tokenizing
	bufferFlush();
}
void TokenSplitter::tokenizePair() {
	// 2 same character && ||
	bufferFlush();
	bufferAppend();
	if (nextHasVal(buffer.at(0))) {
		it++;
		bufferAppend();
	}
	bufferFlush();
}
void TokenSplitter::tokenizePairHead() {
	//2 character comparisons end with =
	bufferFlush();
	bufferAppend();
	if (nextHasVal('=')) {
		it++;
		bufferAppend();
	}
	bufferFlush();
}

std::vector<Token> TokenSplitter::tokenize() {
	if (tokens.size() > 0) {return tokens;}

	while (it != srcEnd){
		if (isSingleSymbol()) {
			tokenizeSingleSymbol();
		}
		else if (isWhitespace()) {
			tokenizeWhitespace();
		}
		else if (isPair()) {
			tokenizePair();
		}
		else if (isPairHead()) {
			tokenizePairHead();
		} 
		else {
			// add to buffer if not special character
			// ie letter/number
			bufferAppend();
		}
		it++;
	}
	bufferFlush();
	return tokens;
};
