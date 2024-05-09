cmake_minimum_required(VERSION 3.24)

project(vast_sim LANGUAGES CXX)

add_executable(vast_sim sim.cpp truck.cpp unloadingstation.cpp main.cpp)

set_property(TARGET vast_sim PROPERTY CXX_STANDARD 20)

