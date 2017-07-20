set (TARGET_NAME TestInternalTradeProcessing)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestInternalTradeProcessing Colonization)
add_test (NAME test_map_internal_trade_processing COMMAND TestInternalTradeProcessing)
set_target_properties (
    TestInternalTradeProcessing
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
