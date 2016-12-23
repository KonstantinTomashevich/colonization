#include <Colonization/BuildConfiguration.hpp>
#include "TradeProcessor.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Backend/PlayersManager.hpp>

namespace Colonization
{
void TradeProcessor::UpdateTradeAreas (float updateDelay)
{
    ClearTradeAreas ();
    Map *map = (Map *) context_->GetGlobalVar ("Map").GetPtr ();
    Urho3D::PODVector <District *> toScan;

    for (int index = 0; index < map->GetDistrictsCount (); index++)
    {
        District *district = map->GetDistrictByIndex (index);
        if (district->hasColony_)
            toScan.Push (district);
    }

    while (!toScan.Empty ())
        tradeAreas_.Push (CreateTradeArea (toScan.At (0), toScan));

    PlayersManager *playersManager = (PlayersManager *) context_->GetGlobalVar ("PlayersManager").GetPtr ();
    for (int index = 0; index < tradeAreas_.Size (); index++)
        ProcessTradeAreaIncome (playersManager, map, tradeAreas_.At (index), updateDelay);
}

InternalTradeArea *TradeProcessor::CreateTradeArea (District *start, Urho3D::PODVector <District *> &unscannedList)
{
    Urho3D::PODVector <District *> areaDistricts;
    ProcessTradeAreaDistrict (start, areaDistricts, unscannedList);

    InternalTradeArea *tradeArea = new InternalTradeArea (context_);
    for (int index = 0; index < areaDistricts.Size (); index++)
        tradeArea->AddDistrictHash (areaDistricts.At (index)->GetHash ());
    return tradeArea;
}

void TradeProcessor::ProcessTradeAreaDistrict (District *district, Urho3D::PODVector <District *> &areaDistricts, Urho3D::PODVector <District *> &unscannedList)
{
    areaDistricts.Push (district);
    unscannedList.Remove (district);

    Urho3D::PODVector <District *> neighbors;
    if (district->logisticsEvolutionPoints_ > 4.0f)
        for (int index = 0; index < district->neighbors_.Size (); index++)
        {
            District *neighbor = district->neighbors_.At (index);
            if (neighbor->hasColony_ && neighbor->colonyOwnerName_ == district->colonyOwnerName_)
                neighbors.Push (neighbor);

            else if (district->logisticsEvolutionPoints_ > 6.0f && neighbor->isSea_ && !neighbor->isImpassable_)
            {
                for (int neighborOfNeighboarIndex = 0; neighborOfNeighboarIndex < neighbor->neighbors_.Size (); index++)
                {
                    District *neighborOfNeighbor = neighbor->neighbors_.At (neighborOfNeighboarIndex);
                    if (neighborOfNeighbor->hasColony_ && neighborOfNeighbor->colonyOwnerName_ == district->colonyOwnerName_)
                        neighbors.Push (neighborOfNeighbor);
                }
            }
        }

    if (!neighbors.Empty ())
        for (int index = 0; index < neighbors.Size (); index++)
        {
            District *neighbor = neighbors.At (index);
            if (unscannedList.Contains (neighbor))
                ProcessTradeAreaDistrict (neighbor, areaDistricts, unscannedList);
        }
}

void TradeProcessor::ProcessTradeAreaIncome (PlayersManager *playersManager, Map *map, InternalTradeArea *tradeArea, float updateDelay)
{
    Player *player = playersManager->GetPlayer (Urho3D::StringHash (
                                                    map->GetDistrictByHash (tradeArea->GetDistrictHashByIndex (0))->
                                                    colonyOwnerName_);
    float internalTaxes = context_->GetGlobalVar ("internalTaxes").GetFloat ();
    TradeDistrictProcessingInfo result = tradeArea->ProcessTrade (map);
    float playersIncome = result.soldTradeGoodsCost_ * result.logisticsBonus_ * result.defenseBonus_ * internalTaxes;
    player->SetGold (player->GetGold () + playersIncome);
}

void TradeProcessor::ClearTradeAreas ()
{
    while (!tradeAreas_.Empty ())
    {
        InternalTradeArea *area = tradeAreas_.At (0);
        tradeAreas_.Remove (area);
        delete area;
    }
}

TradeProcessor::TradeProcessor (Urho3D::Context *context) : Urho3D::Object (context)
{
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (TradeProcessor, Update));
    beforeTradeAreasUpdate_ = 0.0001f;
}

TradeProcessor::~TradeProcessor ()
{
    ClearTradeAreas ();
}

void TradeProcessor::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
    // TODO: Maybe rename all "before*" to "until*"? And do it in AngelScript too.
    beforeTradeAreasUpdate_ -= timeStep;
    if (beforeTradeAreasUpdate_ <= 0.0f)
    {
        UpdateTradeAreas (5.0f);
        beforeTradeAreasUpdate_ = 5.0f;
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
