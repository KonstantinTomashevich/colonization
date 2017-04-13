#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyActionsExecutor.hpp"

#include <Colonization/Backend/Diplomacy/DiplomacyProcessor.hpp>
#include <Colonization/Backend/Player/Player.hpp>
#include <Colonization/Backend/PlayersManager.hpp>

namespace Colonization
{
DiplomacyActionsExecutor::DiplomacyActionsExecutor (Urho3D::Scene *scene) :
    scene_ (scene)
{

}

DiplomacyActionsExecutor::~DiplomacyActionsExecutor ()
{

}

void DiplomacyActionsExecutor::AddPlayerEnemy (Urho3D::StringHash playerNameHash, Urho3D::StringHash newEnemyNameHash)
{
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    Player *player = playersManager->GetPlayerByNameHash (playerNameHash);
    assert (player);

    bool isEnemyExists = playersManager->GetPlayerByNameHash (newEnemyNameHash);
    assert (isEnemyExists);
    if (isEnemyExists)
    {
        player->AddEnemy (newEnemyNameHash);
    }
}

void DiplomacyActionsExecutor::RemovePlayerEnemy (Urho3D::StringHash playerNameHash, Urho3D::StringHash oldEnemyNameHash)
{
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    Player *player = playersManager->GetPlayerByNameHash (playerNameHash);
    assert (player);

    bool isEnemyExists = playersManager->GetPlayerByNameHash (oldEnemyNameHash);
    assert (isEnemyExists);
    if (isEnemyExists)
    {
        player->RemoveEnemy (oldEnemyNameHash);
    }
}

DiplomacyWar *DiplomacyActionsExecutor::CreateWar ()
{
    DiplomacyProcessor *diplomacyProcessor = scene_->GetChild ("diplomacy")->GetComponent <DiplomacyProcessor> ();
    return diplomacyProcessor->CreateWar ();
}
}

