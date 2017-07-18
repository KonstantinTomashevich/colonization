#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Unit/UnitType.hpp>

namespace Colonization
{
class GameConfiguration;
class District;
class UnitsManager;
class PlayerInfo;

//@ASBindGen Class ObjectType=Ref
class Unit : public Urho3D::Component
{
URHO3D_OBJECT (Unit, Component)
public:
    explicit Unit (Urho3D::Context *context);
    virtual ~Unit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest) = 0;

    void UpdateHash (const UnitsManager *owner);
    //@ASBindGen Function OverrideName=get_hash
    Urho3D::StringHash GetHash () const;
    //@ASBindGen Function OverrideName=set_hash
    void SetHash (const Urho3D::StringHash &hash);

    //@ASBindGen Function OverrideName=get_unitType
    UnitType GetUnitType () const;
    //@ASBindGen Function OverrideName=set_unitType
    void SetUnitType (UnitType unitType);

    //@ASBindGen Function OverrideName=get_ownerPlayerName
    Urho3D::String GetOwnerPlayerName () const;
    //@ASBindGen Function OverrideName=set_ownerPlayerName
    void SetOwnerPlayerName (const Urho3D::String &ownerPlayerName);

    //@ASBindGen Function OverrideName=get_positionHash
    Urho3D::StringHash GetPositionHash () const;
    //@ASBindGen Function OverrideName=set_positionHash
    void SetPositionHash (const Urho3D::StringHash &positionHash);

    //@ASBindGen Function OverrideName=get_way
    Urho3D::PODVector <Urho3D::StringHash> GetWay () const;
    //@ASBindGen Function OverrideName=set_way
    void SetWay (Urho3D::PODVector <Urho3D::StringHash> way);
    Urho3D::VariantVector GetWayAttribute () const;
    void SetWayAttribute (const Urho3D::VariantVector &way);

    //@ASBindGen Function OverrideName=get_wayToNextDistrictProgressInPercents
    float GetWayToNextDistrictProgressInPercents () const;
    //@ASBindGen Function OverrideName=set_wayToNextDistrictProgressInPercents
    void SetWayToNextDistrictProgressInPercents (float wayToNextDistrictProgressInPercents);

    //@ASBindGen Function OverrideName=get_isInBattle
    bool GetIsInBattle () const;
    //@ASBindGen Function OverrideName=set_isInBattle
    void SetIsInBattle (bool isInBattle);

    //@ASBindGen Function OverrideName=get_battleHash
    Urho3D::StringHash GetBattleHash () const;
    //@ASBindGen Function OverrideName=set_battleHash
    void SetBattleHash (const Urho3D::StringHash &battleHash);

    //@ASBindGen Function
    virtual bool IsCanGoTo (const District *district, const Map *map, Urho3D::StringHash imaginePosition = Urho3D::StringHash::ZERO) const;
    //@ASBindGen Function
    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const = 0;
    //@ASBindGen Function
    /// Returns false if unit destroyed.
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage) = 0;
    //@ASBindGen Function OverrideName=get_alive
    virtual bool IsAlive () const = 0;
    //@ASBindGen Function
    virtual Urho3D::String GetUnitTypeTag () const = 0;

protected:
    UnitType unitType_;

private:
    Urho3D::StringHash hash_;
    // TODO: Is it necessary to keep unit owner name instead of unit owner name hash?
    Urho3D::String ownerPlayerName_;
    Urho3D::StringHash positionHash_;
    Urho3D::PODVector <Urho3D::StringHash> way_;
    float wayToNextDistrictProgressInPercents_;
    bool isInBattle_;
    Urho3D::StringHash battleHash_;
};
}
