cmake_minimum_required(VERSION 3.9)
project(StudiaProsteProjekty)

set(CMAKE_CXX_STANDARD 11)

add_executable(KnapsackProblemGreedy src/KnapsackProblemGreedy.cpp)
add_executable(SatisfiabilityProblem src/Sat.cpp)
add_executable(KnapsackProblemDynamic src/KnapsackProblemDynamic.cpp)
add_executable(EightQueenPuzzle src/EightQueenPuzzle.cpp)