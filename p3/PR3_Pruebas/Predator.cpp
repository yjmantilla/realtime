// File: Predator.cpp
#include "Predator.h"
#include <algorithm>

Predator::Predator(int initialNumber, int consumptionRate, int reproductionRate)
    : number(initialNumber), consumptionRate(consumptionRate), reproductionRate(reproductionRate) {}

int Predator::getNumber() const {
    return number;
}

void Predator::consumePrey(int& preyNumber) {
    int predatorsThatEat = std::min(preyNumber / consumptionRate, number);
    number = predatorsThatEat;
    preyNumber -= predatorsThatEat * consumptionRate;
}

void Predator::reproduce() {
    number += (number / 2) * reproductionRate;  // For simplicity, assuming every two predators produce reproductionRate new predators
}

