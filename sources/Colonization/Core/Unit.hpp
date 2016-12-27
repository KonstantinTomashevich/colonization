#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/District.hpp>
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

class Unit : public Urho3D::Component
{
URHO3D_OBJECT (Unit, Object)
protected:
    Urho3D::StringHash hash_;
    Urho3D::String ownerPlayerName_;
    UnitType unitType_;
    Urho3D::StringHash positionHash_;
    Urho3D::PODVector <Urho3D::StringHash> way_;
    float wayToNextDistrictProgressInPercents_;
    Urho3D::VariantMap unitTypeSpecificVars_;

public:
    Unit (Urho3D::Context *context);
    virtual ~Unit ();

    static void RegisterObject (Urho3D::Context *context);
    void UpdateHash (UnitsManager *owner);
    Urho3D::StringHash GetHash ();
    void SetHash (Urho3D::StringHash hash);

    UnitType GetUnitType ();
    void SetUnitType (UnitType unitType);

    Urho3D::String GetOwnerPlayerName ();
    void SetOwnerPlayerName (Urho3D::String ownerPlayerName);

    Urho3D::StringHash GetPositionHash ();
    void SetPositionHash (Urho3D::StringHash positionHash);

    Urho3D::PODVector <Urho3D::StringHash> GetWay ();
    void SetWay (Urho3D::PODVector <Urho3D::StringHash> way);
    Urho3D::VariantVector GetWayAttribute ();
    void SetWayAttribute (Urho3D::VariantVector way);

    float GetWayToNextDistrictProgressInPercents ();
    void SetWayToNextDistrictProgressInPercents (float wayToNextDistrictProgressInPercents);

    int FleetUnitGetWarShipsCount ();
    void FleetUnitSetWarShipsCount (int warShipsCount);

    float TradersUnitGetTradeGoodsCost ();
    void TradersUnitSetTradeGoodsCost (float tradeGoodsCost);

    int ColonizatorsUnitGetColonizatorsCount ();
    void ColonizatorsUnitSetColonizatorsCount (int colonizatorsCount);

    int ArmyUnitGetSoldiersCount ();
    void ArmyUnitSetSoldiersCount (int soldiersCount);
};
}
