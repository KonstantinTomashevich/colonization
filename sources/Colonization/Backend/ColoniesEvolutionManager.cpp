#include <Colonization/BuildConfiguration.hpp>
#include "ColoniesEvolutionManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/District/DistrictUtils.hpp>

#include <Colonization/Utils/Network/NetworkUpdateCounterUtils.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
ColoniesEvolutionManager::ColoniesEvolutionManager (Urho3D::Context *context) : Urho3D::Component (context)
{

}

ColoniesEvolutionManager::~ColoniesEvolutionManager ()
{

}

void ColoniesEvolutionManager::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <ColoniesEvolutionManager> (COLONIZATION_SERVER_ONLY_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void ColoniesEvolutionManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
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
                ProcessColony (configuration, district, timeStep);
            }
        }
    }
}

void ColoniesEvolutionManager::HandlePlayerWillBeDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
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

void ColoniesEvolutionManager::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (ColoniesEvolutionManager, Update));
    SubscribeToEvent (EVENT_PLAYER_WILL_BE_DISCONNECTED, URHO3D_HANDLER (ColoniesEvolutionManager, HandlePlayerWillBeDisconnected));
}

void ColoniesEvolutionManager::ProcessColony (GameConfiguration *configuration, District *colony, float timeStep)
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
    NetworkUpdateCounterUtils::AddNetworkUpdatePointsToComponentCounter (colony, updatePoints);
}

float ColoniesEvolutionManager::ProcessColonyPopulation (GameConfiguration *configuration, District *colony, float timeStep)
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

float ColoniesEvolutionManager::ProcessColonyEvolutionBranch (GameConfiguration *configuration, District *colony,
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
    return Urho3D::Abs (evolutionModifer) * 1500.0f;
}
}
