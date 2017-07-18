#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/AngelScript/ScriptFile.h>

namespace Colonization
{
class VictoryProgressUpdater : public Urho3D::Component
{
URHO3D_OBJECT (VictoryProgressUpdater, Component)
public:
    explicit VictoryProgressUpdater (Urho3D::Context *context);
    virtual ~VictoryProgressUpdater ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    float GetTimeUntilGameEnd () const;
    bool IsAnyoneWon () const;
    Urho3D::String GetWinnerName () const;
    Urho3D::String GetVictoryType () const;
    Urho3D::String GetVictoryInfo () const;

protected:
    virtual void OnSceneSet (Urho3D::Scene* scene);

private:
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
};
}
