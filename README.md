# Moon Miner Simulation
This project performs a short 72 hour simulation of a moon mining operation. All code is written per given requirements.
## Requirements
A compiler supporting C++20 or higher
CMake 3.24+

## How to Build
If on a linux machine, run the `make` command and the resultant binary can be found as `build/vast_sim`

## How to Run
Run the binary and pass in the two parameters, the first set to the number of trucks and the second to the number of unloading stations.

Ex: `./build/vast_sim 100 3` will run a simulation with 100 trucks and 3 unloading stations.

Results will appear immediately once the simulation is complete. Recommend routing the output to a file if using large numbers.

## Testing
Development and testing was performed on Fedora 33 using gcc 11.3.1 and cmake 3.24.2. Earlier or later versions of either library may also compile but were not tested.

## Results
Some general output can be found in the `results` directory.