#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
class PlayerInfo : public Urho3D::Component
{
URHO3D_OBJECT (PlayerInfo, Component)
protected:
    Urho3D::String name_;
    float points_;
    Urho3D::Color color_;
    bool isReadyForStart_;
    Urho3D::PODVector <Urho3D::StringHash> enemies_;
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

    int GetEnemiesCount () const;
    Urho3D::PODVector <Urho3D::StringHash> GetEnemies () const;
    Urho3D::StringHash GetEnemyByIndex (int index) const;
    bool IsAtWarWith (Urho3D::StringHash anotherPlayerNameHash) const;
    bool AddEnemy (Urho3D::StringHash anotherPlayerNameHash);
    bool RemoveEnemy (Urho3D::StringHash anotherPlayerNameHash);
    void RemoveAllEnemies ();

    Urho3D::VariantVector GetEnemiesAttribute () const;
    void SetEnemiesAttribute (const Urho3D::VariantVector &enemies);

    Urho3D::VariantMap GetProgressToVictory () const;
    void SetProgressToVictory (const Urho3D::VariantMap &progressToVictory);

    float GetProgressToVictoryOfType (Urho3D::StringHash victoryType) const;
    Urho3D::VariantMap GetProgressToVictoryOfTypeInfo (Urho3D::StringHash victoryType) const;
    void SetProgressToVictoryOfTypeInfo (Urho3D::StringHash victoryType, const Urho3D::VariantMap &progressToVictoryInfo);
};
}
