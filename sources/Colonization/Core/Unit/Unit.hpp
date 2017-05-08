#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Unit/UnitType.hpp>

namespace Colonization
{
class UnitsManager;
class Unit : public Urho3D::Component
{
URHO3D_OBJECT (Unit, Component)
protected:
    Urho3D::StringHash hash_;
    Urho3D::String ownerPlayerName_;
    UnitType unitType_;
    Urho3D::StringHash positionHash_;
    Urho3D::PODVector <Urho3D::StringHash> way_;
    float wayToNextDistrictProgressInPercents_;

public:
    Unit (Urho3D::Context *context);
    virtual ~Unit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest) = 0;

    bool IsCanGoTo (const District *district, const Map *map, Urho3D::StringHash imaginePosition = Urho3D::StringHash::ZERO) const;
    void UpdateHash (const UnitsManager *owner);
    Urho3D::StringHash GetHash () const;
    void SetHash (const Urho3D::StringHash &hash);

    UnitType GetUnitType () const;
    void SetUnitType (UnitType unitType);

    Urho3D::String GetOwnerPlayerName () const;
    void SetOwnerPlayerName (const Urho3D::String &ownerPlayerName);

    Urho3D::StringHash GetPositionHash () const;
    void SetPositionHash (const Urho3D::StringHash &positionHash);

    Urho3D::PODVector <Urho3D::StringHash> GetWay () const;
    void SetWay (Urho3D::PODVector <Urho3D::StringHash> way);
    Urho3D::VariantVector GetWayAttribute () const;
    void SetWayAttribute (const Urho3D::VariantVector &way);

    float GetWayToNextDistrictProgressInPercents () const;
    void SetWayToNextDistrictProgressInPercents (float wayToNextDistrictProgressInPercents);

    // TODO: Implement it in all units.
    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const = 0;
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage) = 0;
    virtual Urho3D::String GetUnitTypeTag () const = 0;
};
}
