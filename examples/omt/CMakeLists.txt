cmake_minimum_required(VERSION 3.15)

add_executable(omt_solver omt_solver.cpp)
target_link_libraries(omt_solver semx)
