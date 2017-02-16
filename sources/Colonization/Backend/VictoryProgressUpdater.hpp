#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
class VictoryProgressUpdater : public Urho3D::Component
{
URHO3D_OBJECT (VictoryProgressUpdater, Component)
protected:
    float timeUntilGameEnd_;
    void UpdateVictoryByPointsProgresses ();
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    VictoryProgressUpdater (Urho3D::Context *context);
    virtual ~VictoryProgressUpdater ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    float GetTimeUntilGameEnd ();
};
}
