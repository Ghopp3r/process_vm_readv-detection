cmake_minimum_required(VERSION 3.10)
project(detector_demo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(detector_demo
		src/demo.cpp
		src/detector.cpp)

add_executable(detector_demo_standoff2
		src/standoff2/demo.cpp
		src/detector.cpp)

add_executable(cheat_simulator
		src/cheat.cpp)

add_executable(cheat_simulator_standoff2
		src/standoff2/cheat.cpp)

target_compile_definitions(detector_demo PRIVATE _GNU_SOURCE)
target_compile_definitions(detector_demo_standoff2 PRIVATE _GNU_SOURCE)
target_compile_definitions(cheat_simulator PRIVATE _GNU_SOURCE)
target_compile_definitions(cheat_simulator_standoff2 PRIVATE _GNU_SOURCE)