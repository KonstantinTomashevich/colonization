#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea/InternalTradeArea.hpp>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>

namespace Colonization
{
class GameConfiguration;
//@ASBindGen Class ObjectType=Ref
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
    explicit VictoryTypesProcessorScriptDataAccessor (Urho3D::Context *context);
    virtual ~VictoryTypesProcessorScriptDataAccessor ();

    void Setup (Map *map, UnitsManager *unitsManager, TradeProcessor *tradeProcessor, GameConfiguration *configuration);
    //@ASBindGen Function AddRef_arg-1 OverrideName=get_gameConfiguration
    const GameConfiguration *GetGameConfiguration () const;

    void SetPlayerInfo (PlayerInfo *playerInfo);
    //@ASBindGen Function AddRef_arg-1 OverrideName=get_playerInfo
    const PlayerInfo *GetPlayerInfo () const;

    //@ASBindGen Function OverrideName=get_districtsCount
    unsigned GetDistrictsCount () const;
    //@ASBindGen Function AddRef_arg-1
    const District *GetDistrictByIndex (int index) const;

    //@ASBindGen Function OverrideName=get_unitsCount
    unsigned GetUnitsCount() const;
    //@ASBindGen Function AddRef_arg-1
    const Unit *GetUnitByIndex (int index) const;

    //@ASBindGen Function OverrideName=get_internalTradeAreasCount
    unsigned GetInternalTradeAreasCount ();
    //@ASBindGen Function AddRef_arg-1
    const InternalTradeArea *GetInternalTradeAreaByIndex (int index) const;
};
}
