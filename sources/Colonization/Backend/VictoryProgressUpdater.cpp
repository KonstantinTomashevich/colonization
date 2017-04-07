#include <Colonization/BuildConfiguration.hpp>
#include "VictoryProgressUpdater.hpp"

#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Scene.h>

#include <Urho3D/IO/Log.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea/InternalTradeArea.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/Player/PlayerComparator.hpp>

#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void VictoryProgressUpdater::UpdateVictoryByPointsProgresses ()
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::Vector <Player *> players = playersManager->GetAllPlayers ();
    Urho3D::Sort <Player *, PlayerComparator> (players.Begin (), players.End (), PlayerComparators::HigherPoints);

    for (int index = 0; index < players.Size (); index++)
    {
        Player *player = players.At (index);
        PlayerInfo *playerInfo = playersManager->GetPlayerInfoByPointer (player);
        assert (playerInfo);
        Urho3D::VariantMap victoryInfo = playerInfo->GetProgressToVictoryOfTypeInfo (VICTORY_TYPE_BY_POINTS);
        victoryInfo [PLAYER_INFO_VICTORY_TYPE_NAME_KEY] = VICTORY_TYPE_BY_POINTS_NAME;
        victoryInfo [PLAYER_INFO_VICTORY_TYPE_INFO_KEY] = VICTORY_TYPE_BY_POINTS_INFO;
        victoryInfo [PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY] = 99.0f * ((players.Size () - index) / players.Size ());
        playerInfo->SetProgressToVictoryOfTypeInfo (VICTORY_TYPE_BY_POINTS, victoryInfo);
    }
}

void VictoryProgressUpdater::SetWinnerFromVictoryByPoints ()
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::Vector <Player *>  players = playersManager->GetAllPlayers ();
    int currentBiggestIndex = -1;
    float currentBiggestPoints = -1.0f;

    for (int index = 0; index < players.Size (); index++)
    {
        Player *player = players.At (index);
        if (player)
        {
            float points = player->GetPoints ();
            if (points > currentBiggestPoints)
            {
                currentBiggestIndex = index;
                currentBiggestPoints = points;
            }
        }
    }

    assert (currentBiggestIndex >= 0);
    assert (currentBiggestPoints >= 0.0f);
    isAnyoneWon_ = true;
    winnerName_ = players.At (currentBiggestIndex)->GetName ();
    victoryType_ = VICTORY_TYPE_BY_POINTS_NAME;

    Urho3D::String rawVictoryInfo = VICTORY_TYPE_BY_POINTS_INFO;
    rawVictoryInfo.Replace ("${PlayerName}", winnerName_);
    victoryInfo_ = rawVictoryInfo;
}

void VictoryProgressUpdater::CheckForAnyVictory ()
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::Vector <Player *>  players = playersManager->GetAllPlayers ();

    int index = 0;
    while (index < players.Size () && !isAnyoneWon_)
    {
        Player *player = players.At (index);
        if (player)
        {
            PlayerInfo *playerInfo = playersManager->GetPlayerInfoByPointer (player);
            assert (playerInfo);

            Urho3D::VariantMap progressToVictory = playerInfo->GetProgressToVictory ();
            int victoryTypeIndex = 0;
            while (victoryTypeIndex < progressToVictory.Size () && !isAnyoneWon_)
            {
                Urho3D::Variant value = progressToVictory.Values ().At (victoryTypeIndex);
                if (value.GetType () == Urho3D::VAR_VARIANTMAP)
                {
                    float progress = value.GetVariantMap () [PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY]->GetFloat ();
                    if (progress >= 100.0f)
                    {
                        isAnyoneWon_ = true;
                        winnerName_ = player->GetName ();
                        victoryType_ = value.GetVariantMap () [PLAYER_INFO_VICTORY_TYPE_NAME_KEY]->GetString ();

                        Urho3D::String rawVictoryInfo = value.GetVariantMap () [PLAYER_INFO_VICTORY_TYPE_INFO_KEY]->GetString ();
                        rawVictoryInfo.Replace ("${PlayerName}", winnerName_);
                        victoryInfo_ = rawVictoryInfo;
                    }
                }
                victoryTypeIndex++;
            }
        }
        index++;
    }
}

