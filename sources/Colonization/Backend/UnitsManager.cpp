#include <Colonization/BuildConfiguration.hpp>
#include "UnitsManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Core/Unit/UnitTags.hpp>
#include <Colonization/Core/Unit/UnitEvents.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/District/District.hpp>

#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void UnitsManager::SettleColonizator (ColonizatorsUnit *unit, Map *map)
{
    assert (unit);
    District *colony = map->GetDistrictByHash (unit->GetPositionHash ());
    assert (colony);

    if (!colony->GetHasColony ())
    {
        colony->SetHasColony (true);
        colony->SetColonyOwnerName (unit->GetOwnerPlayerName ());

        colony->SetFarmsEvolutionPoints (1.0f);
        colony->SetMinesEvolutionPoints (1.0f);
        colony->SetIndustryEvolutionPoints (1.0f);
        colony->SetLogisticsEvolutionPoints (1.0f);
        colony->SetDefenseEvolutionPoints (1.0f);
    }

    if (colony->GetHasColony () && colony->GetColonyOwnerName () != unit->GetOwnerPlayerName ())
    {
        Urho3D::Log::Write (Urho3D::LOG_WARNING, "Can't settle colonizator of " + unit->GetOwnerPlayerName () +
                            " in " + colony->GetName () + ". Because there is a colony of " + colony->GetColonyOwnerName () + "!");
        // TODO: Add ability to reselect colonizators target.
        unit->GetNode ()->Remove ();
    }
    else
    {
        // TODO: If there are a lot of women in colony and small count of men, spawn much more men.
        // TODO: New colonists should decrease evolution points. For examle, NewEvolution = (OldEvolution * oldPopulation / (oldPopulation + colonists)) + colonists / (oldPopulation + colonists)
        float menPercent = Urho3D::Random (0.45f, 0.55f);
        colony->SetMenCount (colony->GetMenCount () + unit->GetColonizatorsCount () * 1.0f * menPercent);
        colony->SetWomenCount (colony->GetWomenCount () + unit->GetColonizatorsCount () * 1.0f * (1.0f - menPercent));
        unit->GetNode ()->Remove ();

        NetworkUpdateCounter *counter = colony->GetNode ()->GetComponent <NetworkUpdateCounter> ();
        if (!counter)
        {
            counter = CreateNetworkUpdateCounterForComponent (colony);
        }
        counter->AddUpdatePoints (100.0f);
    }
}

void UnitsManager::ProcessTrader (GameConfiguration *configuration, TradersUnit *unit)
{
    assert (unit);
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    assert (playersManager);

    Player *player = playersManager->GetPlayerByNameHash (Urho3D::String (unit->GetOwnerPlayerName ()));
    assert (player);

    float externalTaxes = configuration->GetExternalTaxes ();
    player->SetGold (player->GetGold () + unit->GetTradeGoodsCost () * externalTaxes);
    unit->GetNode ()->Remove ();
}

float UnitsManager::GetUnitSpeedBetween (District *position, District *target, GameConfiguration *configuration)
{
    if (position->GetIsSea () && target->GetIsSea ())
    {
        return configuration->GetSailSpeed ();
    }
    else if (!position->GetIsSea () && !target->GetIsSea ())
    {
        return configuration->GetMarchSpeed ();
    }
    else if (!position->GetIsSea () && target->GetIsSea ())
    {
        return configuration->GetEmbarkationSpeed ();
    }
    else if (position->GetIsSea () && !target->GetIsSea ())
    {
        return configuration->GetDisembarkationSpeed ();
    }
    else
    {
        return 0.0f;
    }
}

