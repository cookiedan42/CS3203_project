#pragma once
#include <string>
#include "../../Definitions/Definitions.h"

/*
Holds a token string
*/
struct Token {
private:
	std::string type;
	std::string value;
public:
	Token(std::string t, std::string s);
	Token(std::string t, char c);

	bool isNameToken() const;
	bool isIntegerToken() const;
	bool isSymbolToken() const;
	std::string getValue() const;

	friend bool operator==(const Token& lhs, const Token& rhs) {
		return (lhs.type == rhs.type) && (lhs.value == rhs.value);
	}
	friend bool operator!=(const Token& lhs, const Token& rhs) { 
		return !(lhs == rhs); 
	}
};

struct NameToken : Token {
	NameToken(std::string value);
};

struct IntegerToken : Token {
	IntegerToken(std::string value);
};

struct SymbolToken : Token {
	SymbolToken(std::string value);
};