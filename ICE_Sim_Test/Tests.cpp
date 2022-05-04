#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <memory>
using std::shared_ptr;

#include <string>
using std::string;

#include <exception>
using std::exception;
using std::logic_error;

#include "Engine.h"
#include "Tests.h"
#include "TestException.h"
#include "EngineException.h"

TEST_ENDPOINT operator|(const TEST_ENDPOINT& left, const TEST_ENDPOINT& right) {
    return static_cast<TEST_ENDPOINT>(static_cast<unsigned int>(left) | static_cast<unsigned int>(right));
}

void EngineTestStand::printTestInfo(){
    cout << "Test info:" << endl;

    if (this->timeTypeByIntegration == TIME_TYPE_BY_INTEGRATION::CONST_TIME) {
        cout << "\tlineartime" << endl;
    }
    else if (this->timeTypeByIntegration == TIME_TYPE_BY_INTEGRATION::FUNCTION_VARIABLE_TIME) {
        cout << "\tautotime" << endl;
    }

    if (this->testEndpoint == (TEST_ENDPOINT::BY_REACHED_MAX_CRANKSHAFT_SPEED | TEST_ENDPOINT::BY_OVERHEAT)) {
        cout << "\tstop by reached max crankshaft speed or overheat" << endl;
    }
    else if (this->testEndpoint == TEST_ENDPOINT::BY_REACHED_MAX_CRANKSHAFT_SPEED) {
        cout << "\tstop by reached max crankshaft speed" << endl;
    }
    else if (this->testEndpoint == TEST_ENDPOINT::BY_OVERHEAT) {
        cout << "\tstop by overheat" << endl;
    }

    cout << "\tstart state: V = " << this->engine->currentV << ", T = " << this->engine->getCurrentTemperature() << endl;

}

void EngineTestStand::overheatEngineTest(double ambientTemperature){
    this->engine->start(ambientTemperature);
    this->engine->deltaV = 1;

    this->printTestInfo();

    double deltaTime = 0.5; // not changeable for TIME_TYPE_BY_INTEGRATION::CONST_TIME
    double time = 0.0;

    while (this->engine->is_started()){
        if (this->timeTypeByIntegration == TIME_TYPE_BY_INTEGRATION::CONST_TIME) {
            //
        }
        else if (this->timeTypeByIntegration == TIME_TYPE_BY_INTEGRATION::FUNCTION_VARIABLE_TIME) {
            try {
                deltaTime = this->engine->getDeltaTime();
            }
            catch (const logic_error& le) {
                throw EngineException(le.what(), EngineException::ENGINE_EXCEPTIONS::SIMULATION_ERROR);
            }
        }
        time += deltaTime;

        #ifdef _DEBUG
        cout << "V = " << this->engine->currentV <<
            "\tTe = " << this->engine->getCurrentTemperature() <<
            "\tTa = " << ambientTemperature <<
            "\tdt = " << deltaTime << endl;
        #endif


        if (this->forcedStop == FORCED_STOP_OF_TEST::BY_DETECTED_LIMIT_OF_PARAM &&
            this->engine->getCurrentEngineTempIncrement(deltaTime, ambientTemperature) < MIN_DELTA_TEMPERATURE) {
            break;
        }

        if (this->testEndpoint == (TEST_ENDPOINT::BY_REACHED_MAX_CRANKSHAFT_SPEED | TEST_ENDPOINT::BY_OVERHEAT) &&
            (this->engine->is_MaxCrankshaftSpeedReached() ||
            this->engine->is_overheat())
            ) {
            break;
        }
        else if(this->testEndpoint == TEST_ENDPOINT::BY_REACHED_MAX_CRANKSHAFT_SPEED && 
            this->engine->is_MaxCrankshaftSpeedReached()){
            break;
        }
        else if (this->testEndpoint == TEST_ENDPOINT::BY_OVERHEAT && this->engine->is_overheat()) {
            break;
        }

        this->engine->update(deltaTime, ambientTemperature);
    }

    if(this->engine->is_overheat()) {
        cout << "\tengine is overheat" << endl;
    }

    if (!this->engine->is_overheat()) {
        cout << "\tengine isnt overheat" << endl;
    }

    this->engine->stop();
    cout << "\ttest duration: " << static_cast<int>(time / 60) << "min " << static_cast<int>(time) % 60 << "sec" << endl;
}

EngineTestStand::EngineTestStand(shared_ptr<Engine> setEngine) :
    engine(setEngine) {
    if (!setEngine) {
        throw TestException("detected nullptr to engine", TestException::TEST_EXCEPTIONS::INVALID_POINTER_TO_ENGINE);
    }
}

int readAmbientTemperature() {
    /*
    Input by user:
        Ta - int - ambient temperature
    */
    int Ta = 0;
    cout << "Input ambient temperature: ";

    string buf = "";

INPUT:
    buf = "";
    getline(cin, buf);

    try {
        Ta = std::stoi(buf);
    }
    catch (...) {
        cout << "You did input not a integer number\nTry input: " << endl;
        goto INPUT;
    }
    return Ta;
}