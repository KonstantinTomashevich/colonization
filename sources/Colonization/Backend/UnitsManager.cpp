#include <Colonization/BuildConfiguration.hpp>
#include "UnitsManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void UnitsManager::SettleColonizator (Unit *unit, Map *map)
{
    assert (unit);
    assert (unit->GetUnitType () == UNIT_COLONIZATORS);

    District *colony = map->GetDistrictByHash (unit->GetPositionHash ());
    assert (colony);

    if (!colony->HasColony ())
    {
        colony->SetColony (true);
        colony->SetColonyOwnerName (unit->GetOwnerPlayerName ());

        colony->SetFarmsEvolutionPoints (2.0f);
        colony->SetMinesEvolutionPoints (0.5f);
        colony->SetIndustryEvolutionPoints (0.1f);
        colony->SetLogisticsEvolutionPoints (0.5f);
        colony->SetDefenseEvolutionPoints (0.25f);
    }

    if (colony->HasColony () && colony->GetColonyOwnerName () != unit->GetOwnerPlayerName ())
    {
        Urho3D::Log::Write (Urho3D::LOG_WARNING, "Can't settle colonizator of " + unit->GetOwnerPlayerName () +
                            " in " + colony->GetName () + ". Because there is a colony of " + colony->GetColonyOwnerName () + "!");
        unit->GetNode ()->Remove ();
    }
    else
    {
        float menPercent = Urho3D::Random (0.4f, 0.6f);
        colony->SetMenCount (colony->GetMenCount () + unit->ColonizatorsUnitGetColonizatorsCount () * 1.0f * menPercent);
        colony->SetWomenCount (colony->GetWomenCount () + unit->ColonizatorsUnitGetColonizatorsCount () * 1.0f * (1.0f - menPercent));
        unit->GetNode ()->Remove ();

        NetworkUpdateCounter *counter = colony->GetNode ()->GetComponent <NetworkUpdateCounter> ();
        if (!counter)
        {
            counter = CreateNetworkUpdateCounterForComponent (colony);
        }
        counter->AddUpdatePoints (100.0f);
    }
}

void UnitsManager::ProcessTrader (GameConfiguration *configuration, Unit *unit)
{
    assert (unit);
    assert (unit->GetUnitType () == UNIT_TRADERS);

    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    assert (playersManager);

    Player *player = playersManager->GetPlayerByNameHash (Urho3D::String (unit->GetOwnerPlayerName ()));
    assert (player);

    float externalTaxes = configuration->GetExternalTaxes ();
    player->SetGold (player->GetGold () + unit->TradersUnitGetTradeGoodsCost () * externalTaxes);
    unit->GetNode ()->Remove ();
}

void UnitsManager::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (UnitsManager, Update));
}

UnitsManager::UnitsManager (Urho3D::Context *context) : Urho3D::Component (context),
    units_ ()
{

}

UnitsManager::~UnitsManager ()
{
    units_.Clear ();
}

void UnitsManager::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    for (int index = 0; index < units_.Size (); index++)
    {
        units_.At (index)->DrawDebugGeometry (debug, depthTest);
    }
}

