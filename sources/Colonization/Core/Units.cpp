#include <Colonization/BuildConfiguration.hpp>
#include "Units.hpp"

namespace Colonization
{
Unit::Unit (Urho3D::Context *context, UnitType unitType) : Urho3D::Object (context),
    needDataUpdate_ (true),
    ownerPlayer_ ("???"),
    unitType_ (unitType),
    position_ (0),
    way_ (0),
    wayToNextDistrictProgressInPercents_ (0)
{

}

Unit::~Unit ()
{

}

void Unit::UpdateDataNode (Urho3D::Node *dataNode)
{
    assert (dataNode);
    assert (position_);
    if (dataNode->GetVar ("ownerPlayer").GetString () != ownerPlayer_)
        dataNode->SetVar ("ownerPlayer", ownerPlayer_);

    if (static_cast <UnitType> (dataNode->GetVar ("unitType").GetInt ()) != unitType_)
        dataNode->SetVar ("unitType", static_cast <int> (unitType_));

    if (dataNode->GetVar ("position").GetString () != position_->name_)
        dataNode->SetVar ("position", position_->name_);

    if (dataNode->GetVar ("wayToNextDistrictProgress").GetFloat () != wayToNextDistrictProgressInPercents_)
        dataNode->SetVar ("wayToNextDistrictProgress", wayToNextDistrictProgressInPercents_);

    if (way_.Empty ())
        dataNode->SetVar ("wayTarget", "no_way_target");
    else
        dataNode->SetVar ("wayTarget", way_.At (way_.Size () - 1)->name_);
}

void Unit::ReadDataFromNode (Urho3D::Node *dataNode, Map *map)
{
    assert (dataNode);
    assert (map);
    ownerPlayer_ = dataNode->GetVar ("ownerPlayer").GetString ();
    unitType_ = static_cast <UnitType> (dataNode->GetVar ("unitType").GetInt ());

    position_ = map->GetDistrictByNameHash (Urho3D::StringHash (
                                                dataNode->GetVar ("position").GetString ()));
    assert (position_);
    wayToNextDistrictProgressInPercents_ = dataNode->GetVar ("wayToNextDistrictProgress").GetFloat ();

    District *wayTarget = map->GetDistrictByNameHash (Urho3D::StringHash (
                                                          dataNode->GetVar ("wayTarget").GetString ()));
    way_.Clear ();
    if (wayTarget)
        way_ = map->FindPath (position_, wayTarget, ownerPlayer_, unitType_ != UNIT_FLEET);
}

FleetUnit::FleetUnit (Urho3D::Context *context) : Unit (context, UNIT_FLEET),
    warShipsCount_ (0)
{

}

FleetUnit::~FleetUnit ()
{

}

void FleetUnit::UpdateDataNode (Urho3D::Node *dataNode)
{
    Unit::UpdateDataNode (dataNode);
    if (dataNode->GetVar ("warShipsCount").GetInt () != warShipsCount_)
        dataNode->SetVar ("warShipsCount", warShipsCount_);
}

void FleetUnit::ReadDataFromNode (Urho3D::Node *dataNode, Map *map)
{
    Unit::ReadDataFromNode (dataNode, map);
    warShipsCount_ = dataNode->GetVar ("warShipsCount").GetInt ();
}

TradersUnit::TradersUnit (Urho3D::Context *context) : Unit (context, UNIT_TRADERS),
    tradeGoodsCost_ (0)
{

}

TradersUnit::~TradersUnit ()
{

}

void TradersUnit::UpdateDataNode (Urho3D::Node *dataNode)
{
    Unit::UpdateDataNode (dataNode);
    if (dataNode->GetVar ("tradeGoodsCost").GetFloat () != tradeGoodsCost_)
        dataNode->SetVar ("tradeGoodsCost", tradeGoodsCost_);
}

void TradersUnit::ReadDataFromNode (Urho3D::Node *dataNode, Map *map)
{
    Unit::ReadDataFromNode (dataNode, map);
    tradeGoodsCost_ = dataNode->GetVar ("tradeGoodsCost").GetFloat ();
}

ColonizatorsUnit::ColonizatorsUnit (Urho3D::Context *context) : Unit (context, UNIT_COLONIZATORS),
    colonizatorsCount_ (0)
{

}

ColonizatorsUnit::~ColonizatorsUnit ()
{

}

void ColonizatorsUnit::UpdateDataNode (Urho3D::Node *dataNode)
{
    Unit::UpdateDataNode (dataNode);
    if (dataNode->GetVar ("colonizatorsCount").GetInt () != colonizatorsCount_)
        dataNode->SetVar ("colonizatorsCount", colonizatorsCount_);
}

void ColonizatorsUnit::ReadDataFromNode (Urho3D::Node *dataNode, Map *map)
{
    Unit::ReadDataFromNode (dataNode, map);
    colonizatorsCount_ = dataNode->GetVar ("colonizatorsCount").GetInt ();
}

ArmyUnit::ArmyUnit (Urho3D::Context *context) : Unit (context, UNIT_ARMY),
    soldiersCount_ (0)
{

}

ArmyUnit::~ArmyUnit ()
{

}

void ArmyUnit::UpdateDataNode (Urho3D::Node *dataNode)
{
    Unit::UpdateDataNode (dataNode);
    if (dataNode->GetVar ("soldiersCount").GetInt () != soldiersCount_)
        dataNode->SetVar ("soldiersCount", soldiersCount_);
}

void ArmyUnit::ReadDataFromNode (Urho3D::Node *dataNode, Map *map)
{
    Unit::ReadDataFromNode (dataNode, map);
    soldiersCount_ = dataNode->GetVar ("soldiersCount").GetInt ();
}
}
