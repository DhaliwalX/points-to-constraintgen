cmake_minimum_required(VERSION 3.1)

find_package(LLVM CONFIG REQUIRED)
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(LLVMConfig)
include(cmake/cmake_functions.cmake)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_ROOT}/lib/cmake/llvm")
message(STATUS "Looking for LLVM cmake modules: ${LLVM_ROOT}/lib/cmake/llvm")

add_library(constraintgen MODULE
    src/Variable.cc src/ConstraintGenPass.cc src/Constraint.cc
)

add_library(assigntemp MODULE
	utils/AssignTemporaryName.cc)

add_library(gep MODULE utils/gepinstruction.cpp)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(./include)
add_definitions(${LLVM_DEFINITIONS})


target_compile_features(constraintgen PRIVATE cxx_range_for cxx_auto_type)
target_compile_features(assigntemp PRIVATE cxx_range_for cxx_auto_type)
target_compile_features(gep PRIVATE cxx_range_for cxx_auto_type)

set_target_properties(constraintgen PROPERTIES
    COMPILE_FLAGS "-fno-rtti -g"
)
set_target_properties(assigntemp PROPERTIES COMPILE_FLAGS "-fno-rtti -g")
set_target_properties(gep PROPERTIES COMPILE_FLAGS "-fno-rtti -g")

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
    generate_optimized_file(${file} O1)
    generate_optimized_file(${file} O2)
    generate_optimized_file(${file} O3)
endforeach(file)

foreach(file ${generated_test_files})
    run_opt(${file})
    set_debug(${file})
endforeach(${file})
