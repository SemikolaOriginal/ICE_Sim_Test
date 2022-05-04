#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <iostream>
using std::cout;
using std::endl;

#include "EngineException.h"
#include "TestException.h"

#include "Engine.h"
#include "Tests.h"

int main(){
    shared_ptr<Engine> engine = nullptr;
    try {
        engine = make_shared<Engine>("engine.cfg");
    }
    catch (std::exception& e) {
        std::cerr << "Exception catched : " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (EngineException& engineException) {
        if (engineException.getExceptionNumber() == EngineException::ENGINE_EXCEPTIONS::CREATION_FAILED) {
            cout << "Error in creation proccess of engine" << endl;
        } else if (engineException.getExceptionNumber() == EngineException::ENGINE_EXCEPTIONS::INVALID_CONFIG) {
            cout << "Error data in engine configuration file" << endl;
        }
        system("pause");
        return EXIT_FAILURE;
    }
    
    double ambientTemperature = static_cast<double>(readAmbientTemperature());

    shared_ptr<EngineTestStand> engineTestStand = nullptr;
    try {
        engineTestStand = make_shared<EngineTestStand>(engine);
    }
    catch (TestException& testException) {
        if (testException.getExceptionNumber() == TestException::TEST_EXCEPTIONS::INVALID_POINTER_TO_ENGINE) {
            cout << "Detected ivalid pointer to engine" << endl;
            return EXIT_FAILURE;
        }
    }
    catch (std::exception& e) {
        cout << "Exception catched : " << e.what() << endl;
        return EXIT_FAILURE;
    }

    engineTestStand->timeTypeByIntegration = TIME_TYPE_BY_INTEGRATION::CONST_TIME;
    engineTestStand->testEndpoint = TEST_ENDPOINT::BY_OVERHEAT | TEST_ENDPOINT::BY_REACHED_MAX_CRANKSHAFT_SPEED;
    engineTestStand->forcedStop = FORCED_STOP_OF_TEST::BY_DETECTED_LIMIT_OF_PARAM;

    try {
        engineTestStand->overheatEngineTest(ambientTemperature);
    }
    catch (EngineException& e) {
        cerr << e.getExceptionMessage() << endl;
    }

    engine->cooling(ambientTemperature);

    engineTestStand->testEndpoint = TEST_ENDPOINT::BY_OVERHEAT;
    engineTestStand->timeTypeByIntegration = TIME_TYPE_BY_INTEGRATION::FUNCTION_VARIABLE_TIME;
    engineTestStand->forcedStop = FORCED_STOP_OF_TEST::BY_DETECTED_LIMIT_OF_PARAM;

    try {
        engineTestStand->overheatEngineTest(ambientTemperature);
    }
    catch (EngineException& e) {
        cerr << e.getExceptionMessage() << endl;
    }

    system("pause");
    return EXIT_SUCCESS;
}