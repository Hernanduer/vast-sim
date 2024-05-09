#pragma once

#include <ostream>

class SimState;

/*
Interface class for the simulation objects to simplify basic function access points.
*/
class SimObject
{
public:
    enum ActionSource
    {
        SIM,
        OBJECT
    };

    SimObject(unsigned int id) : identifier{id}, lastActivity{0} {}

    virtual void doAction(SimState& state, ActionSource source) = 0;
    virtual void printStats(std::ostream& outputStream, const SimState& state) const = 0;

protected:
    // Unique identifier for each simulation object
    unsigned int identifier;
    // Tracks on what tick the last doAction call was done to prevent double dipping
    unsigned int lastActivity;    
};