#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyRequestsUtils.hpp"


#include <Colonization/Backend/Diplomacy/DiplomacyProcessor.hpp>
#include <Colonization/Backend/Player/Player.hpp>
#include <Colonization/Backend/PlayersManager.hpp>

namespace Colonization
{
namespace DiplomacyRequestsUtils
{
void AddPlayerEnemy (Urho3D::Scene *scene, Urho3D::StringHash playerNameHash, Urho3D::StringHash newEnemyNameHash)
{
    PlayersManager *playersManager = scene->GetChild ("players")->GetComponent <PlayersManager> ();
    Player *player = playersManager->GetPlayerByNameHash (playerNameHash);
    assert (player);

    bool isEnemyExists = playersManager->GetPlayerByNameHash (newEnemyNameHash);
    assert (isEnemyExists);
    if (isEnemyExists)
    {
        player->AddEnemy (newEnemyNameHash);
    }
}

void RemovePlayerEnemy (Urho3D::Scene *scene, Urho3D::StringHash playerNameHash, Urho3D::StringHash oldEnemyNameHash)
{
    PlayersManager *playersManager = scene->GetChild ("players")->GetComponent <PlayersManager> ();
    Player *player = playersManager->GetPlayerByNameHash (playerNameHash);
    assert (player);

    bool isEnemyExists = playersManager->GetPlayerByNameHash (oldEnemyNameHash);
    assert (isEnemyExists);
    if (isEnemyExists)
    {
        player->RemoveEnemy (oldEnemyNameHash);
    }
}

DiplomacyWar *CreateWar (Urho3D::Scene *scene)
{
    DiplomacyProcessor *diplomacyProcessor = scene->GetChild ("diplomacy")->GetComponent <DiplomacyProcessor> ();
    return diplomacyProcessor->CreateWar ();
}

bool RemoveWarByHash (Urho3D::Scene *scene, Urho3D::StringHash warHash)
{
    DiplomacyProcessor *diplomacyProcessor = scene->GetChild ("diplomacy")->GetComponent <DiplomacyProcessor> ();
    return diplomacyProcessor->RemoveWarByHash (warHash);
}

void SendDiplomacyInfoMessageToAllPlayers (Urho3D::Scene *scene, Urho3D::StringHash infoType, Urho3D::VariantMap infoData)
{
    PlayersManager *playersManager = scene->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::PODVector <Player *> players = playersManager->GetAllPlayers ();

    MessagesHandler *messagesHandler = scene->GetComponent <MessagesHandler> ();
    messagesHandler->SendDiplomacyInfo (infoType, infoData, players);
}

void SendDiplomacyOfferMessageToAllPlayers (Urho3D::Scene *scene, Urho3D::StringHash offerType, unsigned offerDiplomacyRequestId, float autodeclineTime, Urho3D::VariantMap offerData)
{
    PlayersManager *playersManager = scene->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::PODVector <Player *> players = playersManager->GetAllPlayers ();

    MessagesHandler *messagesHandler = scene->GetComponent <MessagesHandler> ();
    messagesHandler->SendDiplomacyOffer (offerType, offerDiplomacyRequestId, autodeclineTime, offerData, players);
}

void SendDiplomacyInfoMessage (Urho3D::Scene *scene, Urho3D::StringHash infoType, Urho3D::VariantMap infoData, Urho3D::PODVector<Urho3D::StringHash> recieviers)
{
    PlayersManager *playersManager = scene->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::PODVector <Player *> players = playersManager->GetPlayersByNames (recieviers);

    MessagesHandler *messagesHandler = scene->GetComponent <MessagesHandler> ();
    messagesHandler->SendDiplomacyInfo (infoType, infoData, players);
}

void SendDiplomacyOfferMessage (Urho3D::Scene *scene, Urho3D::StringHash offerType, unsigned offerDiplomacyRequestId, float autodeclineTime, Urho3D::VariantMap offerData, Urho3D::PODVector <Urho3D::StringHash> recieviers)
{
    PlayersManager *playersManager = scene->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::PODVector <Player *> players = playersManager->GetPlayersByNames (recieviers);

    MessagesHandler *messagesHandler = scene->GetComponent <MessagesHandler> ();
    messagesHandler->SendDiplomacyOffer (offerType, offerDiplomacyRequestId, autodeclineTime, offerData, players);
}
}
}