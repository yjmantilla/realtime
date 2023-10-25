#include "Prey.h"

Prey::Prey(int initialNumber, int reproductionRate) : number(initialNumber), reproductionRate(reproductionRate) {}

int Prey::getNumber() const {
    return number;
}

void Prey::reproduce() {
    number += (number / 2) * reproductionRate;
}

void Prey::setNumber(int newNumber) {
    number = newNumber;
}

