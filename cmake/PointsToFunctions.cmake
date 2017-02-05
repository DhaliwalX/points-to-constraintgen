set(output_dir "${CMAKE_BINARY_DIR}/lib")

function(add_points_to_project project_name sources)

    message("${sources}")
    set(all_sources ${POINTS_TO_SOURCE_FILES} ${sources})
    add_library(${project_name} MODULE ${all_sources} ${POINTS_TO_SOURCE_FILES})

    target_compile_features(${project_name} PRIVATE cxx_range_for
        cxx_auto_type)
    
    # change the default target location
    set_target_properties(${project_name} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${output_dir}")
    set_target_properties(${project_name} PROPERTIES COMPILE_FLAGS "-fno-rtti -g")
endfunction(add_points_to_project)

function(add_points_to_library)
    add_library(pointsto ${POINTS_TO_SOURCE_FILES})
    set_target_properties(pointsto PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${output_dir}")


    target_compile_features(pointsto PRIVATE cxx_range_for
        cxx_auto_type)
endfunction(add_points_to_library)
