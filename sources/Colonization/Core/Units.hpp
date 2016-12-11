#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>

namespace Colonization
{
enum UnitType
{
    UNIT_FLEET = 0,
    UNIT_TRADERS = 1,
    UNIT_COLONIZATORS = 2,
    UNIT_ARMY = 3
};

class Unit : public Urho3D::Object
{
URHO3D_OBJECT (Unit, Object)
protected:
    Unit (Urho3D::Context *context, UnitType unitType);
public:
    virtual ~Unit ();

    Urho3D::String ownerPlayer_;
    UnitType unitType_;
    District *position_;
    Urho3D::PODVector <District *> way_;
    float wayToNextDistrictProgressInPercents_;

    virtual void UpdateDataNode (Urho3D::Node *dataNode);
    virtual void ReadDataFromNode (Urho3D::Node *dataNode, Map *map);
};

class FleetUnit : public Unit
{
URHO3D_OBJECT (FleetUnit, Unit)
public:
    FleetUnit (Urho3D::Context *context);
    virtual ~FleetUnit ();

    int warShipsCount_;
    // TODO: To be continued...

    virtual void UpdateDataNode (Urho3D::Node *dataNode);
    virtual void ReadDataFromNode (Urho3D::Node *dataNode, Map *map);
};

class TradersUnit : public Unit
{
URHO3D_OBJECT (TradersUnit, Unit)
public:
    TradersUnit (Urho3D::Context *context);
    virtual ~TradersUnit ();

    float tradeGoodsCost_;
    // TODO: To be continued...

    virtual void UpdateDataNode (Urho3D::Node *dataNode);
    virtual void ReadDataFromNode (Urho3D::Node *dataNode, Map *map);
};

class ColonizatorsUnit : public Unit
{
URHO3D_OBJECT (ColonizatorsUnit, Unit)
public:
    ColonizatorsUnit (Urho3D::Context *context);
    virtual ~ColonizatorsUnit ();

    int colonizatorsCount_;
    // TODO: To be continued...

    virtual void UpdateDataNode (Urho3D::Node *dataNode);
    virtual void ReadDataFromNode (Urho3D::Node *dataNode, Map *map);
};

class ArmyUnit : public Unit
{
URHO3D_OBJECT (ArmyUnit, Unit)
public:
    ArmyUnit (Urho3D::Context *context);
    virtual ~ArmyUnit ();

    float soldiersCount_;
    // TODO: To be continued...

    virtual void UpdateDataNode (Urho3D::Node *dataNode);
    virtual void ReadDataFromNode (Urho3D::Node *dataNode, Map *map);
};
}
