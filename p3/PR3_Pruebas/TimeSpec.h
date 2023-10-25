// File: TimeSpec.h
#ifndef TIMESPEC_H
#define TIMESPEC_H

class TimeSpec {
public:
    TimeSpec(int timeStep, int runFor, int inspectTime);
    int getTimeStep() const;
    int getRunFor() const;
    int getInspectTime() const;
private:
    int timeStep;
    int runFor;
    int inspectTime;
};

#endif