void VictoryProgressUpdater::ProcessScriptedVictoryTypes (float timeStep)
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    UnitsManager *unitsManager = node_->GetScene ()->GetChild ("units")->GetComponent <UnitsManager> ();
    TradeProcessor *tradeProcessor = node_->GetScene ()->GetComponent <TradeProcessor> ();

    Urho3D::SharedPtr <VictoryTypesProcessorScriptDataAccessor> dataAccessor (new VictoryTypesProcessorScriptDataAccessor (context_));
    dataAccessor->Setup (map, unitsManager, tradeProcessor);

    Urho3D::VariantVector executionParameters;
    executionParameters.Push (Urho3D::Variant (timeStep));
    executionParameters.Push (Urho3D::Variant (dataAccessor.Get ()));
    executionParameters.Push (Urho3D::Variant (Urho3D::VariantMap ()));

    for (int index = 0; index < playersManager->GetPlayersCount (); index++)
    {
        Player *player = playersManager->GetPlayerByIndex (index);
        if (player)
        {
            PlayerInfo *playerInfo = playersManager->GetPlayerInfoByPointer (player);
            assert (playerInfo);
            dataAccessor->SetPlayerInfo (playerInfo);
            executionParameters.At (2) = Urho3D::Variant (Urho3D::VariantMap ());
            victoryTypesProcessor_->Execute (
                        "void Process (float, const VictoryTypesProcessorScriptDataAccessor @+, VariantMap &out)",
                        executionParameters);

            Urho3D::VariantMap output = executionParameters.At (2).GetVariantMap ();
            for (Urho3D::VariantMap::Iterator iterator = output.Begin ();
                 iterator != output.End (); iterator++)
            {
                Urho3D::Variant value = iterator->second_;
                if (value.GetType () == Urho3D::VAR_VARIANTMAP)
                {
                    playerInfo->SetProgressToVictoryOfTypeInfo (iterator->first_,
                                                                value.GetVariantMap ());
                }
            }

            NetworkUpdateCounter *counter = playerInfo->GetNode ()->GetComponent <NetworkUpdateCounter> ();
            if (!counter)
            {
                counter = CreateNetworkUpdateCounterForComponent (playerInfo);
            }
            counter->AddUpdatePoints (20.0f * timeStep);
        }
    }
}

void VictoryProgressUpdater::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (VictoryProgressUpdater, Update));

    if (scene)
    {
        GameConfiguration *configuration = scene->GetComponent <GameConfiguration> ();
        timeUntilGameEnd_ = configuration->GetMaximumGameDuration ();

        Urho3D::ResourceCache *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
        victoryTypesProcessor_ = resourceCache->GetResource <Urho3D::ScriptFile> (
                    configuration->GetVictoryTypesProcessorScriptPath ());
        assert (victoryTypesProcessor_.NotNull ());
        assert (victoryTypesProcessor_->IsCompiled ());
    }
}

VictoryProgressUpdater::VictoryProgressUpdater (Urho3D::Context *context) : Urho3D::Component (context),
    victoryTypesProcessor_ (),
    timeUntilGameEnd_ (99999.0f),
    isAnyoneWon_ (false),
    winnerName_ (Urho3D::String::EMPTY),
    victoryInfo_ (Urho3D::String::EMPTY)
{

}

VictoryProgressUpdater::~VictoryProgressUpdater ()
{

}

void VictoryProgressUpdater::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <VictoryProgressUpdater> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void VictoryProgressUpdater::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
        timeUntilGameEnd_ -= timeStep;
        UpdateVictoryByPointsProgresses ();
        ProcessScriptedVictoryTypes (timeStep);
        CheckForAnyVictory ();
        if (timeUntilGameEnd_ <= 0.0f)
        {
            SetWinnerFromVictoryByPoints ();
        }
    }
}

float VictoryProgressUpdater::GetTimeUntilGameEnd ()
{
    return timeUntilGameEnd_;
}

bool VictoryProgressUpdater::IsAnyoneWon ()
{
    return isAnyoneWon_;
}

Urho3D::String VictoryProgressUpdater::GetWinnerName ()
{
    return winnerName_;
}

Urho3D::String VictoryProgressUpdater::GetVictoryType ()
{
    return victoryType_;
}

Urho3D::String VictoryProgressUpdater::GetVictoryInfo ()
{
    return victoryInfo_;
}

VictoryTypesProcessorScriptDataAccessor::VictoryTypesProcessorScriptDataAccessor (Urho3D::Context *context) : Urho3D::Object (context),
    map_ (0),
    unitsManager_ (0),
    tradeProcessor_ (0),
    playerInfo_ (0)
{

}

VictoryTypesProcessorScriptDataAccessor::~VictoryTypesProcessorScriptDataAccessor ()
{

}

void VictoryTypesProcessorScriptDataAccessor::Setup (Map *map, UnitsManager *unitsManager, TradeProcessor *tradeProcessor)
{
    assert (map);
    assert (unitsManager);
    assert (tradeProcessor);

    map_ = map;
    unitsManager_ = unitsManager;
    tradeProcessor_ = tradeProcessor;
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
