#include "sim.h"

#include "truck.h"
#include "unloadingstation.h"

Sim::Sim(unsigned int numTrucks, unsigned int numStations) :
    state{}
{
    // Save some runtime reserving rather than requiring several reallocations of the vectors
    objects.reserve(numTrucks + numStations);
    state.trucks.reserve(numTrucks);
    state.unloadingStations.reserve(numStations);

    for (int i = 0; i < numTrucks; ++i)
    {
        auto sptr = std::make_shared<Truck>(i);
        state.trucks.push_back(sptr);
        objects.push_back(sptr);
    }

    for (int i = numTrucks; i < numStations+numTrucks; ++i)
    {
        auto sptr = std::make_shared<UnloadingStation>(i);
        state.unloadingStations.push_back(sptr);
        objects.push_back(sptr);
    }
}

void Sim::runSim(std::chrono::duration<unsigned int> timeToRun)
{
    // Convert the received duration into seconds - add 1 to account for bad values == 0, so starting at 1
    int numOfTicks = std::chrono::duration<unsigned int, SimState::TickType::period>(timeToRun).count()+1;

    for (state.tickTime = 1; state.tickTime < numOfTicks; ++state.tickTime)
    {
        for (auto obj : objects)
        {
            state.currentActionObj = obj;
            obj->doAction(state, SimObject::SIM);
        }
    }
}

void Sim::printAll(std::ostream& outputStream) const
{
    for (auto& obj : objects)
    {
        obj->printStats(outputStream, state);
    }
}