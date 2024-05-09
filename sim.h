#pragma once

#include "simstate.h"

#include <chrono>
#include <memory>
#include <vector>

// Forward declare to prevent pulling in excess depenencies
class SimObject;

/*
Primary simulation class, handling initialization of the sim and performing the actual simulation
*/
class Sim
{
public:
    Sim(unsigned int numTrucks, unsigned int numStations);

    // Runs the simulation for a given amount of time
    void runSim(std::chrono::duration<unsigned int> timeToRun);

    // Perform a diagnostic print of all simulated objects to the given outputStream - can support direct print to file with some minor updates to the call
    void printAll(std::ostream& outputStream) const;

private:
    // Simulation state tracking structure
    SimState state;
    // Store of all simulated objects
    std::vector<std::shared_ptr<SimObject>> objects;
    
    // Inclusion of a map of the layout for the simulation seems like a critical feature to add that would
    // allow for accurate tracking of vehicles across distances.
};