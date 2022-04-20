#include <memory>
using std::shared_ptr;

#include <iostream>
using std::cout;
using std::endl;

#include "Engine.h"

extern int readAmbientTemperature();
extern void overheatEngineLinerTimeTest(shared_ptr<Engine>& engine, int ambientTemperature);
extern void overheatEngineAutoTimeTest(shared_ptr<Engine>& engine, int ambientTemperature);

int main(){
    shared_ptr<Engine> engine = std::make_shared<Engine>("engine.cfg");

    #ifdef DEBUG
    cout << "shared_ptr: " << engine.use_count() << "(" << engine << ")" << endl;
    #endif // DEBUG

    if (!engine || engine->isWrong()){
        cout << "Error of engine object creation" << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }
    
    int ambientTemperature = readAmbientTemperature();
    overheatEngineLinerTimeTest(engine, ambientTemperature);

    engine->cooling(ambientTemperature);
    overheatEngineAutoTimeTest(engine, ambientTemperature);

    system("pause");
    return EXIT_SUCCESS;
}