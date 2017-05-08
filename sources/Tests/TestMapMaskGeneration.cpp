#include <Colonization/BuildConfiguration.hpp>
#include "TestMapMaskGeneration.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>

#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/Unit/Unit.hpp>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Frontend/FogOfWarCalculator.hpp>
#include <Colonization/Frontend/MapMaskUpdater.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestMapMaskGenerationApplication)
namespace Tests
{
TestMapMaskGenerationApplication::TestMapMaskGenerationApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestMapMaskGenerationApplication::~TestMapMaskGenerationApplication ()
{

}

void TestMapMaskGenerationApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "TestMapMaskGeneration.log";
    engineParameters_ ["WindowTitle"] = "TestMapMaskGeneration";
    engineParameters_ ["ResourcePrefixPaths"] = "..;.";
}

void TestMapMaskGenerationApplication::Start ()
{
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Set logging level
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetLevel (Urho3D::LOG_DEBUG);

    Colonization::RegisterAllObjects (context_);
    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    scene->CreateComponent <Colonization::GameConfiguration> ();
    Colonization::Map *map = scene->CreateChild ("map")->CreateComponent <Colonization::Map> ();
    const int mapWidth = 5;
    const int mapHeight = 5;

    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            Colonization::District *district = map->CreateDistrict (
                        "(" + Urho3D::String (x) + "; " + Urho3D::String (y) + ")");

            Urho3D::PODVector <Urho3D::Vector3> polygonPoints;
            polygonPoints.Push (Urho3D::Vector3 (x, 0.0f, y));
            polygonPoints.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y));
            polygonPoints.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y + 1.0f));
            polygonPoints.Push (Urho3D::Vector3 (x, 0.0f, y + 1.0f));
            district->SetPolygonPoints (polygonPoints);

            district->SetClimate (Colonization::CLIMATE_TEMPERATE);
            district->SetFarmingSquare (250.0f);
            district->SetLandAverageFertility (1.0f);
            district->SetForestsSquare (150.0f);

            district->SetUnitPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f));
            district->SetColonyPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f));
            district->SetColonyOwnerName ("PlayerX");
        }
    }
    // Map: (~ -- sea, = -- terrain, @ -- colony)
    //   0 1 2 3 4
    // 4 ~ ~ ~ ~ ~
    // 3 ~ = = = ~
    // 2 ~ ~ ~ = ~
    // 1 ~ = @ = ~
    // 0 ~ = ~ ~ ~
    // Start point: (4, 0).
    // End point: (2, 2).

    // array (X * HEIGHT + Y) = (X, Y)
    map->GetDistrictByIndex (1 * mapHeight + 3)->SetIsSea (false);
    map->GetDistrictByIndex (2 * mapHeight + 3)->SetIsSea (false);
    map->GetDistrictByIndex (3 * mapHeight + 3)->SetIsSea (false);
    map->GetDistrictByIndex (3 * mapHeight + 2)->SetIsSea (false);
    map->GetDistrictByIndex (3 * mapHeight + 1)->SetIsSea (false);
    map->GetDistrictByIndex (2 * mapHeight + 1)->SetIsSea (false);
    map->GetDistrictByIndex (1 * mapHeight + 1)->SetIsSea (false);
    map->GetDistrictByIndex (1 * mapHeight + 0)->SetIsSea (false);

    map->GetDistrictByIndex (2 * mapHeight + 1)->SetColony (true);
    map->GetDistrictByIndex (2 * mapHeight + 1)->SetColonyOwnerName ("PlayerX");
    map->RecalculateDistrictsNeighbors ();

    Colonization::UnitsManager *unitsManager =
            scene->CreateChild ("units", Urho3D::REPLICATED)->CreateComponent <Colonization::UnitsManager> ();
    Colonization::Unit *unit1 = unitsManager->CreateUnit (Colonization::UNIT_FLEET);
    Colonization::Unit *unit2 = unitsManager->CreateUnit (Colonization::UNIT_FLEET);

    unit1->SetOwnerPlayerName ("PlayerX");
    unit2->SetOwnerPlayerName ("PlayerX");

    // array (X * HEIGHT + Y) = (X, Y)
    unit1->SetPositionHash (map->GetDistrictByIndex (0 * mapHeight + 4)->GetHash ());
    unit2->SetPositionHash (map->GetDistrictByIndex (4 * mapHeight + 1)->GetHash ());

    Colonization::FogOfWarCalculator *fogOfWarCalculator = scene->CreateComponent <Colonization::FogOfWarCalculator> ();
    fogOfWarCalculator->SetPlayerName ("PlayerX");
    Urho3D::VariantMap eventData;
    eventData [Urho3D::SceneUpdate::P_TIMESTEP] = 1.0f / 60.0f;
    fogOfWarCalculator->Update (Urho3D::E_SCENEUPDATE, eventData);

    Colonization::MapMaskUpdater *mapMaskUpdater = scene->CreateComponent <Colonization::MapMaskUpdater> ();
    mapMaskUpdater->SetMapMinPoint (Urho3D::Vector3 (0.0f, 0.0f, 0.0f));
    mapMaskUpdater->SetMapMaxPoint (Urho3D::Vector3 (mapWidth, 0.0f, mapHeight));
    mapMaskUpdater->RecalculateMaskImage ();
    mapMaskUpdater->GetMaskImage ()->SavePNG ("MapMaskUpdaterResult_MaskImage.png");

    for (int index = 0; index < map->GetDistrictsCount (); index++)
    {
        Colonization::District *district = map->GetDistrictByIndex (index);
        if (mapMaskUpdater->GetDistrictByPoint (district->GetColonyPosition ()) != district->GetHash ())
        {
            unsigned districtColor = mapMaskUpdater->GetDistrictColorInt (district->GetHash ());
            Urho3D::IntVector2 colonyPositionOnMap = mapMaskUpdater->WorldPointToMapPoint (district->GetColonyPosition ());
            unsigned foundColor = mapMaskUpdater->GetMaskImage ()->GetPixelInt (
                        colonyPositionOnMap.x_, colonyPositionOnMap.y_);

            ErrorExit ("MapMaskUpdater::GetDistrictByPoint error with district " + district->GetName () + "!\n" +
                       "District color: " + Urho3D::String (districtColor) +
                       ".\nFound color: " + Urho3D::String (foundColor) + ".");
            assert (false);
        }
    }

    mapMaskUpdater->Update (Urho3D::E_SCENEUPDATE, eventData);
    mapMaskUpdater->GetFogOfWarMaskImage ()->SavePNG ("MapMaskUpdaterResult_FogOfWarMaskImage.png");
    for (int districtIndex = 0; districtIndex < map->GetDistrictsCount (); districtIndex++)
    {
        Colonization::District *district = map->GetDistrictByIndex (districtIndex);
        Urho3D::IntVector2 position = mapMaskUpdater->WorldPointToMapPoint (district->GetUnitPosition ());
        Urho3D::Color color = mapMaskUpdater->GetFogOfWarMaskImage ()->GetPixel (position.x_, position.y_);
        bool willBeVisible = fogOfWarCalculator->IsDistrictVisible (district->GetHash ());

        if ((willBeVisible && color.ToUInt () != mapMaskUpdater->GetVisibleDistrictColor ().ToUInt ()) ||
                (!willBeVisible && color.ToUInt () != mapMaskUpdater->GetDistrictUnderFogColor ().ToUInt ()))
        {
            ErrorExit ("Error in FogOfWar map with distirct " + district->GetName () + "! It will " +
                       (willBeVisible ? Urho3D::String (" ") : Urho3D::String ("not ")) +
                       Urho3D::String ("be visible!"));
            assert (false);
        }
    }
    engine_->Exit ();
}

void TestMapMaskGenerationApplication::Stop ()
{

}
}
