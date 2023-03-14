#pragma once
#include <iostream>
#include <exception>

using namespace std;

class ParseException : public exception {
private:
	string errorMessage;

public:
	ParseException(string message);

	string what();
};