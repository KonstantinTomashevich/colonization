set (TARGET_NAME TestBattleCreationAndJoining)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestBattleCreationAndJoining Colonization)
add_test (NAME test_battle_creation_and_joining COMMAND TestBattleCreationAndJoining)
set_target_properties (
    TestBattleCreationAndJoining
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
