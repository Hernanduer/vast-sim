#include "truck.h"

#include "constants.h"
#include "simstate.h"
#include "unloadingstation.h"

#include <memory>
#include <random>

Truck::Truck(int id) : SimObject(id), actionTicksRemaining{0}, truckState{TO_MINING}, assignedStation{nullptr},
    stats{.ticksForAction = {{TO_MINING, 0}, {MINING, 0}, {TO_UNLOAD, 0}, {IDLING, 0}, {UNLOADING, 0}}, .trips = 0}
{
}

void Truck::doAction(SimState& state, ActionSource source)
{
    // The unloading station controls the action of the truck while unloading. This prevents the unloading station from double dipping on time
    // allotments by potentially having a truck unload, release the station, the station picks another truck and then that truck gets its action period.
    if ((source == SimObject::SIM && truckState == UNLOADING) || lastActivity == state.tickTime)
    {
        return;
    }

    lastActivity = state.tickTime;
    stats.ticksForAction[truckState]++;
    // If an action is underway, no need to do any other logic
    if (actionTicksRemaining > 0)
    {
        actionTicksRemaining -= 1;
        return;
    }

    switch(truckState)
    {
        case TO_MINING:
        {
            startMining();
            break;
        }
        case MINING:
        {
            // Find the unloading station with the lowest wait time and assign itself to that station before beginning to travel there
            for (auto ul : state.unloadingStations)
            {
                if (assignedStation == nullptr || assignedStation->getWaitTime() > ul->getWaitTime())
                {
                    assignedStation = ul;
                }
            }
            assignedStation->assignTruck(std::dynamic_pointer_cast<Truck>(state.currentActionObj));
            truckState = TO_UNLOAD;
            goToLocation();
            break;
        }
        case TO_UNLOAD:
        {
            truckState = IDLING;
            assignedStation->moveTruckToIdle(std::dynamic_pointer_cast<Truck>(state.currentActionObj));
            break;
        }
        case UNLOADING:
        {
            stats.trips += 1;
            assignedStation->unloadDone();
            truckState = TO_MINING;
            goToLocation();
            break;
        }
    }
}

void Truck::printStats(std::ostream& outputStream, const SimState& state) const
{
    outputStream << "Stats for truck #" << identifier << std::endl
        << "  Ticks Mining: " << stats.ticksForAction.at(MINING) << " (" << ((double)stats.ticksForAction.at(MINING) / (double)(state.tickTime-2) * 100) << "%)" << std::endl
        << "  Ticks Travelling: " << stats.ticksForAction.at(TO_MINING) + stats.ticksForAction.at(TO_UNLOAD) << std::endl
        << "  Ticks Idling: " << stats.ticksForAction.at(IDLING) << std::endl
        << "  Ticks Unloading: " << stats.ticksForAction.at(UNLOADING) << std::endl
        << "  Total # of Unloads: " << stats.trips << std::endl;
}

void Truck::startUnload()
{
    static constexpr unsigned int unloadTime =
        std::chrono::duration<unsigned int, SimState::TickType::period>(TimeConstants::UNLOAD_TIME).count();
    actionTicksRemaining = unloadTime;
    truckState = UNLOADING;
}

void Truck::startMining()
{
    // Determine the number of ticks in the given minimum and maximum time constants as a constexpr
    static constexpr unsigned int minTime =
        std::chrono::duration<unsigned int, SimState::TickType::period>(TimeConstants::MIN_MINE_TIME).count();
    static constexpr unsigned int maxTime =
        std::chrono::duration<unsigned int, SimState::TickType::period>(TimeConstants::MAX_MINE_TIME).count();
    static constexpr unsigned int timeDiff = maxTime - minTime;

    // Get a random number of ticks to start a new mining operation
    actionTicksRemaining = (std::rand() % timeDiff) + minTime;
    truckState = MINING;
}

void Truck::goToLocation()
{
    static constexpr unsigned int travelTime =
        std::chrono::duration<unsigned int, SimState::TickType::period>(TimeConstants::TRAVEL_TIME).count();
    actionTicksRemaining = travelTime;
}
