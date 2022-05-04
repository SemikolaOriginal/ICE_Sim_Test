#include "TestException.h"

TestException::TEST_EXCEPTIONS TestException::getExceptionNumber() {
	return this->exceptionNumber;
}


string TestException::getExceptionMessage() {
	return this->exceptionMessage;
}

TestException::TestException(string text, TEST_EXCEPTIONS number) : exceptionMessage(text), exceptionNumber(number) {
}