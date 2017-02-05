cmake_minimum_required(VERSION 3.1)


find_package(LLVM CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(LLVMConfig)

# include these after loading llvm-config
include(cmake/cmake_functions.cmake)
include(cmake/PointsToFunctions.cmake)

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

file (GLOB test_sources ${CMAKE_CURRENT_SOURCE_DIR}/tests/ConstraintGenPass/*.c)

set(args -S -c -emit-llvm -w)
set(clang "clang")

# generate output directories
execute_process(
    COMMAND mkdir -p O0
    COMMAND mkdir -p O1
    COMMAND mkdir -p O2
    COMMAND mkdir -p O3
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
set(generated_test_files "")

message(STATUS "Generating test files")
foreach(file ${test_sources})
    get_filename_component(file_name_only ${file} NAME_WE)
    generate_optimized_file(${file} O0)
endforeach(file)

foreach(file ${generated_test_files})
    run_opt(${file})
    set_debug(${file})
endforeach(${file})

