#pragma once
#include <Urho3D/Scene/Scene.h>

namespace Colonization
{
class DiplomacyWar;
class DiplomacyActionsExecutor
{
protected:
    Urho3D::Scene *scene_;

public:
    DiplomacyActionsExecutor (Urho3D::Scene *scene);
    virtual ~DiplomacyActionsExecutor ();

    void AddPlayerEnemy (Urho3D::StringHash playerNameHash, Urho3D::StringHash newEnemyNameHash);
    void RemovePlayerEnemy (Urho3D::StringHash playerNameHash, Urho3D::StringHash oldEnemyNameHash);
    DiplomacyWar *CreateWar ();
};
}
