#include "EngineConfig.h"

#include <fstream>
using std::ifstream;

#include <map>
using std::map;

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

void EngineConfig::checkDataCorrects() {
	if (
		this->HM < 0.0 ||
		this->HV < 0.0 ||
		this->C < 0.0) {
		this->~EngineConfig();
	}
}
bool EngineConfig::isWrong() {
	return this->MV.empty();
}
/*
CfgFile sctructure:
	I	- int					- engine moment of inertia
	M	- vector<unsigned int>	- engine torque
	V	- vector<unsigned int>	- crankshaft rotation speed
	To	- int - overheat temperature
	HM	- double				- coefficient of heating rate versus torque
	HV	- double				- coefficient of dependence of the heating rate on the speed of rotation of the crankshaft
	C	- double				- coefficient of dependence of the cooling rate on the temperature of the engine and the environment
*/

EngineConfig::EngineConfig(string config_path) {
	ifstream inputFile("engine.cfg");

	if (!inputFile.is_open()) {
		#ifdef DEBUG
		cout << "Error at opening engine configuration file" << endl;
		#else
		cout << "EngineConfig(string config_path): open file error" << endl;
		#endif // DEBUG

		this->~EngineConfig();
		return;
	};

	unsigned int set_I = 0;
	map<unsigned int, unsigned int> set_MV;
	int set_To = 0;
	double set_HM = 0.0;
	double set_HV = 0.0;
	double set_C = 0.0;

	inputFile >> set_I;
	inputFile.ignore();

	string tempStr = "", tempStr2 = "";

	std::getline(inputFile, tempStr);
	std::getline(inputFile, tempStr2);

	// unpack key-string and result-string
	do {
		unsigned int key = 0;
		unsigned int value = 0;
		if (std::string::npos == tempStr.find_first_of(',') && 
			std::string::npos == tempStr2.find_first_of(',')) {
			try {
				key = std::stoul(tempStr2);
			}
			catch (...) {
				this->~EngineConfig();
				return;
			}

			try{
				value = std::stoul(tempStr);
			}
			catch (...) {
				this->~EngineConfig();
				return;
			}

			tempStr2.clear();
			tempStr.clear();
		}
		else {
			try{ 
				key = std::stoul(tempStr2.substr(0, tempStr2.find_first_of(','))); 
			}
			catch (...) {
				this->~EngineConfig();
				return;
			}
			try {
				value = std::stoul(tempStr.substr(0, tempStr.find_first_of(',')));
			}
			catch (...) {
				this->~EngineConfig();
				return;
			}

			tempStr2.erase(0, tempStr2.find_first_of(',') + 1);
			tempStr.erase(0, tempStr.find_first_of(',') + 1);
		}

		set_MV.insert(std::pair<unsigned int, unsigned int>(key, value));

	} while (!tempStr.empty() && !tempStr2.empty());



	inputFile >> set_To >> set_HM >> set_HV >> set_C;
	inputFile.close();

	this->I = set_I;
	this->MV = set_MV;
	this->To = set_To;
	this->HM = set_HM;
	this->HV = set_HV;
	this->C = set_C;

	this->checkDataCorrects(); // !

	cout << "EngineCFG:\n\tI = " << this->I << endl;

	cout << "\tM(V):" << endl;
	for (const auto& row : this->MV) {
		cout << "\t\t" << row.second << "(" << row.first << ")" << endl;
	}
	cout << "\tTo = " << this->To << endl <<
		"\tHM = " << this->HM << endl <<
		"\tHV = " << this->HV << endl <<
		"\tC = " << this->C << endl;

};