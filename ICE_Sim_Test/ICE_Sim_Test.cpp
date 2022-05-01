#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <iostream>
using std::cout;
using std::endl;

#include "EngineException.h"
#include "Engine.h"
#include "Tests.h"

int main(){
    shared_ptr<Engine> engine = nullptr;
    try {
        engine = make_shared<Engine>("engine.cfg");
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

    #ifdef _DEBUG
    cout << "shared_ptr: " << engine.use_count() << "(" << engine << ")" << endl;
    #endif // DEBUG
    
    int ambientTemperature = readAmbientTemperature();
    overheatEngineLinerTimeTest(engine, ambientTemperature);

    engine->cooling(ambientTemperature);
    overheatEngineAutoTimeTest(engine, ambientTemperature);

    system("pause");
    return EXIT_SUCCESS;
}