#include "EngineConfig.h"
#include "EngineException.h"

#include <fstream>
using std::ifstream;

#include <map>
using std::map;

#include <iostream>
using std::cout;
using std::cin;
using std::endl;


bool EngineConfig::checkDataCorrects() {
	if (
		this->HM < 0.0 ||
		this->HV < 0.0 ||
		this->C < 0.0  || 
		this->MV.empty()) {
		return false;
	}
	return true;
}

EngineConfig::EngineConfig(
	unsigned int set_I,
	map<unsigned int, unsigned int> set_MV,
	int set_To,
	double set_HM,
	double set_HV,
	double set_C
) : I(set_I),
MV(set_MV),
To(set_To),
HM(set_HM),
HV(set_HV),
C(set_C) {
	if(!this->checkDataCorrects()){
		throw EngineException::EngineException("detected error in loaded engine configuration", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
	}
};

/*
CfgFile sctructure (line by line):
	I	- int					- engine moment of inertia
	M	- vector<unsigned int>	- engine torque (delimiter ',' without spaces)
	V	- vector<unsigned int>	- crankshaft rotation speed (delimiter ',' without spaces)
	To	- int - overheat temperature
	HM	- double				- coefficient of heating rate versus torque
	HV	- double				- coefficient of dependence of the heating rate on the speed of rotation of the crankshaft
	C	- double				- coefficient of dependence of the cooling rate on the temperature of the engine and the environment
*/

EngineConfig::EngineConfig(string config_path) {
	ifstream inputFile("engine.cfg");

	if (!inputFile.is_open()) {
		throw EngineException::EngineException("error of opening configuration file", EngineException::ENGINE_EXCEPTIONS::CREATION_FAILED);
		return;
	};

	string tempStr = "", tempStr2 = "";

	std::getline(inputFile, tempStr);
	try{
		this->I = std::stoul(tempStr);
	}
	catch (...) {
		throw EngineException::EngineException("error by reading I-parameter from configuration file", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
	}
	
	tempStr = "";
	tempStr2 = "";

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
				throw EngineException::EngineException("error by reading M(V) function from configuration file", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
				return;
			}

			try{
				value = std::stoul(tempStr);
			}
			catch (...) {
				throw EngineException::EngineException("error by reading M(V) function from configuration file", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
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
				throw EngineException::EngineException("error by reading M(V) function from configuration file", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
				return;
			}
			try {
				value = std::stoul(tempStr.substr(0, tempStr.find_first_of(',')));
			}
			catch (...) {
				throw EngineException::EngineException("error by reading M(V) function from configuration file", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
				return;
			}

			tempStr2.erase(0, tempStr2.find_first_of(',') + 1);
			tempStr.erase(0, tempStr.find_first_of(',') + 1);
		}

		this->MV.insert(std::pair<unsigned int, unsigned int>(key, value));

	} while (!tempStr.empty() && !tempStr2.empty());

	std::getline(inputFile, tempStr);
	try {
		this->To = std::stoi(tempStr);
	}
	catch (...) {
		throw EngineException::EngineException("error by reading To-parameter from configuration file", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
	}
	tempStr = "";

	std::getline(inputFile, tempStr);
	try {
		this->HM = std::stod(tempStr);
	}
	catch (...) {
		throw EngineException::EngineException("error by reading Hm-parameter from configuration file", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
	}
	tempStr = "";

	std::getline(inputFile, tempStr);
	try {
		this->HV = std::stod(tempStr);
	}
	catch (...) {
		throw EngineException::EngineException("error by reading Hv-parameter from configuration file", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
	}
	tempStr = "";

	std::getline(inputFile, tempStr);
	try {
		this->C = std::stod(tempStr);
	}
	catch (...) {
		throw EngineException::EngineException("error by reading C-parameter from configuration file", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
	}
	tempStr = "";

	inputFile.close();

	if (!this->checkDataCorrects()) {
		throw EngineException::EngineException("detected error in loaded engine configuration", EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG);
	}

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