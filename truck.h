#pragma once

#include "simobject.h"

#include <map>
#include <memory>

class SimState;
class UnloadingStation;

class Truck : public SimObject
{
    enum TruckState
    {
        TO_MINING,
        MINING,
        TO_UNLOAD,
        IDLING,
        UNLOADING
    };
public:
    Truck(int id);

    virtual void doAction(SimState& state, ActionSource source) override;

    virtual void printStats(std::ostream& outputStream, const SimState& state) const override;

    auto getActionTicksRemaining() const { return actionTicksRemaining; }

    void startUnload();

private:
    // Determine the time for mining and start the mining process
    void startMining();
    // Go to a specific location - no actual location necessary, just sets the location action time
    void goToLocation();

    // Tracks how much longer the given action will take
    unsigned int actionTicksRemaining;
    // Indicates the current state of the truck
    TruckState truckState;
    // Pointer to the chosen unloading station
    std::shared_ptr<UnloadingStation> assignedStation;

    // Stats container for tracking performance
    struct
    {
        std::map<TruckState, unsigned int> ticksForAction;
        unsigned int trips;
    } stats;
};