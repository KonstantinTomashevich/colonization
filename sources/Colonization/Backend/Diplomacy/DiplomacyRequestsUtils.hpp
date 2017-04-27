#pragma once
#include <Urho3D/Scene/Scene.h>

namespace Colonization
{
class DiplomacyWar;
class Player;
namespace DiplomacyRequestsUtils
{
void UpdatePlayerEnemies (Urho3D::Scene *scene, Urho3D::StringHash playerNameHash);
DiplomacyWar *CreateWar (Urho3D::Scene *scene);
bool RemoveWarByHash (Urho3D::Scene *scene, Urho3D::StringHash warHash);

void SendDiplomacyInfoMessageToAllPlayers (Urho3D::Scene *scene, Urho3D::StringHash infoType, Urho3D::VariantMap infoData);
void SendDiplomacyOfferMessageToAllPlayers (Urho3D::Scene *scene, Urho3D::StringHash offerType, unsigned offerDiplomacyRequestId, float autodeclineTime, Urho3D::VariantMap offerData);

void SendDiplomacyInfoMessage (Urho3D::Scene *scene, Urho3D::StringHash infoType, Urho3D::VariantMap infoData, Urho3D::PODVector <Urho3D::StringHash> recieviers);
void SendDiplomacyOfferMessage (Urho3D::Scene *scene, Urho3D::StringHash offerType, unsigned offerDiplomacyRequestId, float autodeclineTime, Urho3D::VariantMap offerData, Urho3D::PODVector <Urho3D::StringHash> recieviers);
}
}
