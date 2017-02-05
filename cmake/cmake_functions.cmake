
set (opt_flags -load ./lib/libconstraintgen.so -constraints)
function(run_opt testfile)
    set (opt_current_flags ${opt_flags} ${testfile})
    get_filename_component(test_target ${testfile} NAME_WE)
    string(REPLACE -generated "" test_target ${test_target})
    add_custom_command(
        OUTPUT output-${test_target}
        COMMAND opt ${opt_current_flags} > /dev/null
        DEPENDS constraintgen ${testfile}
        COMMENT "Running opt on ${testfile}"
    )

    add_custom_target(${test_target} DEPENDS output-${test_target})
endfunction(run_opt)

function (generate_optimized_file file optimization)
    get_filename_component(filename ${file} NAME_WE)
    set(filename ${filename}-${optimization})
    set(output_file ${CMAKE_BINARY_DIR}/${optimization}/${filename}-generated.ll)
    set(current_args ${args} -${optimization} ${file} -o ${output_file})
    execute_process(
        COMMAND clang ${current_args}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE result
    )

    if (${result})
        message(ERROR "Unable to generate ${filename}. (Error code: ${result})")
    else(${result})
        set(generated_test_files ${generated_test_files} ${output_file} PARENT_SCOPE)
    endif(${result})
endfunction(generate_optimized_file)

set (gdb_flags --args)
function(set_debug testfile)
    set (gdb_current_flags ${gdb_flags} opt ${opt_flags} ${testfile})
    get_filename_component(test_target debug-${testfile} NAME_WE)
    string(REPLACE -generated "" test_target ${test_target})
    add_custom_command(
        OUTPUT mdebug-${test_target}
        COMMAND gdb ${gdb_current_flags}
        DEPENDS constraintgen ${testfile}
        COMMENT "Running gdb on ${testfile}"
    )

    add_custom_target(debug-${test_target} DEPENDS mdebug-${test_target})
endfunction(set_debug)

