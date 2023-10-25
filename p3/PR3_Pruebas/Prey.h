#ifndef PREY_H
#define PREY_H

class Prey {
public:
    Prey(int initialNumber, int reproductionRate);
    int getNumber() const;
    void reproduce();
    void setNumber(int newNumber);
private:
    int number;
    int reproductionRate;
};

#endif

