set (CHECK_SCRIPTS_RESOURCE_DIR "\"${CMAKE_BINARY_DIR}/bin/Data/\"")
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/Defines.hpp.cmake ${CMAKE_CURRENT_SOURCE_DIR}/Defines.hpp)
set (TARGET_NAME CheckScripts)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (CheckScripts Colonization)
add_test (NAME check_scripts COMMAND CheckScripts)
set_target_properties (
    CheckScripts
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tools"
    )

