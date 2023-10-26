// File: Predator.h
#ifndef PREDATOR_H
#define PREDATOR_H

class Predator {
public:
    Predator(int initialNumber, int consumptionRate, int reproductionRate);
    int getNumber() const;
    void consumePrey(int& preyNumber);
    void reproduce();
private:
    int number;
    int consumptionRate;
    int reproductionRate;
};

#endif

