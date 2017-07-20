set (TARGET_NAME TestColonyActionFormArmy)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestColonyActionFormArmy Colonization)
add_test (NAME test_colony_action_form_army COMMAND TestColonyActionFormArmy)
set_target_properties (
    TestColonyActionFormArmy
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
