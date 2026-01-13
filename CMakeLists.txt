cmake_minimum_required(VERSION 3.15)
project(SemCal VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_EXAMPLES "Build example solvers" ON)
option(BUILD_TESTS "Build test suite" OFF)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SEMCAL_SOURCES
    src/core/model.cpp
    src/core/formula.cpp
    src/core/semantics.cpp
    src/domain/abstract_domain.cpp
    src/domain/concretization.cpp
    src/domain/galois.cpp
    src/domain/top_element.cpp
    src/state/semantic_state.cpp
    src/operators/restrict.cpp
    src/operators/decompose.cpp
    src/operators/infeasible.cpp
    src/operators/relax.cpp
    src/operators/refine.cpp
    src/operators/project.cpp
    src/operators/lift.cpp
    src/orchestration/pipeline.cpp
    src/orchestration/strategy.cpp
    src/util/result.cpp
)

# Header files
set(SEMCAL_HEADERS
    include/semcal.h
    include/core/model.h
    include/core/formula.h
    include/core/semantics.h
    include/domain/abstract_domain.h
    include/domain/concretization.h
    include/domain/galois.h
    include/state/semantic_state.h
    include/operators/restrict.h
    include/operators/decompose.h
    include/operators/infeasible.h
    include/operators/relax.h
    include/operators/refine.h
    include/operators/project.h
    include/operators/lift.h
    include/orchestration/pipeline.h
    include/orchestration/strategy.h
    include/util/result.h
)

# Create library
add_library(semcal STATIC ${SEMCAL_SOURCES} ${SEMCAL_HEADERS})

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
install(TARGETS semcal
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)
