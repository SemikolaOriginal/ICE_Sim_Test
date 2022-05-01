#include "EngineException.h"


EngineException::ENGINE_EXCEPTIONS EngineException::getExceptionNumber() {
	return this->exceptionNumber;
}


string EngineException::getExceptionMessage() {
	return this->exceptionMessage;
}

EngineException::EngineException(string text, ENGINE_EXCEPTIONS number) : exceptionMessage(text), exceptionNumber(number){
	return;
}