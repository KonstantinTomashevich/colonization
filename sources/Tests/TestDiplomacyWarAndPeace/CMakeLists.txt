set (TARGET_NAME TestDiplomacyWarAndPeace)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestDiplomacyWarAndPeace Colonization)
add_test (NAME test_diplomacy_war_and_peace COMMAND TestDiplomacyWarAndPeace)
set_target_properties (
    TestDiplomacyWarAndPeace
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
