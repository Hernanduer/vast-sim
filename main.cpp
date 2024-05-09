#include "constants.h"
#include "sim.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        // Could make argument handling nicer, or just add some configuration file loading instead
        std::cout << "Please provide all arguments as described below:" << std::endl;
        std::cout << "  Number of trucks" <<
            std::endl << "  Number of unloading stations" << std::endl;
        return 1;
    }
    unsigned int numTrucks = 0;
    unsigned int numStations = 0;
    // Ensure only numbers were passed in
    try
    {
        numTrucks = std::stoul(argv[1]);
        numStations = std::stoul(argv[2]);
    }
    catch (std::exception e)
    {
        std::cout << "Only digital inputs are allowed" << std::endl;
        return 1;
    }

    std::cout << "Running sim with " <<
        numTrucks << " truck(s) and " <<
        numStations << " unloading station(s)" << std::endl;

    Sim simulation(numTrucks, numStations);
    simulation.runSim(TimeConstants::SIMULATION_TIME);
    simulation.printAll(std::cout);
}