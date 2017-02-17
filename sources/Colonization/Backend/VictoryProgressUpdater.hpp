#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
class VictoryProgressUpdater : public Urho3D::Component
{
URHO3D_OBJECT (VictoryProgressUpdater, Component)
protected:
    float timeUntilGameEnd_;
    bool isAnyoneWin_;
    Urho3D::String winnerName_;
    Urho3D::String victoryType_;

    void UpdateVictoryByPointsProgresses ();
    void SetWinnerFromVictoryByPoints ();
    void CheckForAnyVictory ();
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    VictoryProgressUpdater (Urho3D::Context *context);
    virtual ~VictoryProgressUpdater ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    float GetTimeUntilGameEnd ();
    bool IsAnyoneWin ();
    Urho3D::String GetWinnerName ();
    Urho3D::String GetVictoryType ();
};
}
