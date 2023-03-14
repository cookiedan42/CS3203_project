#include "./TokenIterator.h"

void TokenIterator::assertInRange() const {
	if (it == pastTheEnd) { throw std::out_of_range(Exception::ITERATOR_OUT_OF_RANGE); }
}

std::vector<Token>::const_iterator TokenIterator::getRawIt() const {
	return it;
}

void TokenIterator::advance() {
	assertInRange();
	it++;
}

void TokenIterator::advanceTo(const std::string& target) {
	while (!valueIs(target)) {
		advance();
	}
}

void TokenIterator::back() {
	it-- ;
}

bool TokenIterator::valueIs(const std::string& target) const {
	assertInRange();
	return it->getValue() == target;
}
bool TokenIterator::valueIs(const std::string& target, int offset) const {
	// it itself might be past the end and reversing into a vector
	if (it+offset == pastTheEnd) { throw std::out_of_range(Exception::ITERATOR_OUT_OF_RANGE); }
	return (it+offset)->getValue() == target;
}
bool TokenIterator::isName() const {
	assertInRange();
	return ValidatorUtil::isName(it->getValue());
}
bool TokenIterator::isInteger() const {
	assertInRange();
	return ValidatorUtil::isInteger(it->getValue());
}
bool TokenIterator::isBoolean() const {
	assertInRange();
	return ValidatorUtil::isBoolean(it->getValue());
}
bool TokenIterator::isComparator() const {
	assertInRange();
	return ValidatorUtil::isComparator(it->getValue());
}
bool TokenIterator::isMathOperator() const {
	assertInRange();
	return ValidatorUtil::isMathOperator(it->getValue());
}


std::string TokenIterator::getValue() const {
	assertInRange();
	return it->getValue();
}

Token TokenIterator::getToken() const {
	assertInRange();
	return *it;
}

std::vector<Token> TokenIterator::copySubVector(TokenIterator begin, TokenIterator end) {
	return std::vector<Token>(begin.getRawIt(), end.getRawIt());
}