bool UnitsManager::OnNextTargetReached (Unit *unit, Urho3D::PODVector <Urho3D::StringHash> &unitWay,
                                        Map *map, GameConfiguration *configuration)
{
    unit->SetPositionHash (unitWay.At (0));
    unitWay.Remove (unitWay.At (0));
    unit->SetWay (unitWay);
    unit->SetWayToNextDistrictProgressInPercents (0.0f);

    if (unitWay.Empty () && unit->GetUnitType () == UNIT_COLONIZATORS)
    {
        SettleColonizator (((ColonizatorsUnit *) (unit)), map);
        return false;
    }
    else if (unitWay.Empty () && unit->GetUnitType () == UNIT_TRADERS)
    {
        ProcessTrader (configuration, ((TradersUnit *) (unit)));
        return false;
    }
    else
    {
        Urho3D::VariantMap eventData;
        eventData [UnitPositionChanged::UNIT_HASH] = unit->GetHash ();
        SendEvent (EVENT_UNIT_POSITION_CHANGED, eventData);
        return true;
    }
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
        RemoveDeadUnits ();

        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
        assert (map);
        assert (configuration);
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();

        for (int index = 0; index < units_.Size (); index++)
        {
            Unit *unit = units_.At (index);
            Urho3D::PODVector <Urho3D::StringHash> unitWay = unit->GetWay ();
            if (!unitWay.Empty () && !unit->GetIsInBattle ())
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

                bool unitExists = true;
                if (unit->IsCanGoTo (nextTarget, map))
                {
                    float distance = (unitPosition->GetUnitPosition () - nextTarget->GetUnitPosition ()).Length ();
                    float speed = GetUnitSpeedBetween (unitPosition, nextTarget, configuration);

                    float addition = (100.0f * speed * timeStep) / distance;
                    float oldProgress = unit->GetWayToNextDistrictProgressInPercents ();
                    unit->SetWayToNextDistrictProgressInPercents (oldProgress + addition);

                    if (unit->GetWayToNextDistrictProgressInPercents () >= 100.0f)
                    {
                        unitExists = OnNextTargetReached (unit, unitWay, map, configuration);
                        updatePoints += 100.0f;
                    }
                    else
                    {
                        updatePoints += (addition * 20.0f);
                    }
                }
                else
                {
                    unitWay = Urho3D::PODVector <Urho3D::StringHash> ();
                    unit->SetWay (unitWay);
                    updatePoints += 100.0f;
                }

                if (unit->GetNode () && unitExists)
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
}

void UnitsManager::UpdateUnitsList ()
{
    // Reload units array from child nodes.
    assert (node_);
    units_.Clear ();
    Urho3D::PODVector <Urho3D::Node *> unitsNodes;
    node_->GetChildrenWithTag (unitsNodes, TAG_UNIT);
    for (int index = 0; index < unitsNodes.Size (); index++)
    {
        Urho3D::Node *unitNode = unitsNodes.At (index);
        if (unitNode->GetID () < Urho3D::FIRST_LOCAL_ID)
        {
            Unit *unit = unitNode->GetDerivedComponent <Unit> ();
            if (unit)
            {
                units_.Push (Urho3D::SharedPtr <Unit> (unit));
            }
        }
    }
}

void UnitsManager::RemoveDeadUnits ()
{
    Urho3D::Vector <Urho3D::SharedPtr <Unit> >::Iterator iterator = units_.Begin ();
    while (iterator != units_.End ())
    {
        if (iterator->Get ()->IsAlive ())
        {
            iterator++;
        }
        else
        {
            iterator->Get ()->GetNode ()->Remove ();
            iterator = units_.Erase (iterator);
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

Urho3D::PODVector <Urho3D::StringHash> UnitsManager::GetUnitsOfPlayer (Urho3D::StringHash playerNameHash) const
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

Unit *UnitsManager::CreateUnit (UnitType unitType, Urho3D::String ownerPlayerName, Urho3D::StringHash positionHash)
{
    assert (node_);
    Urho3D::Node *unitNode = node_->CreateChild ("unit", Urho3D::REPLICATED);
    Urho3D::SharedPtr <Unit> unit;
    if (unitType == UNIT_FLEET)
    {
        unit = Urho3D::SharedPtr <Unit> (unitNode->CreateComponent <FleetUnit> (Urho3D::REPLICATED));
    }
    else if (unitType == UNIT_TRADERS)
    {
        unit = Urho3D::SharedPtr <Unit> (unitNode->CreateComponent <TradersUnit> (Urho3D::REPLICATED));
    }
    else if (unitType == UNIT_COLONIZATORS)
    {
        unit = Urho3D::SharedPtr <Unit> (unitNode->CreateComponent <ColonizatorsUnit> (Urho3D::REPLICATED));
    }
    else if (unitType == UNIT_ARMY)
    {
        unit = Urho3D::SharedPtr <Unit> (unitNode->CreateComponent <ArmyUnit> (Urho3D::REPLICATED));
    }

    if (unit.NotNull ())
    {
        unitNode->SetName (unit->GetUnitTypeTag () + Urho3D::String (unitNode->GetID ()));
        unitNode->AddTag (TAG_UNIT);
        unitNode->AddTag (unit->GetUnitTypeTag ());

        units_.Push (unit);
        unit->SetPositionHash (positionHash);
        unit->SetOwnerPlayerName (ownerPlayerName);
        unit->UpdateHash (this);

        Urho3D::VariantMap eventData;
        eventData [UnitCreated::UNIT_HASH] = unit->GetHash ();
        SendEvent (EVENT_UNIT_CREATED, eventData);
    }
    else
    {
        unitNode->Remove ();
    }
    return unit;
}

Urho3D::PODVector <Unit *> GetUnitsInDistrict (Urho3D::Scene *scene, Urho3D::StringHash districtHash)
{
    Urho3D::PODVector <Unit *> unitsInDistrict;
    Urho3D::PODVector <Urho3D::Node *> unitsNodes;
    scene->GetChild ("units")->GetChildrenWithTag (unitsNodes, TAG_UNIT);

    for (int index = 0; index < unitsNodes.Size (); index++)
    {
        Urho3D::Node *unitNode = unitsNodes.At (index);
        if (unitNode->GetID () < Urho3D::FIRST_LOCAL_ID)
        {
            Unit *unit = unitNode->GetDerivedComponent <Unit> ();
            if (unit && unit->GetPositionHash () == districtHash)
            {
                unitsInDistrict.Push (unit);
            }
        }
    }
    return unitsInDistrict;
}
}

