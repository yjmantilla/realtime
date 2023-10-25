// File: TimeSpec.cpp
#include "TimeSpec.h"

TimeSpec::TimeSpec(int timeStep, int runFor, int inspectTime) :
    timeStep(timeStep), runFor(runFor), inspectTime(inspectTime) {}

int TimeSpec::getTimeStep() const {
    return timeStep;
}

int TimeSpec::getRunFor() const {
    return runFor;
}

int TimeSpec::getInspectTime() const {
    return inspectTime;
}

