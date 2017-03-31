#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/AngelScript/ScriptFile.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>

namespace Colonization
{
// Maybe split to VictoryProgressUpdater, VictoryTypesProcessorScriptDataAccessor.
class VictoryProgressUpdater : public Urho3D::Component
{
URHO3D_OBJECT (VictoryProgressUpdater, Component)
protected:
    Urho3D::SharedPtr <Urho3D::ScriptFile> victoryTypesProcessor_;
    float timeUntilGameEnd_;
    bool isAnyoneWon_;
    Urho3D::String winnerName_;
    Urho3D::String victoryType_;
    Urho3D::String victoryInfo_;

    void UpdateVictoryByPointsProgresses ();
    void SetWinnerFromVictoryByPoints ();
    void CheckForAnyVictory ();
    void ProcessScriptedVictoryTypes (float timeStep);
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    VictoryProgressUpdater (Urho3D::Context *context);
    virtual ~VictoryProgressUpdater ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    float GetTimeUntilGameEnd ();
    bool IsAnyoneWon ();
    Urho3D::String GetWinnerName ();
    Urho3D::String GetVictoryType ();
    Urho3D::String GetVictoryInfo ();
};

class VictoryTypesProcessorScriptDataAccessor : public Urho3D::Object
{
URHO3D_OBJECT (VictoryTypesProcessorScriptDataAccessor, Object)
protected:
    Map *map_;
    UnitsManager *unitsManager_;
    TradeProcessor *tradeProcessor_;
    PlayerInfo *playerInfo_;

public:
    VictoryTypesProcessorScriptDataAccessor (Urho3D::Context *context);
    virtual ~VictoryTypesProcessorScriptDataAccessor ();

    void Setup (Map *map, UnitsManager *unitsManager, TradeProcessor *tradeProcessor);
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
