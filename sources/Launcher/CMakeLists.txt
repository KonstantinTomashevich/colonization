set (TARGET_NAME ColonizationLauncher)
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_main_executable ()
target_link_libraries (ColonizationLauncher Colonization)
