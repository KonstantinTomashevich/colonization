set (TARGET_NAME TestColonyActionBuildWarShip)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestColonyActionBuildWarShip Colonization)
add_test (NAME test_colony_action_build_war_ship COMMAND TestColonyActionBuildWarShip)
set_target_properties (
    TestColonyActionBuildWarShip
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
