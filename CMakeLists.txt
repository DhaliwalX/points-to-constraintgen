cmake_minimum_required(VERSION 3.1)


find_package(LLVM CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(LLVMConfig)

# include these after loading llvm-config
include(cmake/cmake_functions.cmake)
include(cmake/PointsToFunctions.cmake)
include(cmake/FindPythonModule.cmake)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_ROOT}/lib/cmake/llvm")
message(STATUS "Looking for LLVM cmake modules: ${LLVM_ROOT}/lib/cmake/llvm")

set(POINTS_TO_INCLUDE_DIRECTORY CACHE INTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(./include)
add_definitions(${LLVM_DEFINITIONS})

add_subdirectory(./include)
add_subdirectory(./src)
add_subdirectory(./samples)
add_subdirectory(./tests)

file (GLOB test_sources ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.ll)

# set(args -S -c -emit-llvm -w)
# set(clang "clang")

# set various configuration settings in the test suite
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/tests/lit.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/tests/Tests/lit.cfg")


# generate output directories
execute_process(
    COMMAND mkdir -p tests/Tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

foreach(file ${test_sources})
    execute_process(
        COMMAND cp ${file} ${CMAKE_BINARY_DIR}/tests/Tests/)
endforeach(file)
