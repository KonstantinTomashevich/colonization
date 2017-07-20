set (TARGET_NAME TestBattleProcessingAndLooting)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestBattleProcessingAndLooting Colonization)
add_test (NAME test_battle_processing_and_looting COMMAND TestBattleProcessingAndLooting)
set_target_properties (
    TestBattleProcessingAndLooting
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
