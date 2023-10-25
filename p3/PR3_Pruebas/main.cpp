// File: main.cpp
#include <iostream>
#include "Prey.h"
#include "Predator.h"
#include "TimeSpec.h"

int main(int argc, char* argv[]) {
    if(argc != 9) {
        std::cerr << "Usage: " << argv[0] << " <prey_number> <predator_number> <time_step> <run_for> <inspect_time> <prey reproduction per step> <predator consumption per step> <predator reproduction per step>\n";
        return 1;
    }

    Prey prey(std::stoi(argv[1]), std::stoi(argv[6]));
    Predator predator(std::stoi(argv[2]), std::stoi(argv[7]), std::stoi(argv[8]));
    TimeSpec timeSpec(std::stoi(argv[3]), std::stoi(argv[4]), std::stoi(argv[5]));

    for(int t = 0; t < timeSpec.getRunFor(); t += timeSpec.getTimeStep()) {
    
        int preyNumber = prey.getNumber();
        predator.consumePrey(preyNumber);
        prey.setNumber(preyNumber);  // Update prey number
        prey.reproduce();
        predator.reproduce();

        if(t % timeSpec.getInspectTime() == 0) {
            std::cout << "Time: " << t << ", Prey: " << prey.getNumber() << ", Predator: " << predator.getNumber() << '\n';
        }
    }

    return 0;
}

