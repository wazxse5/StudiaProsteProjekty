cmake_minimum_required(VERSION 3.9)
project(StudiaProsteProjekty)

set(CMAKE_CXX_STANDARD 11)

add_executable(KnapsackProblemGreedy KnapsackProblemGreedy.cpp )
add_executable(SatisfiabilityProblem Sat.cpp)
add_executable(KnapsackProblemDynamic KnapsackProblemDynamic.cpp)
add_executable(EightQueenPuzzle EightQueenPuzzle.cpp)