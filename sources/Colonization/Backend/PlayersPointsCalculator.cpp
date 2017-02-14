#include <Colonization/BuildConfiguration.hpp>
#include "PlayersPointsCalculator.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
float PlayersPointsCalculator::CalculateColonyPoints (District *district, GameConfiguration *configuration)
{
    assert (district->HasColony ());
    float modifer = configuration->GetColonyPointsModifer ();
    float population = district->GetMenCount () + district->GetWomenCount ();
    float totalEvolution = district->GetFarmsEvolutionPoints () + district->GetMinesEvolutionPoints () +
            district->GetIndustryEvolutionPoints () + district->GetLogisticsEvolutionPoints () +
            district->GetDefenseEvolutionPoints ();
    return (modifer * population * totalEvolution);
}

float PlayersPointsCalculator::CalculateUnitPoints (Unit *unit, GameConfiguration *configuration)
{
    float modifer = 0.0f;
    float mainProperty = 0.0f;

    if (unit->GetUnitType () == UNIT_FLEET)
    {
        modifer = configuration->GetUnitFleetPointsModifer ();
        mainProperty = unit->FleetUnitGetWarShipsCount ();
    }
    else if (unit->GetUnitType () == UNIT_TRADERS)
    {
        modifer = configuration->GetUnitTradersPointsModifer ();
        mainProperty = unit->TradersUnitGetTradeGoodsCost ();
    }
    else if (unit->GetUnitType () == UNIT_COLONIZATORS)
    {
        modifer = configuration->GetUnitColonizatorsPointsModifer ();
        mainProperty = unit->ColonizatorsUnitGetColonizatorsCount ();
    }
    else if (unit->GetUnitType () == UNIT_ARMY)
    {
        modifer = configuration->GetUnitArmyPointsModifer ();
        mainProperty = unit->ArmyUnitGetSoldiersCount ();
    }

    return (modifer * mainProperty);
}

void PlayersPointsCalculator::UpdatePlayerPoints (Player *player, GameConfiguration *configuration, Map *map, UnitsManager *unitsManager)
{
    float summaryPoints = 0.0f;
    Urho3D::PODVector <Urho3D::StringHash> coloniesHashes = map->GetColoniesOfPlayer (Urho3D::StringHash (player->GetName ()));
    if (!coloniesHashes.Empty ())
    {
        for (int index = 0; index < coloniesHashes.Size (); index++)
        {
            District *colony = map->GetDistrictByHash (coloniesHashes.At (index));
            float points = CalculateColonyPoints (colony, configuration);
            summaryPoints += points;
        }
    }

    Urho3D::PODVector <Urho3D::StringHash> unitsHashes = unitsManager->GetUnitsOfPlayer (Urho3D::StringHash (player->GetName ()));
    if (!unitsHashes.Empty ())
    {
        for (int index = 0; index < unitsHashes.Size (); index++)
        {
            Unit *unit = unitsManager->GetUnitByHash (unitsHashes.At (index));
            float points = CalculateUnitPoints (unit, configuration);
            summaryPoints += points;
        }
    }
    player->SetPoints (summaryPoints);
}

void PlayersPointsCalculator::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (PlayersPointsCalculator, Update));
}

PlayersPointsCalculator::PlayersPointsCalculator (Urho3D::Context *context) : Urho3D::Component (context)
{

}

PlayersPointsCalculator::~PlayersPointsCalculator ()
{

}

void PlayersPointsCalculator::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <PlayersPointsCalculator> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void PlayersPointsCalculator::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        assert (node_);
        PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        UnitsManager *unitsManager = node_->GetScene ()->GetChild ("units")->GetComponent <UnitsManager> ();
        GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();

        assert (playersManager);
        assert (map);
        assert (unitsManager);
        assert (configuration);

        for (int index = 0; index < playersManager->GetPlayersCount (); index++)
        {
            Player *player = playersManager->GetPlayerByIndex (index);
            if (player)
            {
                UpdatePlayerPoints (player, configuration, map, unitsManager);
            }
        }
    }
}
}
