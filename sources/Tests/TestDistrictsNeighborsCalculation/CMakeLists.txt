set (TARGET_NAME TestDistrictsNeighborsCalculation)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestDistrictsNeighborsCalculation Colonization)
add_test (NAME test_districts_neighbors_calculation COMMAND TestDistrictsNeighborsCalculation)
set_target_properties (
    TestDistrictsNeighborsCalculation
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