void UnitsManager::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <UnitsManager> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void UnitsManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        UpdateUnitsList ();
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
        assert (map);
        assert (configuration);

        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
        float sailSpeed = configuration->GetSailSpeed ();
        float marchSpeed = configuration->GetMarchSpeed ();
        float embarkationSpeed = configuration->GetEmbarkationSpeed ();
        float disembarkationSpeed = configuration->GetDisembarkationSpeed ();

        for (int index = 0; index < units_.Size (); index++)
        {
            Unit *unit = units_.At (index);
            Urho3D::PODVector <Urho3D::StringHash> unitWay = unit->GetWay ();
            if (!unitWay.Empty ())
            {
                float updatePoints = 0.0f;
                if (unit->GetPositionHash () == unitWay.At (0))
                {
                    unitWay.Remove (unitWay.At (0));
                    unit->SetWay (unitWay);
                    updatePoints += 100.0f;
                }

                District *unitPosition = map->GetDistrictByHash (unit->GetPositionHash ());
                assert (unitPosition);

                District *nextTarget = map->GetDistrictByHash (unitWay.At (0));
                assert (nextTarget);

                float distance = (unitPosition->GetUnitPosition () - nextTarget->GetUnitPosition ()).Length ();
                float speed;

                if (unitPosition->IsSea () && nextTarget->IsSea ())
                {
                    speed = sailSpeed;
                }
                else if (!unitPosition->IsSea () && !nextTarget->IsSea ())
                {
                    speed = marchSpeed;
                }
                else if (!unitPosition->IsSea () && nextTarget->IsSea ())
                {
                    speed = embarkationSpeed;
                }
                else if (unitPosition->IsSea () && !nextTarget->IsSea ())
                {
                    speed = disembarkationSpeed;
                }
                else
                {
                    speed = 0.0f;
                }

                float addition = (100.0f * speed * timeStep) / distance;
                float oldProgress = unit->GetWayToNextDistrictProgressInPercents ();
                unit->SetWayToNextDistrictProgressInPercents (oldProgress + addition);

                bool exists = true;
                if (unit->GetWayToNextDistrictProgressInPercents () >= 100.0f)
                {
                    unit->SetPositionHash (unitWay.At (0));
                    unitWay.Remove (unitWay.At (0));
                    unit->SetWay (unitWay);
                    unit->SetWayToNextDistrictProgressInPercents (0.0f);

                    if (unitWay.Empty () && unit->GetUnitType () == UNIT_COLONIZATORS)
                    {
                        SettleColonizator (unit, map);
                        exists = false;
                    }
                    else if (unitWay.Empty () && unit->GetUnitType () == UNIT_TRADERS)
                    {
                        ProcessTrader (configuration, unit);
                        exists = false;
                    }
                    else
                    {
                        updatePoints += 100.0f;
                    }
                }
                else
                {
                    updatePoints += (addition * 20.0f);
                }

                if (unit->GetNode () && exists)
                {
                    NetworkUpdateCounter *counter = unit->GetNode ()->GetComponent <NetworkUpdateCounter> ();
                    if (!counter)
                    {
                        counter = CreateNetworkUpdateCounterForComponent (unit);
                    }
                    counter->AddUpdatePoints (updatePoints);
                }
            }
        }
    }

    // TODO: To be continued...
}

void UnitsManager::UpdateUnitsList ()
{
    // Reload units array from child nodes.
    assert (node_);
    units_.Clear ();
    Urho3D::PODVector <Urho3D::Node *> unitsNodes;
    node_->GetChildrenWithComponent (unitsNodes, Unit::GetTypeStatic ());
    for (int index = 0; index < unitsNodes.Size (); index++)
    {
        Urho3D::Node *unitNode = unitsNodes.At (index);
        if (unitNode->GetID () < Urho3D::FIRST_LOCAL_ID)
        {
            units_.Push (Urho3D::SharedPtr <Unit> (unitNode->GetComponent <Unit> ()));
        }
    }
}

int UnitsManager::GetUnitsCount () const
{
    return units_.Size ();
}

Unit *UnitsManager::GetUnitByIndex (int index) const
{
    assert (index < units_.Size ());
    return units_.At (index);
}

Unit *UnitsManager::GetUnitByHash (Urho3D::StringHash hash) const
{
    for (int index = 0; index < units_.Size (); index++)
    {
        if (units_.At (index)->GetHash () == hash)
        {
            return units_.At (index);
        }
    }
    return 0;
}

Urho3D::PODVector <Urho3D::StringHash> UnitsManager::GetUnitsOfPlayer (Urho3D::StringHash playerNameHash)
{
    Urho3D::PODVector <Urho3D::StringHash> units;
    for (int index = 0; index < units_.Size (); index++)
    {
        Unit *unit = units_.At (index);
        if (Urho3D::StringHash (unit->GetOwnerPlayerName ()) == playerNameHash)
        {
            units.Push (unit->GetHash());
        }
    }
    return units;
}

Unit *UnitsManager::CreateUnit ()
{
    assert (node_);
    Urho3D::Node *unitNode = node_->CreateChild ("unit", Urho3D::REPLICATED);
    unitNode->SetName ("Unit" + Urho3D::String (unitNode->GetID ()));
    Urho3D::SharedPtr <Unit> unit (unitNode->CreateComponent <Unit> (Urho3D::REPLICATED));
    units_.Push (unit);
    return unit;
}
}

