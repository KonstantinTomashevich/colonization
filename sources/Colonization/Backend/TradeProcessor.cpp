#include <Colonization/BuildConfiguration.hpp>
#include "TradeProcessor.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/District/DistrictComparator.hpp>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void TradeProcessor::UpdateTradeAreas (float updateDelay)
{
    tradeAreas_.Clear ();
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
    assert (configuration);
    assert (map);
    Urho3D::PODVector <District *> toScan;

    for (int index = 0; index < map->GetDistrictsCount (); index++)
    {
        District *district = map->GetDistrictByIndex (index);
        if (district->GetHasColony ())
        {
            toScan.Push (district);
        }
    }

    Urho3D::Sort <District *, DistrictComparator> (toScan.Begin (), toScan.End (), DistrictComparators::HigherLogistics);
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
            float updatePoints = UpdateTradeArea (tradeAreaSharedPtr, map, toScan.At (0), toScan, configuration);
            tradeAreas_.Push (tradeAreaSharedPtr);

            NetworkUpdateCounter *counter = tradeAreaSharedPtr->GetNode ()->GetComponent <NetworkUpdateCounter> ();
            if (!counter)
            {
                counter = CreateNetworkUpdateCounterForComponent (tradeAreaSharedPtr);
            }
            counter->AddUpdatePoints (updatePoints);
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
        ProcessTradeAreaIncome (playersManager, map, tradeAreas_.At (index), configuration, updateDelay);
    }
}

float TradeProcessor::UpdateTradeArea (InternalTradeArea *tradeArea, Map *map, District *start,
                                       Urho3D::PODVector <District *> &unscannedList, GameConfiguration *configuration)
{
    Urho3D::PODVector <District *> areaDistricts;
    ProcessTradeAreaDistrict (map, start, areaDistricts, unscannedList, configuration);

    Urho3D::PODVector <Urho3D::StringHash> areaDistrictsHashes;
    for (int index = 0; index < areaDistricts.Size (); index++)
    {
        areaDistrictsHashes.Push (areaDistricts.At (index)->GetHash ());
    }

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

        return 100.0f;
    }
    else
    {
        return 0.0f;
    }
}

