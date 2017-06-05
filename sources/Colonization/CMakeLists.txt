message (STATUS "Unwrapping scaffolds...")
set (UNWRAPPER_SCRIPT "./code-generation/GenerateDataObjectFromScaffold/Main.lua")
set (AS_BIND_GEN_COMMAND "ASBindGenCommand=ASBindGen")
file (GLOB_RECURSE SCAFFOLDS "*.scaffold")
file (MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/UnwrapperLogs/)

foreach (SCAFFOLD ${SCAFFOLDS})
    file (RELATIVE_PATH SCAFFOLD_REL ${CMAKE_SOURCE_DIR} ${SCAFFOLD})
    get_filename_component (DIR ${SCAFFOLD_REL} DIRECTORY)
    get_filename_component (NAME ${SCAFFOLD} NAME_WE)
    message (STATUS "Unwrapping ${NAME}...")

    set (OUTPUT_HPP "${DIR}/${NAME}.hpp")
    set (OUTPUT_CPP "${DIR}/${NAME}.cpp")
    execute_process (COMMAND ${LUA} ${UNWRAPPER_SCRIPT} ${SCAFFOLD_REL} ${OUTPUT_HPP} ${OUTPUT_CPP} ${AS_BIND_GEN_COMMAND}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE RESULT
        OUTPUT_FILE "${CMAKE_BINARY_DIR}/UnwrapperLogs/${NAME}.log")

    if (NOT RESULT EQUAL 0)
        message (FATAL_ERROR "Can't unwrap ${NAME}! See BinaryDir/UnwrapperLogs/${NAME}.log")
    endif ()
endforeach ()
message (STATUS "Unwrapping scaffolds done.")

configure_file (${CMAKE_CURRENT_SOURCE_DIR}/BuildConfiguration.hpp.cmake ${CMAKE_CURRENT_SOURCE_DIR}/BuildConfiguration.hpp)
set (TARGET_NAME Colonization)
define_source_files (RECURSE GLOB_H_PATTERNS "*.hpp" EXCLUDE_PATTERNS "AngelScriptBindings/[^;]+;")

set (COLONIZATION_HEADERS_LIST )
set (IS_FIRST_ITEM 1)
foreach (HEADER ${H_FILES})
    if (NOT IS_FIRST_ITEM)
        set (START ",\n    ")
    else ()
        set (IS_FIRST_ITEM 0)
        set (START "    ")
    endif ()
    set (COLONIZATION_HEADERS_LIST "${COLONIZATION_HEADERS_LIST}${START}\"${HEADER}\"")
endforeach ()
configure_file (${CMAKE_SOURCE_DIR}/ASBindGenConfiguration.lua.cmake ${CMAKE_SOURCE_DIR}/ASBindGenConfiguration.lua)

message (STATUS "Generation Angel Script bindings via ASBindGen...")
set (ASBINDGEN_SCRIPT "./code-generation/AngelScriptBindingsGenerator/Main.lua")
execute_process (COMMAND ${LUA} ${ASBINDGEN_SCRIPT} "ASBindGenConfiguration.lua"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE RESULT
    OUTPUT_FILE "${CMAKE_BINARY_DIR}/ASBindGen.log")
if (NOT RESULT EQUAL 0)
    message (FATAL_ERROR "Can't generate Angel Script bindings! See BinaryDir/ASBindGen.log")
endif ()
message (STATUS "Angel Script bindings generated.")

define_dependency_libs (Urho3D)
setup_library (STATIC)
