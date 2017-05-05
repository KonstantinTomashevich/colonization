set (ANGELSCRIPT_API_HEADER_OUTPUT "\"${CMAKE_SOURCE_DIR}/bin/AngelScriptAPI.hpp\"")
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Defines.hpp.cmake ${CMAKE_CURRENT_SOURCE_DIR}/Defines.hpp)
set (TARGET_NAME GenerateAngelScriptBindings)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (GenerateAngelScriptBindings Colonization)
add_test (NAME generate_angel_script_bindings COMMAND GenerateAngelScriptBindings)
set_target_properties (
    GenerateAngelScriptBindings
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Utils"
    )
