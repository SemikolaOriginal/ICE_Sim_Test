#pragma once
#include "EngineConfig.h"

#include <string>
using std::string;

class Engine :
    private EngineConfig
{
private:
	double currentTemperature = 0.0;
	bool isStarted = false;
	bool isOverheat = false;

public:
	unsigned int currentV = 0;
	int deltaV = 0;

	void start(int ambientTemperature);
	void stop();
	void update(double deltaTime, int ambientTemperature);
	bool is_started();
	bool is_overheat();
	bool is_MaxCrankshaftSpeedReached();
	void cooling(int setEngineTemperature);

	int getCurrentM();
	double getCurrentHeatCoef();
	double getCurrentCoolingCoef(int ambientTemperature);
	double getCurrentEngineTempIncrement(double time, int ambientTemperature);
	int getCurrentTemperature();
	double getDeltaTime();

	bool isWrong();

	Engine(
		int set_I,
		map<unsigned int, unsigned int> set_MV,
		int set_To,
		double set_HM,
		double set_HV,
		double set_C) :
		EngineConfig(set_I, set_MV, set_To, set_HM, set_HV, set_C){};
	Engine(string config_path) : 
		EngineConfig(config_path){};
};

