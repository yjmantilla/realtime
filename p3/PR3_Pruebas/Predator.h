// File: Predator.h
#ifndef PREDATOR_H
#define PREDATOR_H

class Predator {
public:
    Predator(int initialNumber, int consumptionRate, int reproductionRate);  // Add reproductionRate parameter
    int getNumber() const;
    void consumePrey(int& preyNumber);
    void reproduce();  // Add this method
private:
    int number;
    int consumptionRate;
    int reproductionRate;  // Add this member variable
};

#endif

