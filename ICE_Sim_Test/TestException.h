#pragma once

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

class TestException : public std::exception
{
public:
	enum class TEST_EXCEPTIONS {
		INVALID_POINTER_TO_ENGINE
	};

private:
	string exceptionMessage;
	TEST_EXCEPTIONS exceptionNumber;

public:
	TEST_EXCEPTIONS getExceptionNumber();
	string getExceptionMessage();

	TestException(string text, TEST_EXCEPTIONS number);

};