add_subdirectory(BasicPointsToAnalysis)
set (CONSTRAINT_GEN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/TestPass.cc
)

include_directories(${POINTS_TO_INCLUDE_DIRECTORY})
add_points_to_project(TestPass ${CONSTRAINT_GEN_SOURCES})

find_python_module(lit REQUIRED)
find_package(PythonInterp REQUIRED)
add_custom_target(check
    COMMAND lit ${CMAKE_BINARY_DIR}/tests/Tests -v -j 1
    DEPENDS TestPass)


