#include <Colonization/BuildConfiguration.hpp>
#include "TradeProcessor.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
void TradeProcessor::UpdateTradeAreas (float updateDelay)
{
    tradeAreas_.Clear ();
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    Urho3D::HashMap <Urho3D::StringHash, District *> toScanHashMap;

    for (int index = 0; index < map->GetDistrictsCount (); index++)
    {
        District *district = map->GetDistrictByIndex (index);
        if (district->HasColony ())
        {
            float logisticsEvolutionPoints = district->GetLogisticsEvolutionPoints ();
            while (toScanHashMap [Urho3D::StringHash (static_cast <int> (logisticsEvolutionPoints * 1000))])
            {
                logisticsEvolutionPoints += 0.001f;
            }
            toScanHashMap [Urho3D::StringHash (static_cast <int> (logisticsEvolutionPoints * 1000))] = district;
        }
    }

    toScanHashMap.Sort ();
    Urho3D::PODVector <District *> toScan;
    for (int index = toScanHashMap.Values ().Size () - 1; index >= 0; index--)
    {
        toScan.Push (toScanHashMap.Values ().At (index));
    }

    assert (node_);
    Urho3D::PODVector <Urho3D::Node *> tradeAreasNodes;
    node_->GetChildrenWithComponent (tradeAreasNodes, InternalTradeArea::GetTypeStatic ());
    int tradeAreaIndex = 0;

    while (!toScan.Empty () || tradeAreaIndex < tradeAreasNodes.Size ())
    {
        if (!toScan.Empty ())
        {
            Urho3D::Node *node;
            if (tradeAreaIndex < tradeAreasNodes.Size ())
            {
                node = tradeAreasNodes.At (tradeAreaIndex);
            }
            else
            {
                node = node_->CreateChild ("InternalTradeArea" + Urho3D::String (tradeAreaIndex), Urho3D::REPLICATED);
                node->CreateComponent <InternalTradeArea> (Urho3D::REPLICATED);
            }

            Urho3D::SharedPtr <InternalTradeArea> tradeAreaSharedPtr (node->GetComponent <InternalTradeArea> ());
            UpdateTradeArea (tradeAreaSharedPtr, map, toScan.At (0), toScan);
            tradeAreas_.Push (tradeAreaSharedPtr);
            tradeAreaSharedPtr->MarkNetworkUpdate ();
        }
        else
        {
            Urho3D::Node *node = tradeAreasNodes.At (tradeAreaIndex);
            node->Remove ();
        }
        tradeAreaIndex++;
    }
    tradeAreasNodes.Clear ();

    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    for (int index = 0; index < tradeAreas_.Size (); index++)
    {
        ProcessTradeAreaIncome (playersManager, map, tradeAreas_.At (index), updateDelay);
    }

}

void TradeProcessor::UpdateTradeArea (InternalTradeArea *tradeArea, Map *map, District *start, Urho3D::PODVector<District *> &unscannedList)
{
    Urho3D::PODVector <District *> areaDistricts;
    ProcessTradeAreaDistrict (map, start, areaDistricts, unscannedList);

    Urho3D::PODVector <Urho3D::StringHash> areaDistrictsHashes;
    for (int index = 0; index < areaDistricts.Size (); index++)
    {
        areaDistrictsHashes.Push (areaDistricts.At (index)->GetHash ());
    }

    assert (node_);
    if (tradeArea->GetDistrictsHashesArray () != areaDistrictsHashes)
    {
        while (tradeArea->GetDistrictsHashesCount ())
        {
            tradeArea->RemoveDistrictHash (tradeArea->GetDistrictHashByIndex (0));
        }

        for (int index = 0; index < areaDistrictsHashes.Size (); index++)
        {
            tradeArea->AddDistrictHash (areaDistrictsHashes.At (index));
        }
    }
}

void TradeProcessor::ProcessTradeAreaDistrict (Map *map, District *district, Urho3D::PODVector <District *> &areaDistricts, Urho3D::PODVector <District *> &unscannedList)
{
    areaDistricts.Push (district);
    unscannedList.Remove (district);

    Urho3D::PODVector <District *> neighbors;
    if (district->GetLogisticsEvolutionPoints () >= 4.0f)
    {
        Urho3D::PODVector <Urho3D::StringHash> districtNeighborsHashes = district->GetNeighborsHashes ();
        for (int index = 0; index < districtNeighborsHashes.Size (); index++)
        {
            District *neighbor = map->GetDistrictByHash (districtNeighborsHashes.At (index));
            assert (neighbor);

            if (neighbor->HasColony () && neighbor->GetColonyOwnerName () == district->GetColonyOwnerName ())
            {
                neighbors.Push (neighbor);
            }

            else if (district->GetLogisticsEvolutionPoints () >= 6.0f && neighbor->IsSea () && !neighbor->IsImpassable ())
            {
                Urho3D::PODVector <Urho3D::StringHash> neighborsOfNeighborsHashes = neighbor->GetNeighborsHashes ();
                for (int neighborOfNeighboarIndex = 0; neighborOfNeighboarIndex < neighborsOfNeighborsHashes.Size (); neighborOfNeighboarIndex++)
                {
                    District *neighborOfNeighbor = map->GetDistrictByHash (neighborsOfNeighborsHashes.At (neighborOfNeighboarIndex));
                    if (neighborOfNeighbor->HasColony () && neighborOfNeighbor->GetColonyOwnerName () == district->GetColonyOwnerName ())
                    {
                        neighbors.Push (neighborOfNeighbor);
                    }
                }
            }
        }
    }

    if (!neighbors.Empty ())
    {
        for (int index = 0; index < neighbors.Size (); index++)
        {
            District *neighbor = neighbors.At (index);
            if (unscannedList.Contains (neighbor))
            {
                ProcessTradeAreaDistrict (map, neighbor, areaDistricts, unscannedList);
            }
        }
    }
}

