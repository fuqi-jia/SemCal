cmake_minimum_required(VERSION 3.15)

# SMT Solver example (Satisfiability Modulo Theories)
add_executable(smt_solver smt_solver.cpp)
target_link_libraries(smt_solver semx)

# SAT Solver example (pure Boolean satisfiability)
add_executable(sat_solver sat_solver.cpp)
target_link_libraries(sat_solver semx)
