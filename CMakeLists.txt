cmake_minimum_required(VERSION 3.21)
project(SatSolver)

set(CMAKE_CXX_STANDARD 11)

add_executable(SatSolver main.cpp Global.h start.cpp CnfParser.cpp DPLLSolver.cpp)
