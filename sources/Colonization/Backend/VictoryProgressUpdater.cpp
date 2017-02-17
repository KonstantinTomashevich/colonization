#include <Colonization/BuildConfiguration.hpp>
#include "VictoryProgressUpdater.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/IO/Log.h>

#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
void VictoryProgressUpdater::UpdateVictoryByPointsProgresses ()
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::HashMap <Urho3D::StringHash, Player*> playersByPoints;

    for (int index = 0; index < playersManager->GetPlayersCount (); index++)
    {
        Player *player = playersManager->GetPlayerByIndex (index);
        if (player)
        {
            unsigned pointsInt = std::floor (player->GetPoints () * 1000.0f);
            while (playersByPoints [Urho3D::StringHash (pointsInt)])
            {
                pointsInt++;
            }
            playersByPoints [Urho3D::StringHash (pointsInt)] = player;
        }
    }

    playersByPoints.Sort ();
    // Because bigger points is at end of map, start from end of map.
    for (int index = playersByPoints.Size () - 1; index >= 0; index--)
    {
        Player *player = playersByPoints.Values ().At (index);
        PlayerInfo *playerInfo = playersManager->GetPlayerInfoByPointer (player);
        assert (playerInfo);
        Urho3D::VariantMap victoryInfo = playerInfo->GetProgressToVictoryOfTypeInfo (VICTORY_TYPE_BY_POINTS);
        victoryInfo [PLAYER_INFO_VICTORY_TYPE_NAME_KEY] = VICTORY_TYPE_BY_POINTS_NAME;
        victoryInfo [PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY] = 99.0f * (index * 1.0f / playersByPoints.Size ());
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
                    }
                }
                victoryTypeIndex++;
            }
        }
        index++;
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
    }
}

VictoryProgressUpdater::VictoryProgressUpdater (Urho3D::Context *context) : Urho3D::Component (context),
    timeUntilGameEnd_ (99999.0f),
    isAnyoneWon_ (false),
    winnerName_ (Urho3D::String::EMPTY)
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
}
