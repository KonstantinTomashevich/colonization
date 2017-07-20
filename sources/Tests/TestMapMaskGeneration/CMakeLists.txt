set (TARGET_NAME TestMapMaskGeneration)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestMapMaskGeneration Colonization)
add_test (NAME test_map_mask_generation COMMAND TestMapMaskGeneration)
set_target_properties (
    TestMapMaskGeneration
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
