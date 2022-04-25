#pragma once

int readAmbientTemperature();

void overheatEngineLinerTimeTest(shared_ptr<Engine>& engine, int ambientTemperature);
void overheatEngineAutoTimeTest(shared_ptr<Engine>& engine, int ambientTemperature);