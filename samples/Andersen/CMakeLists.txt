set(ANDERSONS_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/AndersensConstraintSolver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/AndersensConstraintSolver.h
    ${CMAKE_CURRENT_SOURCE_DIR}/AndersensGraphDump.cc)

include_directories(${POINTS_TO_INCLUDE_DIRECTORY})
include_directories(../../utils)
add_points_to_project(anderson ${ANDERSONS_SOURCE_FILES})

