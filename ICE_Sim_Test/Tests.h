#pragma once

#include <memory>
using std::shared_ptr;

constexpr auto MIN_DELTA_TEMPERATURE = 1.0e-18;

enum class FORCED_STOP_OF_TEST {
	DONT_STOP,
	BY_DETECTED_LIMIT_OF_PARAM
};

enum class TIME_TYPE_BY_INTEGRATION {
	CONST_TIME,
	FUNCTION_VARIABLE_TIME
};

enum class TEST_ENDPOINT {
	// init only by 2^n, when n is natural num
	BY_REACHED_MAX_CRANKSHAFT_SPEED = 1,
	BY_OVERHEAT = 2
};
TEST_ENDPOINT operator|(const TEST_ENDPOINT& left, const TEST_ENDPOINT& right);


class EngineTestStand {
private:
	shared_ptr<Engine> engine = nullptr;

public:
	TIME_TYPE_BY_INTEGRATION timeTypeByIntegration = TIME_TYPE_BY_INTEGRATION::CONST_TIME;
	TEST_ENDPOINT testEndpoint = TEST_ENDPOINT::BY_REACHED_MAX_CRANKSHAFT_SPEED;
	FORCED_STOP_OF_TEST forcedStop = FORCED_STOP_OF_TEST::DONT_STOP;

	void printTestInfo();

	void overheatEngineTest(double ambientTemperature);

	EngineTestStand(shared_ptr<Engine> setEngine);// Engine* setEngine);
};

int readAmbientTemperature();