#include "TestColonyActionFormArmy.hpp"
#include "EmptyInternalPlayer.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>

#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/District/ColonyActions.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea/InternalTradeArea.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/ColoniesActionsProcessor.hpp>
#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestColonyActionFormArmyApplication)
namespace Tests
{
TestColonyActionFormArmyApplication::TestColonyActionFormArmyApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestColonyActionFormArmyApplication::~TestColonyActionFormArmyApplication ()
{

}

void TestColonyActionFormArmyApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "TestColonyActionFormArmy.log";
    engineParameters_ ["WindowTitle"] = "TestColonyActionFormArmy";
    engineParameters_ ["ResourcePrefixPaths"] = "..;.";
}

void TestColonyActionFormArmyApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Set logging level
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetLevel (Urho3D::LOG_DEBUG);

    Colonization::RegisterAllObjects (context_);
    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    Colonization::GameConfiguration *configuration = scene->CreateComponent <Colonization::GameConfiguration> ();
    Colonization::Map *map = scene->CreateChild ("map")->CreateComponent <Colonization::Map> ();
    const int mapWidth = 2;
    const int mapHeight = 2;
    const int soldiersWillBeModilized = 50;

    for (int x = 0; x < mapWidth; x++)
    {
        for (int y = 0; y < mapHeight; y++)
        {
            Colonization::District *district = map->CreateDistrict (
                        "(" + Urho3D::String (x) + "; " + Urho3D::String (y) + ")");

            Urho3D::PODVector <Urho3D::Vector3> polygonPoints;
            polygonPoints.Push (Urho3D::Vector3 (x, 0.0f, y ));
            polygonPoints.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y));
            polygonPoints.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y + 1.0f));
            polygonPoints.Push (Urho3D::Vector3 (x, 0.0f, y + 1.0f));
            district->SetPolygonPoints (polygonPoints);

            district->SetUnitPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f));
            if (x == 1 && y == 1)
            {
                district->SetColonyPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f));
                district->SetIsSea (false);

                district->SetUnitPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f));
                district->SetColonyPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f));
                district->SetHasColony (true);
                district->SetColonyOwnerName ("PlayerX");
                district->SetMenCount (100.0f + soldiersWillBeModilized);
                district->SetWomenCount (100.0f);

                district->SetFarmsEvolutionPoints (1.0f);
                district->SetMinesEvolutionPoints (1.0f);
                district->SetIndustryEvolutionPoints (1.0f);
                district->SetLogisticsEvolutionPoints (1.0f);
                district->SetDefenseEvolutionPoints (1.0f);
            }
            else
            {
                district->SetIsSea (true);
            }
        }
    }
    map->RecalculateDistrictsNeighbors ();
    // array (X * HEIGHT + Y) = (X, Y)

    Urho3D::PODVector <Urho3D::StringHash> wayToEuropeDistricts;
    wayToEuropeDistricts.Push (map->GetDistrictByIndex (0)->GetHash ());
    configuration->SetWayToEuropeDistricts (wayToEuropeDistricts);

    scene->CreateComponent <Colonization::MessagesHandler> ();
    Colonization::PlayersManager *playersManager  = scene->CreateChild ("players")->CreateComponent <Colonization::PlayersManager> ();
    scene->CreateComponent <Colonization::NetworkUpdateSmoother> ();

    Colonization::UnitsManager *unitsManager =
            scene->CreateChild ("units", Urho3D::REPLICATED)->CreateComponent <Colonization::UnitsManager> ();
    Colonization::ColoniesActionsProcessor *actionsProcessor =
            scene->CreateComponent <Colonization::ColoniesActionsProcessor> ();

    EmptyInternalPlayer *player = new EmptyInternalPlayer (context_, "PlayerX", Urho3D::Color::RED, scene);
    if (!playersManager->AddInternalPlayer (player))
    {
        ErrorExit ("Can't create internal player!");
    }
    player->SetGold (configuration->GetOneSoldierMobilizationCost () * (soldiersWillBeModilized + 1));

    Urho3D::VariantMap updateEventData;
    updateEventData [Urho3D::SceneUpdate::P_TIMESTEP] =
            configuration->GetOneSoldierMobilizationTime () * (soldiersWillBeModilized + 1);

    Colonization::District *colony = map->GetDistrictByIndex (1 * mapHeight + 1);
    Urho3D::VariantMap formArmyAction;
    formArmyAction [Colonization::ColonyActions::FormArmy::SOLDIERS_COUNT] = soldiersWillBeModilized;

    colony->AddColonyAction (Colonization::ColonyActions::FORM_ARMY, formArmyAction);
    actionsProcessor->Update (Urho3D::E_SCENEUPDATE, updateEventData);
    if (unitsManager->GetUnitsCount () != 1)
    {
        ErrorExit ("Expected 1 created army unit! But got " +
                   Urho3D::String (unitsManager->GetUnitsCount ()) + " units and " +
                   Urho3D::String (colony->GetColonyActionsCount ()) + " colony actions.");
        return;
    }
    else
    {
        Colonization::ArmyUnit *army = static_cast <Colonization::ArmyUnit *> (unitsManager->GetUnitByIndex (0));
        if (army->GetSoldiersCount () != soldiersWillBeModilized * 1.0f)
        {
            ErrorExit ("Expected " + Urho3D::String (soldiersWillBeModilized) + " soldiers, but got " +
                       Urho3D::String (army->GetSoldiersCount ()) + "!");
            return;
        }
        else
        {
            engine_->Exit ();
        }
    }
}

void TestColonyActionFormArmyApplication::Stop ()
{

}
}
