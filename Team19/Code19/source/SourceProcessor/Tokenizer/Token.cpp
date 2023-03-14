# include "./Token.h"


Token::Token(std::string t, std::string s) : type(t), value(s) {}

Token::Token(std::string t, char c) : type(t), value(std::string(1, c)) {}

NameToken::NameToken(std::string value) : Token(SIMPLE::NAME, value) {}

IntegerToken::IntegerToken(std::string value) : Token(SIMPLE::INTEGER, value) {}

SymbolToken::SymbolToken(std::string value) : Token(SIMPLE::SYMBOL, value) {}



bool Token::isNameToken() const {
	return type == SIMPLE::NAME;
}
bool Token::isIntegerToken() const {
	return type == SIMPLE::INTEGER;
}
bool Token::isSymbolToken() const {
	return type == SIMPLE::SYMBOL;
}
std::string Token::getValue() const {
	return value;
}
