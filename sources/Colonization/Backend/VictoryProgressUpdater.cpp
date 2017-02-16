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
        Urho3D::VariantMap victoryInfo = playerInfo->GetProgressToVictoryOfTypeInfo ("ByPoints");
        victoryInfo ["Progress"] = 100.0f * (index * 1.0f * playersByPoints.Size ());
        playerInfo->SetProgressToVictoryOfTypeInfo ("ByPoints", victoryInfo);
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
    timeUntilGameEnd_ (99999.0f)
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
    }
}

float VictoryProgressUpdater::GetTimeUntilGameEnd ()
{
    return timeUntilGameEnd_;
}
}
