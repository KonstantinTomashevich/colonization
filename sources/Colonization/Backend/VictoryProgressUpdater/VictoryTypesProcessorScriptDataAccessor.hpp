#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/AngelScript/ScriptFile.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea/InternalTradeArea.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>

namespace Colonization
{
class VictoryTypesProcessorScriptDataAccessor : public Urho3D::Object
{
URHO3D_OBJECT (VictoryTypesProcessorScriptDataAccessor, Object)
protected:
    Map *map_;
    UnitsManager *unitsManager_;
    TradeProcessor *tradeProcessor_;
    PlayerInfo *playerInfo_;
    GameConfiguration *configuration_;

public:
    VictoryTypesProcessorScriptDataAccessor (Urho3D::Context *context);
    virtual ~VictoryTypesProcessorScriptDataAccessor ();

    void Setup (Map *map, UnitsManager *unitsManager, TradeProcessor *tradeProcessor, GameConfiguration *configuration);
    const GameConfiguration *GetGameConfiguration () const;

    void SetPlayerInfo (PlayerInfo *playerInfo);
    const PlayerInfo *GetPlayerInfo () const;

    unsigned GetDistrictsCount () const;
    const District *GetDistrictByIndex (int index) const;

    unsigned GetUnitsCount() const;
    const Unit *GetUnitByIndex (int index) const;

    unsigned GetInternalTradeAreasCount ();
    const InternalTradeArea *GetInternalTradeAreaByIndex (int index) const;
};
}
