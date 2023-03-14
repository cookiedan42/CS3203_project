#pragma once
# include <vector>
#include <stdexcept>
#include"../Validator/ValidatorUtil.h"
#include "./Token.h"

//hold the token vector
// expose methods to get current value
// expose methods to get offset value
// safe iteration and access

using TokenIterator_exception = std::invalid_argument;

struct TokenIterator {
private:
	std::vector<Token>::const_iterator it;
	// Copy of vector.cend() for bounds checking
	std::vector<Token>::const_iterator pastTheEnd;

	TokenIterator(std::vector<Token>::const_iterator it, std::vector<Token>::const_iterator pastTheEnd) : it(it), pastTheEnd(pastTheEnd) {}
	void assertInRange() const;
	//get a copy of it for constructing sub vector
	std::vector<Token>::const_iterator getRawIt() const;
public:
	TokenIterator(TokenIterator& other) : it(other.it),pastTheEnd(other.pastTheEnd) {}
	
	//Construct a TokenIterator pointing to the start of the vector<Token>
	static TokenIterator cbegin(std::vector<Token>& tokens) { 
		return TokenIterator(tokens.cbegin(),tokens.cend()); 
	}
	
	//Construct a TokenIterator pointing past the end of the vector<Token>
	static TokenIterator cend(std::vector<Token>& tokens) { 
		return TokenIterator(tokens.cend(),tokens.cend()); 
	}



	friend bool operator==(const TokenIterator& lhs, const TokenIterator& rhs) {
		return (lhs.it == rhs.it) && (lhs.pastTheEnd==rhs.pastTheEnd);
	}
	friend bool operator!=(const TokenIterator& lhs, const TokenIterator& rhs) {
		return !(lhs == rhs);
	}
	friend TokenIterator operator+(const TokenIterator& lhs, const int& rhs) {
		return TokenIterator(lhs.it + rhs,lhs.pastTheEnd);
	}
	friend TokenIterator operator-(const TokenIterator& lhs, const int& rhs) {
		return TokenIterator(lhs.it - rhs,lhs.pastTheEnd);
	}
	friend ptrdiff_t operator-(const TokenIterator& lhs, const TokenIterator& rhs) {
		return lhs.it - rhs.it;
	}

	//safe it++
	void advance();
	// advance until TokenIterator is pointing to a Token containing target
	void advanceTo(const std::string& target);
	//it--
	void back();

	
	//Test if the value of the current Token being pointed to is equal to target
	bool valueIs(const std::string& target) const;
	//Test if the value of the Token at it+offset to is equal to target
	bool valueIs(const std::string& target,int offset) const;
	//Test if the Iterator is pointing to a Token containing a valid Name
	bool isName() const;
	//Test if the Iterator is pointing to a Token containing a valid Integer
	bool isInteger() const;
	//Test if the Iterator is pointing to a Token containing a valid Boolean
	bool isBoolean() const;
	//Test if the Iterator is pointing to a Token containing a valid Comparator
	bool isComparator() const;
	//Test if the Iterator is pointing to a Token containing a valid Math operator
	bool isMathOperator() const;
	//Get the value of the Token TokenIterator is pointing to 
	std::string getValue() const;
	//Get a copy of the Token TokenIterator is pointing to 
	Token getToken() const;


	/**
	Construct vector<Token> from two iterators
	Follows cpp vector construction semantics, 
	@param begin TokenIterator pointing to first element in output vector
	@param end TokenIterator pointing past last element in output vector
	@return new vector<Token>
	*/
	static std::vector<Token> copySubVector(TokenIterator begin,TokenIterator end) ;
};
