#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>

namespace Colonization
{
class UnitsManager;
enum UnitType
{
    UNIT_FLEET = 0,
    UNIT_TRADERS = 1,
    UNIT_COLONIZATORS = 2,
    UNIT_ARMY = 3
};

// TODO: Maybe really split it? One class for each unit type.
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

    int fleetUnitWarShipsCount_;
    Urho3D::PODVector <float> fleetUnitWarShipsHealthPoints_;

    float tradersUnitTradeGoodsCost_;
    int colonizatorsUnitColonizatorsCount_;
    int armyUnitSoldiersCount_;

public:
    Unit (Urho3D::Context *context);
    virtual ~Unit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);

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

    int FleetUnitGetWarShipsCount () const;
    void FleetUnitSetWarShipsCount (int warShipsCount);

    /// Any war ship health is in (0.0f, 100.0f].
    Urho3D::PODVector <float> FleetUnitGetWarShipsHealthPoints () const;
    void FleetUnitSetWarShipsHealthPoints (const Urho3D::PODVector <float> &warShipsHealthPoints);
    Urho3D::VariantVector FleetUnitGetWarShipsHealthPointsAttribute () const;
    void FleetUnitSetWarShipsHealthPointsAttribute (const Urho3D::VariantVector &warShipsHealthPoints);

    float TradersUnitGetTradeGoodsCost () const;
    void TradersUnitSetTradeGoodsCost (float tradeGoodsCost);

    int ColonizatorsUnitGetColonizatorsCount () const;
    void ColonizatorsUnitSetColonizatorsCount (int colonizatorsCount);

    int ArmyUnitGetSoldiersCount () const;
    void ArmyUnitSetSoldiersCount (int soldiersCount);
};
}
