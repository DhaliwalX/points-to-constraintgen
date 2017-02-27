set (BASIC_POINTS_TO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/BasicPointsToAnalysis.cc
)

include_directories(${POINTS_TO_INCLUDE_DIRECTORY})
add_points_to_project(BasicPointsToAnalysis ${BASIC_POINTS_TO_SOURCES})
