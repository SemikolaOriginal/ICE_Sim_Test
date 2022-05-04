#include "Engine.h"

#ifdef _DEBUG

#include <iostream>
using std::cout;
using std::endl;

#endif

#include <exception>
using std::logic_error;

void Engine::start(double ambientTemperature){
	if(this->isStarted)
		return;

	this->currentTemperature = ambientTemperature;
	this->currentV = this->MV.begin()->first;
	this->isStarted = true;
}

void Engine::stop() {
	if (!this->isStarted)
		return;

	this->currentV = 0;
	this->isStarted = false;
}
void Engine::update(double deltaTime, double& ambientTemperature) {
	double temp = this->getCurrentEngineTempIncrement(deltaTime, ambientTemperature);
	//ambientTemperature += this->getCurrentCoolingCoef(ambientTemperature);

	#ifdef _DEBUG
	cout << "getCurrentEngineTempIncrement: " << temp << endl;
	#endif // DEBUG
	this->currentTemperature += temp;

	if (this->currentV + this->deltaV <= this->MV.rbegin()->first &&
		this->currentV + this->deltaV >= 0) {
		this->currentV += this->deltaV;
	}
	else {
		// Reaches the V value limit
	}

	if (this->currentTemperature >= this->To) {
		this->isOverheat = true;
		return;
	}
	if (this->isOverheat && this->currentTemperature <= this->To) {
		this->isOverheat = false;
	}
}

bool Engine::is_started() {
	return this->isStarted;
}

bool Engine::is_overheat() {
	return this->isOverheat;
};

bool Engine::is_MaxCrankshaftSpeedReached() {
	return (this->currentV + this->deltaV >= this->MV.rbegin()->first);
}

void Engine::cooling(double setEngineTemperature) {
	if (setEngineTemperature >= this->currentTemperature) return;

	this->currentTemperature = setEngineTemperature;
	if (setEngineTemperature < this->To && this->isOverheat)
		this->isOverheat = false;
}

int Engine::getCurrentM() {
	auto it = this->MV.find(this->currentV);
	if (it != this->MV.end()) {
		return it->second;
	}

	std::pair<
		std::map<unsigned int, unsigned int>::iterator,
		std::map<unsigned int, unsigned int>::iterator> MV_range = this->MV.equal_range(this->currentV);
	MV_range.first = MV_range.second--;

	double k = 
		static_cast<double>(static_cast<int>(MV_range.second->second) - static_cast<int>(MV_range.first->second)) /
		static_cast<double>(static_cast<int>(MV_range.second->first) - static_cast<int>(MV_range.first->first));
	double b = static_cast<double>(static_cast<int>(MV_range.second->second) - k * static_cast<int>(MV_range.second->first));

	return static_cast<int>(round((k * this->currentV + b)));
}

double Engine::getCurrentHeatCoef() {
	int M = this->getCurrentM();
	return (M * this->HM + this->currentV * this->currentV * this->HV);
}

double Engine::getCurrentCoolingCoef(const double& ambientTemperature) {
	return this->C * (this->currentTemperature - ambientTemperature);
}

double Engine::getCurrentEngineTempIncrement(double time, const double& ambientTemperature) {
	return (this->getCurrentHeatCoef() - this->getCurrentCoolingCoef(ambientTemperature)) * time;
}

double Engine::getCurrentTemperature() {
	return round(this->currentTemperature);
}

double Engine::getDeltaTime() {
	int M = this->getCurrentM();

	// dont swap conditions place

	if ((this->deltaV == 0 && M == 0) || 
		(this->is_MaxCrankshaftSpeedReached() && M == 0)) {
		return static_cast<double>(this->I);
	} else if (this->deltaV != 0 && M == 0) {
		throw logic_error("detected zero division by calculate Engine::getDeltaTime()");
	}
	return static_cast<double>(this->I) * static_cast<double>(this->deltaV) / static_cast<double>(M);
}