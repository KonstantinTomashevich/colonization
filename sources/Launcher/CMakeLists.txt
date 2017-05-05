set (TARGET_NAME ColonizationLauncher)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_main_executable ()
target_link_libraries (ColonizationLauncher Colonization)
set_target_properties (
    ColonizationLauncher
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/GameLauncher"
    )
