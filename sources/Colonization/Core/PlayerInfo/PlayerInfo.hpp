#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
//@ASBindGen Class ObjectType=Ref
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
    explicit PlayerInfo (Urho3D::Context *context);
    virtual ~PlayerInfo ();
    static void RegisterObject (Urho3D::Context *context);

    //@ASBindGen Function OverrideName=get_name
    Urho3D::String GetName () const;
    //@ASBindGen Function OverrideName=set_name
    void SetName (const Urho3D::String &name);

    //@ASBindGen Function OverrideName=get_points
    float GetPoints () const;
    //@ASBindGen Function OverrideName=set_points
    void SetPoints (float points);

    //@ASBindGen Function OverrideName=get_color
    Urho3D::Color GetColor () const;
    //@ASBindGen Function OverrideName=set_color
    void SetColor (const Urho3D::Color &color);

    //@ASBindGen Function OverrideName=get_isReadyForStart
    bool IsReadyForStart () const;
    //@ASBindGen Function OverrideName=set_isReadyForStart
    void SetIsReadyForStart (bool isReadyForStart);

    //@ASBindGen Function OverrideName=get_enemiesCount
    int GetEnemiesCount () const;
    //@ASBindGen Function
    Urho3D::PODVector <Urho3D::StringHash> GetEnemies () const;
    //@ASBindGen Function
    Urho3D::StringHash GetEnemyByIndex (int index) const;
    //@ASBindGen Function
    bool IsAtWarWith (Urho3D::StringHash anotherPlayerNameHash) const;
    //@ASBindGen Function
    bool AddEnemy (Urho3D::StringHash anotherPlayerNameHash);
    //@ASBindGen Function
    bool RemoveEnemy (Urho3D::StringHash anotherPlayerNameHash);
    //@ASBindGen Function
    void RemoveAllEnemies ();

    Urho3D::VariantVector GetEnemiesAttribute () const;
    void SetEnemiesAttribute (const Urho3D::VariantVector &enemies);

    //@ASBindGen Function OverrideName=get_progressToVictory
    Urho3D::VariantMap GetProgressToVictory () const;
    //@ASBindGen Function OverrideName=set_progressToVictory
    void SetProgressToVictory (const Urho3D::VariantMap &progressToVictory);

    //@ASBindGen Function
    float GetProgressToVictoryOfType (Urho3D::StringHash victoryType) const;
    //@ASBindGen Function
    Urho3D::VariantMap GetProgressToVictoryOfTypeInfo (Urho3D::StringHash victoryType) const;
    //@ASBindGen Function
    void SetProgressToVictoryOfTypeInfo (Urho3D::StringHash victoryType, const Urho3D::VariantMap &progressToVictoryInfo);
};
}
