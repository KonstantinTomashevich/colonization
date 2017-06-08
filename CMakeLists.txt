project (Colonization)
cmake_minimum_required (VERSION 2.8.6)
if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy (SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy (SET CMP0042 NEW)
    endif ()
endif ()

# Copy Urho3D CMake modules.
if (NOT CMAKE_MODULE_PATH)
    message (STATUS "CMAKE_MODULE_PATH isn't setted! Setting CMAKE_MODULE_PATH to urho3d-cmake/Modules.")
    set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/urho3d-cmake/Modules")
    if (NOT EXISTS "${CMAKE_SOURCE_DIR}/urho3d-cmake/")
        message (STATUS "Copying Urho3D's CMake modules...")
        if (NOT EXISTS "$ENV{URHO3D_HOME}/share/CMake/")
            file (COPY "$ENV{URHO3D_HOME}/share/Urho3D/CMake/" DESTINATION "${CMAKE_SOURCE_DIR}/urho3d-cmake/")
        else ()
            file (COPY "$ENV{URHO3D_HOME}/share/CMake/" DESTINATION "${CMAKE_SOURCE_DIR}/urho3d-cmake")
        endif ()
    endif ()
    message (STATUS "Setted CMAKE_MODULE_PATH to urho3d-cmake/Modules!")
endif ()

# Copy Urho3D CoreData.
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/bin/CoreData")
    message (STATUS "Copying Urho3D's CoreData asset...")
    if (NOT EXISTS "$ENV{URHO3D_HOME}/share/Resources/CoreData/")
        file (COPY "$ENV{URHO3D_HOME}/share/Urho3D/Resources/CoreData/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/CoreData")
    else ()
        file (COPY "$ENV{URHO3D_HOME}/share/Resources/CoreData/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/CoreData")
    endif ()
endif ()


# Copy Urho3D Data (for Urho3D Editor).
if (NOT EXISTS "${CMAKE_SOURCE_DIR}/bin/Urho3DEditorData")
    message (STATUS "Copying Urho3D's Data asset (for editor)...")
    if (NOT EXISTS "$ENV{URHO3D_HOME}/share/Resources/Data/")
        file (COPY "$ENV{URHO3D_HOME}/share/Urho3D/Resources/Data/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/Urho3DEditorData")
    else ()
        file (COPY "$ENV{URHO3D_HOME}/share/Resources/Data/" DESTINATION "${CMAKE_SOURCE_DIR}/bin/Urho3DEditorData")
    endif ()
endif ()

if (NOT EXISTS "${CMAKE_BINARY_DIR}/bin/Urho3DEditorData")
    message (STATUS "Copying Urho3D's Data asset (for editor) to binary dir...")
    file (COPY "${CMAKE_SOURCE_DIR}/bin/Urho3DEditorData/" DESTINATION "${CMAKE_BINARY_DIR}/bin/Urho3DEditorData")
endif ()

# Find lua executable.
find_program (IS_LUA_PROGRAM_EXISTS lua)
if (NOT IS_LUA_PROGRAM_EXISTS)
    if (EXISTS "$ENV{URHO3D_HOME}/bin/lua")
        set (LUA "$ENV{URHO3D_HOME}/bin/lua")
    else ()
        set (LUA "$ENV{URHO3D_HOME}/bin/luajit")
    endif ()

    find_program (IS_NEW_LUA_PROGRAM_EXISTS ${LUA})
    if (NOT IS_NEW_LUA_PROGRAM_EXISTS)
        message (FATAL_ERROR "${LUA} isn't exists! Can't build without Lua.")
    endif ()
else ()
    set (LUA lua)
endif ()
message (STATUS "Lua command: ${LUA}")

# Setup dependencies.
set (DEPENDENCIES_DIR "dependencies")
file (MAKE_DIRECTORY ${DEPENDENCIES_DIR})

# Setup ScaffoldUnwrapper.
set (SCAFFOLD_UNWRAPPER_REPO_OWNER "KonstantinTomashevich")
set (SCAFFOLD_UNWRAPPER_REPO_NAME "scaffold-unwrapper")
if (EXISTS "${CMAKE_SOURCE_DIR}/${DEPENDENCIES_DIR}/${SCAFFOLD_UNWRAPPER_REPO_NAME}")
    message (STATUS "Updating ScaffoldUnwrapper dependency...")
    execute_process (COMMAND "git" "pull" "origin" "master"
                     OUTPUT_VARIABLE OUTPUT
                     RESULT_VARIABLE RESULT
                     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${DEPENDENCIES_DIR}/${SCAFFOLD_UNWRAPPER_REPO_NAME}")
    if (NOT RESULT EQUAL 0)
        message (FATAL_ERROR "Can not update ScaffoldUnwrapper!")
    endif ()
else ()
    message (STATUS "Cloning ScaffoldUnwrapper dependency...")
    execute_process (COMMAND "git" "clone" "https://github.com/${SCAFFOLD_UNWRAPPER_REPO_OWNER}/${SCAFFOLD_UNWRAPPER_REPO_NAME}.git"
                     OUTPUT_VARIABLE OUTPUT
                     RESULT_VARIABLE RESULT
                     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${DEPENDENCIES_DIR}")
    if (NOT RESULT EQUAL 0)
        message (FATAL_ERROR "Can not clone ScaffoldUnwrapper!")
    endif ()
endif ()
set (SCAFFOLD_UNWRAPPER_SCRIPT "./${DEPENDENCIES_DIR}/${SCAFFOLD_UNWRAPPER_REPO_NAME}/Main.lua")

# Setup ASBindGen.
set (AS_BIND_GEN_REPO_OWNER "KonstantinTomashevich")
set (AS_BIND_GEN_REPO_NAME "as-bind-gen")
if (EXISTS "${CMAKE_SOURCE_DIR}/${DEPENDENCIES_DIR}/${AS_BIND_GEN_REPO_NAME}")
    message (STATUS "Updating ASBindGen dependency...")
    execute_process (COMMAND "git" "pull" "origin" "master"
                     OUTPUT_VARIABLE OUTPUT
                     RESULT_VARIABLE RESULT
                     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${DEPENDENCIES_DIR}/${AS_BIND_GEN_REPO_NAME}")
    if (NOT RESULT EQUAL 0)
        message (FATAL_ERROR "Can not update ASBindGen!")
    endif ()
else ()
    message (STATUS "Cloning ASBindGen dependency...")
    execute_process (COMMAND "git" "clone" "https://github.com/${AS_BIND_GEN_REPO_OWNER}/${AS_BIND_GEN_REPO_NAME}.git"
                     OUTPUT_VARIABLE OUTPUT
                     RESULT_VARIABLE RESULT
                     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${DEPENDENCIES_DIR}")
    if (NOT RESULT EQUAL 0)
        message (FATAL_ERROR "Can not clone ASBindGen!")
    endif ()
endif ()
set (AS_BIND_GEN_SCRIPT "./${DEPENDENCIES_DIR}/${AS_BIND_GEN_REPO_NAME}/Main.lua")

include (UrhoCommon)
find_package (Urho3D REQUIRED)
include_directories (${URHO3D_INCLUDE_DIRS})
enable_testing ()
add_subdirectory (sources)
