#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <memory>
using std::shared_ptr;

#include <string>
using std::string;

#include "Engine.h"
#include "Tests.h"

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

void overheatEngineLinerTimeTest(shared_ptr<Engine>& engine, int ambientTemperature) {
    engine->start(ambientTemperature);
    engine->deltaV = 1;

    cout << "Overheat engine linertime test" << endl;
    cout << "\tstart state: V = " << engine->currentV << ", T = " << engine->getCurrentTemperature() << endl;

    double deltaTime = 0.5; // not changeable in cycle
    double time = 0.0;

    while (engine->is_started() && !(engine->is_overheat()) && !engine->is_MaxCrankshaftSpeedReached()) {
        engine->update(deltaTime, ambientTemperature);
        time += deltaTime;

        #ifdef _DEBUG
        cout << "V = " << engine->currentV << "\tT = " << engine->getCurrentTemperature() << endl;
        #endif

        if (engine->is_overheat()){
            cout << "\tengine is overheat" << endl;
            engine->stop();
            break;
        }
    }

    if (!engine->is_overheat()) {
        cout << "\tengine isnt overheat" << endl;
    }

    engine->stop();
    cout << "\ttest duration: " << static_cast<int>(time) << "min " << static_cast<int>(time) % 60 << "sec" << endl;
}

void overheatEngineAutoTimeTest(shared_ptr<Engine>& engine, int ambientTemperature) {
    engine->start(ambientTemperature);
    engine->deltaV = 5;

    cout << "Overheat engine autotime test" << endl;
    cout << "\tstart state: V = " << engine->currentV << ", T = " << engine->getCurrentTemperature() << endl;

    double deltaTime = 0.0; // changeable in cycle, only initialization by zero
    double time = 0.0;
    while (engine->is_started() && !(engine->is_overheat()) && !engine->is_MaxCrankshaftSpeedReached()) {
        deltaTime = engine->getDeltaTime();
        time += deltaTime;
        engine->update(deltaTime, ambientTemperature);

        #ifdef _DEBUG
        cout << "V = " << engine->currentV << "\tT = " << engine->getCurrentTemperature() <<
            "\tdt = " << deltaTime << endl;
        #endif

        if (engine->is_overheat()) {
            cout << "\tengine is overheat" << endl;
            engine->stop();
        }
    }

    if (!engine->is_overheat()) {
        cout << "\tengine isnt overheat" << endl;
    }

    engine->stop();
    cout << "\ttest duration: " << static_cast<int>(time) << "min " << static_cast<int>(time) % 60 << "sec" << endl;
}