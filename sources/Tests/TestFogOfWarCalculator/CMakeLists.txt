set (TARGET_NAME TestFogOfWarCalculator)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_executable ()
target_link_libraries (TestFogOfWarCalculator Colonization)
add_test (NAME test_fog_of_war_calculator COMMAND TestFogOfWarCalculator)
set_target_properties (
    TestFogOfWarCalculator
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
