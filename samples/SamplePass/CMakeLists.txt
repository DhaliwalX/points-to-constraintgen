set (CONSTRAINT_GEN_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ConstraintGenPass.cc
)

include_directories(${POINTS_TO_INCLUDE_DIRECTORY})
add_points_to_project(constraintgen ${CONSTRAINT_GEN_SOURCES})
