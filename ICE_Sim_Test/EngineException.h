#pragma once

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

class EngineException
{
public:
	enum class ENGINE_EXCEPTIONS {
		INVALID_CONFIG,
		CREATION_FAILED,
		SIMULATION_ERROR
	};

private:
	string exceptionMessage;
	ENGINE_EXCEPTIONS exceptionNumber;

public:
	ENGINE_EXCEPTIONS getExceptionNumber();
	string getExceptionMessage();

	EngineException(string text, ENGINE_EXCEPTIONS number);

};

