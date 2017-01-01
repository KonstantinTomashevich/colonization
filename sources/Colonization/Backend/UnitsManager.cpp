#include <Colonization/BuildConfiguration.hpp>
#include "UnitsManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Categories.hpp>

namespace Colonization
{
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
            units_.Push (Urho3D::SharedPtr <Unit> (unitNode->GetComponent <Unit> ()));
    }
}

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
        float mansPercent = Urho3D::Random (0.4f, 0.6f);
        colony->SetMenCount (colony->GetMenCount () + unit->ColonizatorsUnitGetColonizatorsCount () * 1.0f * mansPercent);
        colony->SetWomenCount (colony->GetWomenCount () + unit->ColonizatorsUnitGetColonizatorsCount () * 1.0f * (1.0f - mansPercent));
        unit->GetNode ()->Remove ();
        colony->MarkNetworkUpdate ();
    }
}

void UnitsManager::ProcessTrader (Unit *unit)
{
    assert (unit);
    assert (unit->GetUnitType () == UNIT_TRADERS);

    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    assert (playersManager);

    Player *player = playersManager->GetPlayer (Urho3D::String (unit->GetOwnerPlayerName ()));
    assert (player);

    float externalTaxes = context_->GetGlobalVar ("externalTaxes").GetFloat ();
    player->SetGold (player->GetGold () + unit->TradersUnitGetTradeGoodsCost () * externalTaxes);
    unit->GetNode ()->Remove ();
}

UnitsManager::UnitsManager (Urho3D::Context *context) : Urho3D::Component (context),
    units_ ()
{
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (UnitsManager, Update));
}

UnitsManager::~UnitsManager ()
{
    units_.Clear ();
}

void UnitsManager::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <UnitsManager> (COLONIZATION_SERVER_ONLY_CATEGORY);
}

void UnitsManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        UpdateUnitsList ();
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        assert (map);

        float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
        float sailSpeed = context_->GetGlobalVar ("sailSpeed").GetFloat ();
        float marchSpeed = context_->GetGlobalVar ("marchSpeed").GetFloat ();
        float embarkationSpeed = context_->GetGlobalVar ("embarkationSpeed").GetFloat ();
        float disembarkationSpeed = context_->GetGlobalVar ("disembarkationSpeed").GetFloat ();

        for (int index = 0; index < units_.Size (); index++)
        {
            Unit *unit = units_.At (index);
            Urho3D::PODVector <Urho3D::StringHash>  unitWay = unit->GetWay ();
            if (!unitWay.Empty ())
            {
                if (unit->GetPositionHash () == unitWay.At (0))
                {
                    unitWay.Remove (unitWay.At (0));
                    unit->SetWay (unitWay);
                    unit->MarkNetworkUpdate ();
                }

                District *unitPosition = map->GetDistrictByHash (unit->GetPositionHash ());
                assert (unitPosition);

                District *nextTarget = map->GetDistrictByHash (unitWay.At (0));
                assert (nextTarget);

                float distance = (unitPosition->GetUnitPosition () - nextTarget->GetUnitPosition ()).Length ();
                float speed;

                if (unitPosition->IsSea () && nextTarget->IsSea ())
                    speed = sailSpeed;
                else if (!unitPosition->IsSea () && !nextTarget->IsSea ())
                    speed = marchSpeed;
                else if (!unitPosition->IsSea () && nextTarget->IsSea ())
                    speed = embarkationSpeed;
                else if (unitPosition->IsSea () && !nextTarget->IsSea ())
                    speed = disembarkationSpeed;
                else
                    speed = 0.0f;

                float addition = (100.0f * speed * timeStep) / distance;
                unit->SetWayToNextDistrictProgressInPercents (unit->GetWayToNextDistrictProgressInPercents () + addition);

                if (unit->GetWayToNextDistrictProgressInPercents () >= 100.0f)
                {
                    unit->SetPositionHash (unitWay.At (0));
                    unitWay.Remove (unitWay.At (0));
                    unit->SetWayToNextDistrictProgressInPercents (0.0f);
                    unit->SetWay (unitWay);

                    if (unitWay.Empty () && unit->GetUnitType () == UNIT_COLONIZATORS)
                        SettleColonizator (unit, map);
                    else if (unitWay.Empty () && unit->GetUnitType () == UNIT_TRADERS)
                        ProcessTrader (unit);
                }
                unit->MarkNetworkUpdate ();
            }
        }

        // TODO: To be continued...
    }
}

int UnitsManager::GetUnitsCount ()
{
    return units_.Size ();
}

Unit *UnitsManager::GetUnitByIndex (int index)
{
    assert (index < units_.Size ());
    return units_.At (index);
}

Unit *UnitsManager::GetUnitByHash (Urho3D::StringHash hash)
{
    for (int index = 0; index < units_.Size (); index++)
        if (units_.At (index)->GetHash () == hash)
            return units_.At (index);
    return 0;
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

