cmake_minimum_required(VERSION 3.15)
project(SemX VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_EXAMPLES "Build example solvers" ON)
option(BUILD_TESTS "Build test suite" OFF)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SEMX_SOURCES
    # SemCal: Axiomatic Semantic Calculus
    src/semcal/core/model.cpp
    src/semcal/core/partial_model.cpp
    src/semcal/core/formula.cpp
    src/semcal/core/semantics.cpp
    src/semcal/domain/abstract_domain.cpp
    src/semcal/domain/concretization.cpp
    src/semcal/domain/galois.cpp
    src/semcal/domain/top_element.cpp
    src/semcal/state/semantic_state.cpp
    src/semcal/operators/restrict.cpp
    src/semcal/operators/decompose.cpp
    src/semcal/operators/infeasible.cpp
    src/semcal/operators/relax.cpp
    src/semcal/operators/refine.cpp
    src/semcal/operators/shadow.cpp
    src/semcal/operators/lift.cpp
    src/semcal/backends/cad_backend.cpp
    src/semcal/backends/lp_backend.cpp
    src/semcal/backends/icp_backend.cpp
    src/semcal/backends/cad_stub.cpp
    src/semcal/backends/lp_stub.cpp
    src/semcal/operators/infeasible_cad.cpp
    src/semcal/operators/decompose_cad.cpp
    src/semcal/operators/infeasible_lp.cpp
    src/semcal/util/result.cpp
    # SemKernel: Verified Semantic Kernel
    src/semkernel/kernel.cpp
    # SemSearch: Generic Search Engine
    src/semsearch/search_engine.cpp
    # SemSolver: Concrete Solver Instances
    src/semsolver/solver.cpp
    # Legacy strategies (migrated to semsolver)
    src/semsolver/strategies/pipeline.cpp
    src/semsolver/strategies/strategy.cpp
)

# Header files
set(SEMX_HEADERS
    include/semx.h
    # SemCal: Axiomatic Semantic Calculus
    include/semcal/core/model.h
    include/semcal/core/partial_model.h
    include/semcal/core/formula.h
    include/semcal/core/semantics.h
    include/semcal/domain/abstract_domain.h
    include/semcal/domain/concretization.h
    include/semcal/domain/galois.h
    include/semcal/domain/top_element.h
    include/semcal/state/semantic_state.h
    include/semcal/operators/operator.h
    include/semcal/operators/restrict.h
    include/semcal/operators/decompose.h
    include/semcal/operators/infeasible.h
    include/semcal/operators/relax.h
    include/semcal/operators/refine.h
    include/semcal/operators/shadow.h
    include/semcal/operators/lift.h
    include/semcal/backends/cad_backend.h
    include/semcal/backends/lp_backend.h
    include/semcal/backends/icp_backend.h
    include/semcal/util/op_result.h
    # SemKernel: Verified Semantic Kernel
    include/semkernel/kernel.h
    # SemSearch: Generic Search Engine
    include/semsearch/search_engine.h
    # SemSolver: Concrete Solver Instances
    include/semsolver/solver.h
    # Legacy strategies (migrated to semsolver)
    include/semsolver/strategies/pipeline.h
    include/semsolver/strategies/strategy.h
)

# Create library
add_library(semx STATIC ${SEMX_SOURCES} ${SEMX_HEADERS})

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# Installation
install(TARGETS semx
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
