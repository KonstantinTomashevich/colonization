#include <Colonization/BuildConfiguration.hpp>
#include "ColoniesActionsProcessor.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <cmath>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void ColoniesActionsProcessor::ProcessColonyActions (District *colony, float timeStep)
{
    Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> action = colony->GetColonyActionByIndex (0);
    if (ProcessAction (colony, action, timeStep))
    {
        colony->RemoveColonyActionByIndex (0);
    }
    else
    {
        colony->SetColonyActionAtIndexData (0, action.second_);
    }
}

void ColoniesActionsProcessor::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (ColoniesActionsProcessor, Update));
}

bool ColoniesActionsProcessor::ProcessAction (District *colony, Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> &action, float timeStep)
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

    action.second_ = actionData;
    return result;
}

bool ColoniesActionsProcessor::ProcessBuildWarShipAction (GameConfiguration *configuration, Map *map, UnitsManager *unitsManager,
                                                          District *colony, Urho3D::VariantMap &actionData, float timeStep)
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Player *player = playersManager->GetPlayerByNameHash (Urho3D::StringHash (colony->GetColonyOwnerName ()));
    assert (player);

    Urho3D::StringHash targetDistrictHash = actionData [ColonyActions::BuildWarShip::TARGET_DISTRICT].GetStringHash ();
    District *targetDistrict = map->GetDistrictByHash (targetDistrictHash);
    if (!targetDistrict || !colony->IsNeighborsWith (targetDistrictHash) || !targetDistrict->IsSea ())
    {
        Urho3D::Log::Write (Urho3D::LOG_ERROR, "Can't build war ships in " + colony->GetHash ().ToString () +
                            " because target district " + targetDistrictHash.ToString () + " is incorrect!");
        return true;
    }

    float buildingSpeed = 1.0f / configuration->GetOneWarShipBasicBuildTime ();
    buildingSpeed *= sqrt (colony->GetIndustryEvolutionPoints ());

    float currentShipProgress = actionData [ColonyActions::BuildWarShip::PROGRESS].GetFloat ();
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
            Unit *newWarShip = unitsManager->CreateUnit ();
            newWarShip->SetUnitType (UNIT_FLEET);
            newWarShip->SetPositionHash (targetDistrictHash);
            newWarShip->SetOwnerPlayerName (colony->GetColonyOwnerName ());
            newWarShip->FleetUnitSetWarShipsCount (1);
            newWarShip->UpdateHash (unitsManager);
            return true;
        }
        else
        {
            actionData [ColonyActions::BuildWarShip::PROGRESS] = currentShipProgress;
            return false;
        }
    }
    else
    {
        return false;
    }
}

ColoniesActionsProcessor::ColoniesActionsProcessor (Urho3D::Context *context) : Urho3D::Component (context)
{

}

ColoniesActionsProcessor::~ColoniesActionsProcessor ()
{

}

void ColoniesActionsProcessor::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <ColoniesActionsProcessor> (COLONIZATION_SERVER_ONLY_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void ColoniesActionsProcessor::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        assert (map);
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
        for (int index = 0; index < map->GetDistrictsCount (); index++)
        {
            District *district = map->GetDistrictByIndex (index);
            if (district->HasColony () && district->GetColonyActionsCount () > 0)
            {
                ProcessColonyActions (district, timeStep);
            }
        }
    }
}
}

