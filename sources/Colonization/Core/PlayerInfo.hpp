#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
const Urho3D::StringHash PLAYER_INFO_VICTORY_TYPE_NAME_KEY ("Name");
const Urho3D::StringHash PLAYER_INFO_VICTORY_TYPE_INFO_KEY ("Info");
const Urho3D::StringHash PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY ("Progress");
const Urho3D::StringHash VICTORY_TYPE_BY_POINTS ("ByPoints");
const Urho3D::String VICTORY_TYPE_BY_POINTS_NAME ("by points");
const Urho3D::String VICTORY_TYPE_BY_POINTS_INFO ("Maximum game time elapsed. ${PlayerName} has biggest number of points.");

class PlayerInfo : public Urho3D::Component
{
URHO3D_OBJECT (PlayerInfo, Component)
protected:
    Urho3D::String name_;
    float points_;
    Urho3D::Color color_;
    bool isReadyForStart_;
    /// Progresses to victories. Key is victory type name, value is VariantMap.
    /// Each value has progress indicator named "progress". It is float from 0.0 to 100.0.
    /// Each value has angel script function name, which parses this value and returns text for tooltip.
    Urho3D::VariantMap progressToVictory_;

public:
    PlayerInfo (Urho3D::Context *context);
    virtual ~PlayerInfo ();
    static void RegisterObject (Urho3D::Context *context);

    Urho3D::String GetName () const;
    void SetName (const Urho3D::String &name);

    float GetPoints () const;
    void SetPoints (float points);

    Urho3D::Color GetColor () const;
    void SetColor (const Urho3D::Color &color);

    bool IsReadyForStart () const;
    void SetIsReadyForStart (bool isReadyForStart);

    Urho3D::VariantMap GetProgressToVictory () const;
    void SetProgressToVictory (const Urho3D::VariantMap &progressToVictory);

    float GetProgressToVictoryOfType (Urho3D::StringHash victoryType) const;
    Urho3D::VariantMap GetProgressToVictoryOfTypeInfo (Urho3D::StringHash victoryType) const;
    void SetProgressToVictoryOfTypeInfo (Urho3D::StringHash victoryType, const Urho3D::VariantMap &progressToVictoryInfo);
};
}
