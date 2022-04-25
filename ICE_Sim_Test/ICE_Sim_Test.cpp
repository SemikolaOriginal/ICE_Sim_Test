#include <memory>
using std::shared_ptr;

#include <iostream>
using std::cout;
using std::endl;

#include "Engine.h"
#include "Tests.h"

int main(){
    shared_ptr<Engine> engine(new Engine("engine.cfg"));

    #ifdef _DEBUG
    cout << "shared_ptr: " << engine.use_count() << "(" << engine << ")" << endl;
    #endif // DEBUG

    if (!engine || engine->isWrong()){
        cout << "Error of engine object creation" << endl;
        system("pause");
        return EXIT_FAILURE;
    }
    
    int ambientTemperature = readAmbientTemperature();
    overheatEngineLinerTimeTest(engine, ambientTemperature);

    engine->cooling(ambientTemperature);
    overheatEngineAutoTimeTest(engine, ambientTemperature);

    system("pause");
    return EXIT_SUCCESS;
}