void TradeProcessor::ProcessTradeAreaIncome (PlayersManager *playersManager, Map *map, InternalTradeArea *tradeArea, float updateDelay)
{
    Player *player = playersManager->GetPlayer (Urho3D::StringHash (
                                                    map->GetDistrictByHash (tradeArea->GetDistrictHashByIndex (0))->
                                                    GetColonyOwnerName ()));
    if (!player)
    {
        Urho3D::Log::Write (Urho3D::LOG_ERROR, "Found colony of null player! Colony: " +
                            map->GetDistrictByHash (tradeArea->GetDistrictHashByIndex (0))->GetName () + ", player: " +
                            map->GetDistrictByHash (tradeArea->GetDistrictHashByIndex (0))->GetColonyOwnerName () + ".");
    }
    else
    {
        GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
        assert (configuration);
        float internalTaxes = configuration->GetInternalTaxes ();

        Urho3D::SharedPtr <TradeDistrictProcessingInfo> result = tradeArea->ProcessTrade (map);
        float playersIncome = result->GetSoldTradeGoodsCost () * result->GetLogisticsBonus () *
                result->GetDefenseBonus () * updateDelay * internalTaxes;
        player->SetGold (player->GetGold () + playersIncome);

        UnitsManager *unitsManager = node_->GetScene ()->GetChild ("units")->GetComponent <UnitsManager> ();
        assert (unitsManager);
        float unsoldGoldPerDistrict = result->GetUnsoldTradeGoodsCost () * updateDelay  / tradeArea->GetDistrictsHashesCount ();
        for (int index = 0; index < tradeArea->GetDistrictsHashesCount (); index++)
        {
            District *district = map->GetDistrictByHash (tradeArea->GetDistrictHashByIndex (index));
            assert (district);

            Unit *unit = unitsManager->CreateUnit ();
            unit->SetUnitType (UNIT_TRADERS);
            unit->TradersUnitSetTradeGoodsCost (unsoldGoldPerDistrict);
            unit->SetOwnerPlayerName (district->GetColonyOwnerName ());
            unit->SetPositionHash (district->GetHash ());

            GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
            District *nearestEuropeDistrict = 0;
            Urho3D::PODVector <Urho3D::StringHash> wayToEuropeDistricts = configuration->GetWayToEuropeDistricts ();
            nearestEuropeDistrict = map->GetDistrictByHash (wayToEuropeDistricts.At (0));
            assert (nearestEuropeDistrict);
            float minDistance = (nearestEuropeDistrict->GetUnitPosition () - district->GetUnitPosition ()).Length ();

            if (wayToEuropeDistricts.Size () > 1)
            {
                for (int index = 1; index < wayToEuropeDistricts.Size (); index++)
                {
                    District *europeDistrict = map->GetDistrictByHash (wayToEuropeDistricts.At (index));
                    assert (district);
                    if ((europeDistrict->GetUnitPosition () - district->GetUnitPosition ()).Length () < minDistance)
                    {
                        nearestEuropeDistrict = europeDistrict;
                        minDistance = (europeDistrict->GetUnitPosition () - district->GetUnitPosition ()).Length ();
                    }
                }
            }

            unit->SetWay (map->FindPath (unit->GetPositionHash (), nearestEuropeDistrict->GetHash (),
                                         unit->GetOwnerPlayerName (), true, false));
            unit->UpdateHash (unitsManager);
        }
    }
}

void TradeProcessor::ClearTradeAreas ()
{
    for (int index = 0; index < tradeAreas_.Size (); index++)
    {
        tradeAreas_.At (index)->GetNode ()->Remove ();
    }
    tradeAreas_.Clear ();
}

void TradeProcessor::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (TradeProcessor, Update));
}

TradeProcessor::TradeProcessor (Urho3D::Context *context) : Urho3D::Component (context)
{
    untilTradeAreasUpdate_ = 0.0001f;
}

TradeProcessor::~TradeProcessor ()
{
    tradeAreas_.Clear ();
}

void TradeProcessor::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <TradeProcessor> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void TradeProcessor::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
        untilTradeAreasUpdate_ -= timeStep;
        if (untilTradeAreasUpdate_ <= 0.0f)
        {
            UpdateTradeAreas (10.0f);
            untilTradeAreasUpdate_ = 10.0f;
        }
    }
}

int TradeProcessor::GetTradeAreasCount ()
{
    return tradeAreas_.Size ();
}

InternalTradeArea *TradeProcessor::GetTradeAreaByIndex (int index)
{
    assert (index < tradeAreas_.Size ());
    return tradeAreas_.At (index);
}
}
