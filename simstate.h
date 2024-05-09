#pragma once

#include <chrono>
#include <memory>
#include <queue>
#include <vector>

// Forward declare to prevent pulling in excess depenencies
class SimObject;
class Truck;
class UnloadingStation;

// State tracker for allowing sim data to be passed into objects during processing
struct SimState
{
    SimState() : tickTime{0} {}

    // Current tick time of the simulation - one tick is one second, could be configurable in future
    int tickTime;
    // Tracking what current object is performing an action
    std::shared_ptr<SimObject> currentActionObj;
    // Vector of all trucks in sim
    std::vector<std::shared_ptr<Truck>> trucks;
    // Vector of all unloading stations in sim
    std::vector<std::shared_ptr<UnloadingStation>> unloadingStations;

    // Queue tracking trucks waiting for an empty unloading station
    std::queue<std::shared_ptr<Truck>> waitingTrucks;
    // Queue tracking empty unloading station if there are no waiting trucks
    std::queue<std::shared_ptr<UnloadingStation>> emptyUnloadingStations;

    // Provide a unified place to track how long a tick is worth - could be made configurable using templates
    using TickType = std::chrono::seconds;
};