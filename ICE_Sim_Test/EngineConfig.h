#pragma once

/*
Engine configuration:
	I	- int								- engine moment of inertia
	MV	- map<unsigned int, unsigned int>	- dependence of torque on the speed of rotation of the crankshaft: M(V)
		map key: V, value: M; v is unique param
	To	- int - overheat temperature
	HM	- double							- coefficient of heating rate versus torque
	HV	- double							- coefficient of dependence of the heating rate on the speed of rotation of the crankshaft
	C	- double							- coefficient of dependence of the cooling rate on the temperature of the engine and the environment

*/

#include <map>
using std::map;

#include <string>
using std::string;

#include <iostream>


class EngineConfig
{
private:
	bool checkDataCorrects();

protected:
	unsigned int I = 0;
	map<unsigned int, unsigned int> MV;
	int To = 0;
	double HM = 0.0;
	double HV = 0.0;
	double C = 0.0;

public:

	EngineConfig(
		unsigned int set_I,
		map<unsigned int, unsigned int> set_MV,
		int set_To,
		double set_HM,
		double set_HV,
		double set_C
	);
	EngineConfig(string config_path);
};

