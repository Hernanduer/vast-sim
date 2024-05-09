#include "unloadingstation.h"

#include "constants.h"
#include "simstate.h"
#include "truck.h"

#include <iostream>

UnloadingStation::UnloadingStation(int id) : SimObject(id), activeTruck{nullptr},
    stats{.ticksIdle=0, .ticksUnloading=0, .totalUnloaded=0}
{}

void UnloadingStation::doAction(SimState& state, ActionSource source)
{
    if (activeTruck != nullptr)
    {
        stats.ticksUnloading += 1;
        activeTruck->doAction(state, SimObject::OBJECT);
    }
    else
    {
        stats.ticksIdle += 1;
    }
}

void UnloadingStation::printStats(std::ostream& outputStream, const SimState& state) const
{
    outputStream << "Stats for unloading station #" << identifier << std::endl
        << "  Ticks Unloading: " << stats.ticksUnloading << " (" << ((double)stats.ticksUnloading / (double)(state.tickTime-2) * 100) << "%)" << std::endl
        << "  Ticks Idling: " << stats.ticksIdle << std::endl
        << "  Total # of Unloads: " << stats.totalUnloaded << std::endl;
}

void UnloadingStation::assignTruck(std::shared_ptr<Truck> truck)
{
    travellingTrucks.push_back(truck);
}

void UnloadingStation::moveTruckToIdle(std::shared_ptr<Truck> truck)
{
    // Loop through all travelling trucks to find the current one - this could be improved with a map-vector pair to speed up lookup times if there are huge amounts of traffic
    for (auto itr = travellingTrucks.begin(); itr != travellingTrucks.end(); ++itr)
    {
        if (truck == *itr)
        {
            // Either move the truck to activeTruck, or put it in the idling queue
            if (activeTruck != nullptr)
            {
                idlingTrucks.push(truck);
            }
            else
            {
                activeTruck = truck;
                activeTruck->startUnload();
            }
            travellingTrucks.erase(itr);
            return;
        }
    }
    std::cerr << "Attempted to move truck to idle that was not assigned!" << std::endl;
}

void UnloadingStation::unloadDone()
{
    stats.totalUnloaded += 1;
    // Determine if there's a truck idling and set it up as the next activeTruck if so
    if (idlingTrucks.size() > 0)
    {
        activeTruck = idlingTrucks.front();
        idlingTrucks.pop();
        activeTruck->startUnload();
        return;
    }
    activeTruck = nullptr;
}

unsigned int UnloadingStation::getWaitTime() const
{
    // If the station is completely empty, then return
    if (activeTruck == nullptr && travellingTrucks.size() == 0)
    {
        return 0;
    }
    // Get the remaining time for the active truck and then all the trucks already local but idling
    // NOTE: Some of the logic here relies on the travel time being the exact same for all vehicles; some improvements would
    // be necessary if that assumption were to be changed.
    unsigned int startAmt = 0;
    if (activeTruck != nullptr)
    {
        startAmt = activeTruck->getActionTicksRemaining();
    }
    std::chrono::duration<unsigned int, SimState::TickType::period> waitTime(startAmt);
    waitTime += idlingTrucks.size() * TimeConstants::UNLOAD_TIME;

    for (auto truck : travellingTrucks)
    {
        // If the station will be vacant while waiting for the truck to arrive, reset the waitTime to the arrival of the truck
        if (truck->getActionTicksRemaining() > waitTime.count())
        {
            waitTime = std::chrono::duration<unsigned int, SimState::TickType::period>(truck->getActionTicksRemaining());
        }
        waitTime += TimeConstants::UNLOAD_TIME;
    }
    return waitTime.count();
}