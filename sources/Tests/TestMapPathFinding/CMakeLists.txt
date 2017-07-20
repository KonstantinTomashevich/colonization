set (TARGET_NAME TestMapPathFinding)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestMapPathFinding Colonization)
add_test (NAME test_map_path_finding COMMAND TestMapPathFinding)
set_target_properties (
    TestMapPathFinding
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
