#include <Colonization/BuildConfiguration.hpp>
#include "VictoryTypesProcessorScriptDataAccessor.hpp"
#include <Urho3D/Core/Context.h>

namespace Colonization
{
VictoryTypesProcessorScriptDataAccessor::VictoryTypesProcessorScriptDataAccessor (Urho3D::Context *context) : Urho3D::Object (context),
    map_ (0),
    unitsManager_ (0),
    tradeProcessor_ (0),
    playerInfo_ (0),
    configuration_ (0)
{

}

VictoryTypesProcessorScriptDataAccessor::~VictoryTypesProcessorScriptDataAccessor ()
{

}

void VictoryTypesProcessorScriptDataAccessor::Setup (Map *map, UnitsManager *unitsManager, TradeProcessor *tradeProcessor, Colonization::GameConfiguration *configuration)
{
    assert (map);
    assert (unitsManager);
    assert (tradeProcessor);
    assert (configuration);

    map_ = map;
    unitsManager_ = unitsManager;
    tradeProcessor_ = tradeProcessor;
    configuration_ = configuration;
}

const GameConfiguration *VictoryTypesProcessorScriptDataAccessor::GetGameConfiguration () const
{
    return configuration_;
}

void VictoryTypesProcessorScriptDataAccessor::SetPlayerInfo (PlayerInfo *playerInfo)
{
    assert (playerInfo);
    playerInfo_ = playerInfo;
}

const PlayerInfo *VictoryTypesProcessorScriptDataAccessor::GetPlayerInfo () const
{
    assert (playerInfo_);
    return playerInfo_;
}

unsigned VictoryTypesProcessorScriptDataAccessor::GetDistrictsCount () const
{
    assert (map_);
    return map_->GetDistrictsCount ();
}

const District *VictoryTypesProcessorScriptDataAccessor::GetDistrictByIndex (int index) const
{
    assert (map_);
    return map_->GetDistrictByIndex (index);
}

unsigned VictoryTypesProcessorScriptDataAccessor::GetUnitsCount () const
{
    assert (unitsManager_);
    return unitsManager_->GetUnitsCount ();
}

const Unit *VictoryTypesProcessorScriptDataAccessor::GetUnitByIndex (int index) const
{
    assert (unitsManager_);
    return unitsManager_->GetUnitByIndex (index);
}

unsigned VictoryTypesProcessorScriptDataAccessor::GetInternalTradeAreasCount ()
{
    assert (tradeProcessor_);
    return tradeProcessor_->GetTradeAreasCount ();
}

const InternalTradeArea *VictoryTypesProcessorScriptDataAccessor::GetInternalTradeAreaByIndex (int index) const
{
    assert (tradeProcessor_);
    return tradeProcessor_->GetTradeAreaByIndex (index);
}
}
