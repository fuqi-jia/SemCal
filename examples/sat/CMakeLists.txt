cmake_minimum_required(VERSION 3.15)

add_executable(sat_solver sat_solver.cpp)
target_link_libraries(sat_solver semcal)
