set (TARGET_NAME TestBasicColonyBalance)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestBasicColonyBalance Colonization)
add_test (NAME test_basic_colony_balance COMMAND TestBasicColonyBalance)
set_target_properties (
    TestBasicColonyBalance
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
