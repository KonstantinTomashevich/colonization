#include <Colonization/BuildConfiguration.hpp>
#include "ColoniesProcessor.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <cmath>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/District/ColonyActions.hpp>
#include <Colonization/Core/Unit/UnitEvents.hpp>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Network/NetworkUpdateCounterUtils.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
ColoniesProcessor::ColoniesProcessor (Urho3D::Context *context) : Urho3D::Component (context)
{

}

ColoniesProcessor::~ColoniesProcessor ()
{

}

void ColoniesProcessor::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <ColoniesProcessor> (COLONIZATION_SERVER_ONLY_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void ColoniesProcessor::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
        assert (map);
        assert (configuration);

        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
        for (int index = 0; index < map->GetDistrictsCount (); index++)
        {
            District *district = map->GetDistrictByIndex (index);
            if (district->GetHasColony ())
            {
                float updatePoints = 0.0f;
                updatePoints += ProcessColonyEvolution (configuration, district, timeStep);
                if (district->GetColonyActionsCount () > 0)
                {
                    updatePoints += ProcessColonyActions (district, timeStep);
                }
                NetworkUpdateCounterUtils::AddNetworkUpdatePointsToComponentCounter (district, updatePoints);
            }
        }
    }
}

void ColoniesProcessor::HandlePlayerWillBeDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Player *player = static_cast <Player *> (eventData [PlayerWillBeDisconnected::PLAYER].GetPtr ());
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    Urho3D::PODVector <Urho3D::StringHash> colonies = map->GetColoniesOfPlayer (Urho3D::StringHash (player->GetName ()));

    for (int index = 0; index < colonies.Size (); index++)
    {
        District *colony = map->GetDistrictByHash (colonies.At (index));
        DistrictUtils::CleanupColonyFromDistrict (colony);
    }
}

void ColoniesProcessor::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (ColoniesProcessor, Update));
    SubscribeToEvent (EVENT_PLAYER_WILL_BE_DISCONNECTED, URHO3D_HANDLER (ColoniesProcessor, HandlePlayerWillBeDisconnected));
}

float ColoniesProcessor::ProcessColonyEvolution(GameConfiguration *configuration, District *colony, float timeStep)
{
    float updatePoints = 0.0f;
    updatePoints += ProcessColonyPopulation (configuration, colony, timeStep);
    updatePoints += ProcessColonyEvolutionBranch (configuration, colony, DEB_FARMS, timeStep);
    updatePoints += ProcessColonyEvolutionBranch (configuration, colony, DEB_MINES, timeStep);
    updatePoints += ProcessColonyEvolutionBranch (configuration, colony, DEB_INDUSTRY, timeStep);
    updatePoints += ProcessColonyEvolutionBranch (configuration, colony, DEB_LOGISTICS, timeStep);
    updatePoints += ProcessColonyEvolutionBranch (configuration, colony, DEB_DEFENSE, timeStep);
    // TODO: Implement average level of life calculation.
    colony->SetAverageLevelOfLifePoints (1.0f);
    return updatePoints;
}

float ColoniesProcessor::ProcessColonyPopulation (GameConfiguration *configuration, District *colony, float timeStep)
{
    // TODO: If colony population is very big, stop grow. Maybe automatically send colonists from it.
    float sexRatio = colony->GetMenCount () / colony->GetWomenCount ();
    float increaseModifer = 1.0f - Urho3D::Abs (1.0f - sexRatio);
    float populationIncrease = (colony->GetMenCount () * colony->GetWomenCount ()) *
            configuration->GetColoniesBasicPopulationIncrease () * increaseModifer * timeStep;

    float newPopulationSexRatio = Urho3D::Random (0.4f, 0.6f);
    float newMen = populationIncrease * newPopulationSexRatio * timeStep;
    float newWomen = populationIncrease * (1.0f - newPopulationSexRatio) * timeStep;

    float oldMenCount = colony->GetMenCount ();
    float oldWomenCount = colony->GetWomenCount ();
    colony->SetMenCount (oldMenCount + newMen);
    colony->SetWomenCount (oldWomenCount + newWomen);

    if (floor (oldMenCount) == floor (oldMenCount + newMen) &&
            floor (oldWomenCount) == floor (oldWomenCount + newWomen))
    {
        return 0.0f;
    }
    else
    {
        float points = 0.0f;
        points += newMen * 20.0f / (oldMenCount + newMen);
        points += newWomen * 20.0f / (oldWomenCount + newWomen);
        return points;
    }
}

float ColoniesProcessor::ProcessColonyEvolutionBranch (GameConfiguration *configuration, District *colony,
                                                              DistrictEvolutionBranch branch, float timeStep)
{
    float balance = DistrictUtils::GetBalance (colony, branch);
    float evolutionModifer;
    if (balance >= 0.0f)
    {
        evolutionModifer = configuration->GetMaximumEvolutionSpeed () * timeStep;
    }
    else
    {
        evolutionModifer = -configuration->GetMaximumDegradationSpeed () * timeStep;
    }

    float evolutionCost;
    if (balance >= 0.0f)
    {
        evolutionCost = evolutionModifer * configuration->GetEvolutionCostPerLevelPerColonist () *
                DistrictUtils::CalculateColonyPopulation (colony);
    }
    else
    {
        evolutionCost = evolutionModifer * configuration->GetDegradationCostPerLevelPerColonist () *
                DistrictUtils::CalculateColonyPopulation (colony);
    }

    if (evolutionCost > 0.0f && evolutionCost > balance)
    {
        evolutionModifer *= (balance / evolutionCost);
        evolutionCost = balance;
    }

    DistrictUtils::SetEvolutionPoints (colony, branch, DistrictUtils::GetEvolutionPoints (colony, branch) + evolutionModifer);
    DistrictUtils::SetBalance (colony, branch, balance - evolutionCost);
    return Urho3D::Abs (evolutionModifer) * 3000.0f;
}

