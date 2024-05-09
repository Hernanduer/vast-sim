#include <chrono>

/*
All of these constants could be made configurable, but would need to be split between absolute value and
time unit, and some functions would need to be templatized.
*/
namespace TimeConstants
{
    using namespace std::chrono_literals;
    const auto SIMULATION_TIME = 72h;
    const auto MIN_MINE_TIME = 1h;
    const auto MAX_MINE_TIME = 5h;
    const auto TRAVEL_TIME = 30min;
    const auto UNLOAD_TIME = 5min;
}