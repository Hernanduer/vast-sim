#pragma once

#include "simobject.h"

#include <memory>
#include <queue>
#include <vector>

class Truck;

/*
Class representing an unload station, performing required sim activities of that object type.
*/
class UnloadingStation : public SimObject
{
public:

    UnloadingStation(int id);
    // Perform the unloading station action, currently only calling the activeTruck's doAction
    virtual void doAction(SimState& state, ActionSource source) override;
    // Print unloading station stats
    virtual void printStats(std::ostream& outputStream, const SimState& state) const override;

    // Assigns a given truck to the station
    void assignTruck(std::shared_ptr<Truck> truck);
    // Moves a given truck from the travelling list to the idle list and activates if possible
    void moveTruckToIdle(std::shared_ptr<Truck> truck);
    // Notifies the unloading station that a truck has completed its unload cycle
    void unloadDone();

    // Calculates how long the current wait time is for a given station
    unsigned int getWaitTime() const;

private:
    // Pointer to the currently unloading truck
    std::shared_ptr<Truck> activeTruck;
    // Queue representing all trucks present at the unloading site but waiting their turn
    std::queue<std::shared_ptr<Truck>> idlingTrucks;
    // Vector representing all trucks on their way to the unloading site, but not currently at it
    std::vector<std::shared_ptr<Truck>> travellingTrucks;

    // Stats container for tracking performance
    struct
    {
        unsigned int ticksIdle;
        unsigned int ticksUnloading;
        unsigned int totalUnloaded;
    } stats;
};