float ColoniesProcessor::ProcessColonyActions (District *colony, float timeStep)
{
    Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> action = colony->GetColonyActionByIndex (0);
    float updatePoints = 0.0f;
    if (ProcessAction (colony, action, timeStep))
    {
        colony->RemoveColonyActionByIndex (0);
        updatePoints = 100.0f;
    }
    else
    {
        colony->SetColonyActionAtIndexData (0, action.second_);
        updatePoints = timeStep * 25.0f;
    }
    return updatePoints;
}

bool ColoniesProcessor::ProcessAction (District *colony, Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> &action, float timeStep)
{
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    UnitsManager *unitsManager = node_->GetScene ()->GetChild ("units")->GetComponent <UnitsManager> ();
    GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();

    bool result = false;
    Urho3D::VariantMap actionData = action.second_;
    if (action.first_ == ColonyActions::BUILD_WAR_SHIP)
    {
        result = ProcessBuildWarShipAction (configuration, map, unitsManager, colony, actionData, timeStep);
    }
    else if (action.first_ == ColonyActions::FORM_ARMY)
    {
        result = ProcessFormArmyAction (configuration, map, unitsManager, colony, actionData, timeStep);
    }

    action.second_ = actionData;
    return result;
}

bool ColoniesProcessor::ProcessBuildWarShipAction (GameConfiguration *configuration, Map *map, UnitsManager *unitsManager,
                                                          District *colony, Urho3D::VariantMap &actionData, float timeStep)
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Player *player = playersManager->GetPlayerByNameHash (Urho3D::StringHash (colony->GetColonyOwnerName ()));
    assert (player);

    Urho3D::StringHash targetDistrictHash = actionData [ColonyActions::BuildWarShip::TARGET_DISTRICT].GetStringHash ();
    District *targetDistrict = map->GetDistrictByHash (targetDistrictHash);
    if (!targetDistrict || !colony->IsNeighborsWith (targetDistrictHash) || !targetDistrict->GetIsSea ())
    {
        Urho3D::Log::Write (Urho3D::LOG_ERROR, "Can't build war ships in " + colony->GetHash ().ToString () +
                            " because target district " + targetDistrictHash.ToString () + " is incorrect!");
        return true;
    }

    float buildingSpeed = 1.0f / configuration->GetOneWarShipBasicBuildTime ();
    buildingSpeed *= Urho3D::Sqrt (colony->GetIndustryEvolutionPoints ());
    // TODO: Should population influence building speed?

    float currentShipProgress = actionData [COLONY_ACTION_PROGRESS].GetFloat ();
    float timeStepProgress = buildingSpeed * timeStep;
    if (currentShipProgress + timeStepProgress > 1.0f)
    {
        timeStepProgress = 1.0f - currentShipProgress;
    }

    float progressCost = timeStepProgress * configuration->GetOneWarShipBuildingCost ();
    if (player->GetGold () > progressCost)
    {
        currentShipProgress += timeStepProgress;
        player->SetGold (player->GetGold () - progressCost);

        if (currentShipProgress >= 1.0f && colony->GetMenCount () > configuration->GetOneWarShipCrew ())
        {
            colony->SetMenCount (colony->GetMenCount () - configuration->GetOneWarShipCrew ());
            NetworkUpdateCounterUtils::AddNetworkUpdatePointsToComponentCounter (colony, 100.0f);

            FleetUnit *newWarShip = static_cast <FleetUnit *> (
                        unitsManager->CreateUnit (UNIT_FLEET, colony->GetColonyOwnerName (), targetDistrictHash));
            newWarShip->SetWarShipsCount (1);
            return true;
        }
        else
        {
            actionData [COLONY_ACTION_PROGRESS] = currentShipProgress;
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool ColoniesProcessor::ProcessFormArmyAction (GameConfiguration *configuration, Map *map, UnitsManager *unitsManager,
                                                       District *colony, Urho3D::VariantMap &actionData, float timeStep)
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Player *player = playersManager->GetPlayerByNameHash (Urho3D::StringHash (colony->GetColonyOwnerName ()));
    assert (player);

    int soldiersCount = actionData [ColonyActions::FormArmy::SOLDIERS_COUNT].GetInt ();
    float formingSpeed = 1.0f / (configuration->GetOneSoldierMobilizationTime () * soldiersCount);
    float timeStepProgress = formingSpeed * timeStep;

    float currentProgress = actionData [COLONY_ACTION_PROGRESS].GetFloat ();
    if (currentProgress + timeStepProgress > 1.0f)
    {
        timeStepProgress = 1.0f - currentProgress;
    }

    float progressCost = timeStepProgress * soldiersCount * configuration->GetOneSoldierMobilizationCost ();
    if (player->GetGold () > progressCost)
    {
        currentProgress += timeStepProgress;
        player->SetGold (player->GetGold () - progressCost);

        if (currentProgress >= 1.0f && colony->GetMenCount () > soldiersCount)
        {
            colony->SetMenCount (colony->GetMenCount () - soldiersCount);
            NetworkUpdateCounterUtils::AddNetworkUpdatePointsToComponentCounter (colony, 100.0f);

            ArmyUnit *army = static_cast <ArmyUnit *> (
                    unitsManager->CreateUnit (UNIT_ARMY, colony->GetColonyOwnerName (), colony->GetHash ()));
            army->SetSoldiersCount (soldiersCount * 1.0f);
            return true;
        }
        else
        {
            actionData [COLONY_ACTION_PROGRESS] = currentProgress;
            return false;
        }
    }
    else
    {
        return false;
    }
}
}

