set (TARGET_NAME EditorLauncher)
set (RESOURCE_DIRS "${CMAKE_SOURCE_DIR}/bin/CoreData;${CMAKE_SOURCE_DIR}/bin/CoreData;${CMAKE_SOURCE_DIR}/bin/Urho3DEditorData")
define_source_files (RECURSE GLOB_H_PATTERNS *.hpp)
setup_main_executable ()
target_link_libraries (EditorLauncher Colonization)
set_target_properties (
    EditorLauncher
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tools"
    )
