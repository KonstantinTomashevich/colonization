set (TARGET_NAME TestDistrictsNeighborsCalculation)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestDistrictsNeighborsCalculation.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestDistrictsNeighborsCalculation.cpp)
setup_executable ()
target_link_libraries (TestDistrictsNeighborsCalculation Colonization)
add_test (NAME test_districts_neighbors_calculation COMMAND TestDistrictsNeighborsCalculation)

set (TARGET_NAME TestMapPathFinding)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestMapPathFinding.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestMapPathFinding.cpp)
setup_executable ()
target_link_libraries (TestMapPathFinding Colonization)
add_test (NAME test_map_path_finding COMMAND TestMapPathFinding)

set (TARGET_NAME TestBasicColonyBalance)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestBasicColonyBalance.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestBasicColonyBalance.cpp)
setup_executable ()
target_link_libraries (TestBasicColonyBalance Colonization)
add_test (NAME test_basic_colony_balance COMMAND TestBasicColonyBalance)

set (TARGET_NAME TestInternalTradeProcessing)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestInternalTradeProcessing.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestInternalTradeProcessing.cpp)
setup_executable ()
target_link_libraries (TestInternalTradeProcessing Colonization)
add_test (NAME test_internal_trade_processing COMMAND TestInternalTradeProcessing)

set (TARGET_NAME TestColonyActionBuildWarShip)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestColonyActionBuildWarShip.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestColonyActionBuildWarShip.cpp)
setup_executable ()
target_link_libraries (TestColonyActionBuildWarShip Colonization)
add_test (NAME test_colony_action_build_war_ship COMMAND TestColonyActionBuildWarShip)

set (TARGET_NAME TestFogOfWarCalculator)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestFogOfWarCalculator.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestFogOfWarCalculator.cpp)
setup_executable ()
target_link_libraries (TestFogOfWarCalculator Colonization)
add_test (NAME test_fog_of_war_calculator COMMAND TestFogOfWarCalculator)

set (TARGET_NAME TestMapMaskGeneration)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestMapMaskGeneration.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestMapMaskGeneration.cpp)
setup_executable ()
target_link_libraries (TestMapMaskGeneration Colonization)
add_test (NAME test_map_mask_generation COMMAND TestMapMaskGeneration)

set (TARGET_NAME TestDiplomacyWarAndPeace)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestDiplomacyWarAndPeace.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestDiplomacyWarAndPeace.cpp)
setup_executable ()
target_link_libraries (TestDiplomacyWarAndPeace Colonization)
add_test (NAME test_diplomacy_war_and_peace COMMAND TestDiplomacyWarAndPeace)

set (TARGET_NAME TestBattleCreationAndJoining)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestBattleCreationAndJoining.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestBattleCreationAndJoining.cpp)
setup_executable ()
target_link_libraries (TestBattleCreationAndJoining Colonization)
add_test (NAME test_battle_creation_and_joining COMMAND TestBattleCreationAndJoining)

set (TARGET_NAME TestBattleProcessingAndLooting)
set (SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestBattleProcessingAndLooting.hpp ${CMAKE_CURRENT_SOURCE_DIR}/TestBattleProcessingAndLooting.cpp)
setup_executable ()
target_link_libraries (TestBattleProcessingAndLooting Colonization)
add_test (NAME test_battle_processing_and_looting COMMAND TestBattleProcessingAndLooting)

set_target_properties (
    TestDistrictsNeighborsCalculation
    TestMapPathFinding
    TestBasicColonyBalance
    TestInternalTradeProcessing
    TestColonyActionBuildWarShip
    TestFogOfWarCalculator
    TestMapMaskGeneration
    TestDiplomacyWarAndPeace
    TestBattleCreationAndJoining
    TestBattleProcessingAndLooting

    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Tests"
    )