void TradeProcessor::ProcessTradeAreaDistrict (Map *map, District *district, Urho3D::PODVector <District *> &areaDistricts,
                                               Urho3D::PODVector <District *> &unscannedList, GameConfiguration *configuration)
{
    areaDistricts.Push (district);
    unscannedList.Remove (district);

    Urho3D::PODVector <District *> neighbors;
    if (district->GetLogisticsEvolutionPoints () >= configuration->GetMinimumLogisticsToUniteDistrictsToTradeArea ())
    {
        Urho3D::PODVector <Urho3D::StringHash> districtNeighborsHashes = district->GetNeighborsHashes ();
        for (int index = 0; index < districtNeighborsHashes.Size (); index++)
        {
            District *neighbor = map->GetDistrictByHash (districtNeighborsHashes.At (index));
            assert (neighbor);

            if (neighbor->GetHasColony () && neighbor->GetColonyOwnerName () == district->GetColonyOwnerName ())
            {
                neighbors.Push (neighbor);
            }

            else if (district->GetLogisticsEvolutionPoints () >= configuration->GetMinimumLogisticsToUniteDistrictsToTradeAreaBySea ()
                     && neighbor->GetIsSea () && !neighbor->GetIsImpassable ())
            {
                Urho3D::PODVector <Urho3D::StringHash> neighborsOfNeighborsHashes = neighbor->GetNeighborsHashes ();
                for (int neighborOfNeighboarIndex = 0; neighborOfNeighboarIndex < neighborsOfNeighborsHashes.Size (); neighborOfNeighboarIndex++)
                {
                    District *neighborOfNeighbor = map->GetDistrictByHash (neighborsOfNeighborsHashes.At (neighborOfNeighboarIndex));
                    if (neighborOfNeighbor->GetHasColony () && neighborOfNeighbor->GetColonyOwnerName () == district->GetColonyOwnerName ())
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
                ProcessTradeAreaDistrict (map, neighbor, areaDistricts, unscannedList, configuration);
            }
        }
    }
}

void TradeProcessor::ProcessTradeAreaIncome (PlayersManager *playersManager, Map *map, InternalTradeArea *tradeArea,
                                             GameConfiguration *configuration, float updateDelay)
{
    Player *player = playersManager->GetPlayerByNameHash (Urho3D::StringHash (
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
        float internalTaxes = configuration->GetInternalTaxes ();
        Urho3D::SharedPtr <TradeDistrictProcessingInfo> result (tradeArea->ProcessTrade (map, updateDelay, true));
        float playersIncome = result->GetSoldTradeGoodsCost () * internalTaxes;
        player->SetGold (player->GetGold () + playersIncome);

        UnitsManager *unitsManager = node_->GetScene ()->GetChild ("units")->GetComponent <UnitsManager> ();
        assert (unitsManager);
        for (int index = 0; index < tradeArea->GetDistrictsHashesCount (); index++)
        {
            District *district = map->GetDistrictByHash (tradeArea->GetDistrictHashByIndex (index));
            assert (district);
            float unsoldGoodsCost = CalculateUnsoldGoodsCost (configuration, district);
            SendTrader (map, unitsManager, configuration, district, unsoldGoodsCost);
        }
    }
}

float TradeProcessor::CalculateUnsoldGoodsCost (GameConfiguration *configuration, District *district)
{
    float unsoldGoodsCost = 0.0f;
    Urho3D::VariantMap farmsProduction = district->GetLastTradeFarmsProduction ();
    unsoldGoodsCost += (farmsProduction [DISTRICT_PRODUCTION_AMOUNT_KEY].GetFloat () -
                        farmsProduction [DISTRICT_PRODUCTION_SELLED_KEY].GetFloat ()) *
            configuration->GetFarmsProductionExternalCost () *
            farmsProduction [DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY].GetFloat ();

    Urho3D::VariantMap minesProduction = district->GetLastTradeMinesProduction ();
    unsoldGoodsCost += (minesProduction [DISTRICT_PRODUCTION_AMOUNT_KEY].GetFloat () -
                        minesProduction [DISTRICT_PRODUCTION_SELLED_KEY].GetFloat ()) *
            configuration->GetMinesProductionExternalCost ()*
            minesProduction [DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY].GetFloat ();


    Urho3D::VariantMap industryProduction = district->GetLastTradeIndustryProduction ();
    unsoldGoodsCost += (industryProduction [DISTRICT_PRODUCTION_AMOUNT_KEY].GetFloat () -
                        industryProduction [DISTRICT_PRODUCTION_SELLED_KEY].GetFloat ()) *
            configuration->GetIndustryProductionExternalCost ()*
            industryProduction [DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY].GetFloat ();

    return unsoldGoodsCost;
}

void TradeProcessor::SendTrader (Map *map, UnitsManager *unitsManager, GameConfiguration *configuration,
                                 District *district, float goodsCost)
{
    Unit *unit = unitsManager->CreateUnit (UNIT_TRADERS, district->GetColonyOwnerName (), district->GetHash ());
    ((TradersUnit *) (unit))->SetTradeGoodsCost (goodsCost);

    District *nearestEuropeDistrict = map->GetDistrictByHash (
                configuration->GetHeuristicNearestWayToEuropeDistrict (map, district));
    map->FindPath (nearestEuropeDistrict->GetHash (), unit);
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

TradeProcessor::TradeProcessor (Urho3D::Context *context) : Urho3D::Component (context),
    tradeAreasUpdateDelay_ (10.0f),
    untilTradeAreasUpdate_ (0.0f)
{

}

TradeProcessor::~TradeProcessor ()
{
    tradeAreas_.Clear ();
}

void TradeProcessor::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <TradeProcessor> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Areas Update Delay", GetTradeAreasUpdateDelay, SetTradeAreasUpdateDelay, float, 10.0f, Urho3D::AM_DEFAULT);
}

void TradeProcessor::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
        untilTradeAreasUpdate_ -= timeStep;
        if (untilTradeAreasUpdate_ <= 0.0f)
        {
            UpdateTradeAreas (tradeAreasUpdateDelay_);
            untilTradeAreasUpdate_ = tradeAreasUpdateDelay_;
        }
    }
}

int TradeProcessor::GetTradeAreasCount () const
{
    return tradeAreas_.Size ();
}

InternalTradeArea *TradeProcessor::GetTradeAreaByIndex (int index) const
{
    assert (index < tradeAreas_.Size ());
    return tradeAreas_.At (index);
}

float TradeProcessor::GetTimeUntilTradeAreasUpdate () const
{
    return untilTradeAreasUpdate_;
}

float TradeProcessor::GetTradeAreasUpdateDelay () const
{
    return tradeAreasUpdateDelay_;
}

void TradeProcessor::SetTradeAreasUpdateDelay (float tradeAreasUpdateDelay)
{
    tradeAreasUpdateDelay_ = tradeAreasUpdateDelay;
    if (tradeAreasUpdateDelay_ > untilTradeAreasUpdate_)
    {
        untilTradeAreasUpdate_ = tradeAreasUpdateDelay;
    }
}
}
