cmake_minimum_required(VERSION 3.22)

project(Dueling)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(./include DIR_SRCS)

add_executable(find_best_c find_best_c.cpp ${DIR_SRCS})
add_executable(test test.cpp ${DIR_SRCS})
add_executable(heuristic HeuristicSearch.cpp ${DIR_SRCS})
add_executable(find_state_evolution find_state_evolution.cpp ${DIR_SRCS})
add_executable(iteration_probability_original iteration_probability_original.cpp)
add_executable(maximum_probability_all_M_original maximum_probability_all_M_original.cpp)