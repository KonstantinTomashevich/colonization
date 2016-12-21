#include <Colonization/BuildConfiguration.hpp>
#include "Player.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/ColoniesManager.hpp>

namespace Colonization
{
void Player::ProcessSetUnitMoveTargetAction (Urho3D::VectorBuffer data)
{
    Map *map = (Map *) context_->GetGlobalVar ("Map").GetPtr ();
    UnitsManager *unitsManager = (UnitsManager *) context_->GetGlobalVar ("UnitsManager").GetPtr ();
    // Skip action type.
    data.ReadInt ();

    assert (map);
    assert (unitsManager);

    Urho3D::StringHash unitHash = data.ReadStringHash ();
    Urho3D::StringHash targetDistrictHash = data.ReadStringHash ();

    Unit *unit = unitsManager->GetUnitsContainer ()->GetUnitByHash (unitHash);
    District *target = map->GetDistrictByHash (targetDistrictHash);

    assert (unit);
    assert (target);

    if (unit->ownerPlayer_ != name_)
        return;

    if ((unit->unitType_ == UNIT_FLEET && target->isSea_) ||
            (unit->unitType_ != UNIT_FLEET && target->hasColony_ && target->colonyOwnerName_ == name_) ||
            (unit->unitType_ == UNIT_COLONIZATORS && !target->isSea_))
        unit->way_ = map->FindPath (unit->position_, target, name_, unit->unitType_ != UNIT_FLEET, unit->unitType_ == UNIT_COLONIZATORS);
}

void Player::ProcessInvestToColonyAction (Urho3D::VectorBuffer data)
{
    Map *map = (Map *) context_->GetGlobalVar ("Map").GetPtr ();
    ColoniesManager *coloniesManager = (ColoniesManager *) context_->GetGlobalVar ("ColoniesManager").GetPtr ();
    // Skip action type.
    data.ReadInt ();

    Urho3D::StringHash targetDistrictHash = data.ReadStringHash ();
    District *targetDistrict = map->GetDistrictByHash (targetDistrictHash);
    assert (targetDistrict);
    assert (!targetDistrict->isSea_);
    assert (!targetDistrict->isImpassable_);
    assert (!targetDistrict->hasColony_ || (targetDistrict->hasColony_ && targetDistrict->colonyOwnerName_ == name_));

    Urho3D::StringHash investitionType = data.ReadStringHash ();
    float money = data.ReadFloat ();
    assert (money < gold_);

    coloniesManager->Invest (targetDistrict, investitionType, money);
    gold_ -= money;
}

void Player::ProcessRequestColonizatorsFromEuropeAction (Urho3D::VectorBuffer data)
{
    // TODO: It's not a final version. May be rewrited later.
    if (gold_ >= 100.0f)
    {
        Map *map = (Map *) context_->GetGlobalVar ("Map").GetPtr ();
        UnitsManager *unitsManager = (UnitsManager *) context_->GetGlobalVar ("UnitsManager").GetPtr ();
        // Skip action type.
        data.ReadInt ();

        assert (map);
        assert (unitsManager);

        Urho3D::StringHash targetDistrictHash = data.ReadStringHash ();
        District *targetDistrict = map->GetDistrictByHash (targetDistrictHash);
        assert (targetDistrict);
        assert (!targetDistrict->isSea_);
        assert (!targetDistrict->isImpassable_);
        assert (!targetDistrict->hasColony_ || (targetDistrict->hasColony_ && targetDistrict->colonyOwnerName_ == name_));

        gold_ -= 100.0f;
        ColonizatorsUnit *colonizatorsUnit = new ColonizatorsUnit (context_);
        colonizatorsUnit->colonizatorsCount_ = 100;
        // TODO: Position is temporary! Will be rewrited!
        colonizatorsUnit->position_ = map->GetDistrictByIndex (4 * 5 + 4); // [X * HEIGHT + Y] = (X, Y)
        colonizatorsUnit->way_ = map->FindPath (colonizatorsUnit->position_, targetDistrict, name_, true, true);
        assert (!colonizatorsUnit->way_.Empty ());
        colonizatorsUnit->ownerPlayer_ = name_;
        unitsManager->GetUnitsContainer ()->AddUnit (colonizatorsUnit);
    }
}

Player::Player (Urho3D::Context *context, Urho3D::String name, Urho3D::Connection *connection) : Urho3D::Object (context),
    name_ (name),
    gold_ (0.0f),
    points_ (0.0f),
    actionsSequence_ (),
    connection_ (connection)
{
    assert (connection);
    assert (!name.Empty ());
}

Player::~Player ()
{

}

void Player::Update (float delta)
{
    while (!actionsSequence_.Empty ())
    {
        Urho3D::Pair <PlayerActionType, Urho3D::Variant> action = actionsSequence_.At (0);

        if (action.first_ == PLAYER_ACTION_SET_UNIT_MOVE_TARGET)
            ProcessSetUnitMoveTargetAction (action.second_.GetVectorBuffer ());

        else if (action.first_ == PLAYER_ACTION_INVEST_TO_COLONY)
            ProcessInvestToColonyAction (action.second_.GetVectorBuffer ());

        else if (action.first_ == PLAYER_ACTION_REQUEST_COLONIZATORS_FROM_EUROPE)
            ProcessRequestColonizatorsFromEuropeAction (action.second_.GetVectorBuffer ());

        actionsSequence_.Remove (actionsSequence_.At (0));
    }
}

Urho3D::Pair <PlayerActionType, Urho3D::Variant> Player::GetAction(int index)
{
    assert (index < actionsSequence_.Size ());
    return actionsSequence_.At (index);
}

void Player::RemoveAction (int index)
{
    assert (index < actionsSequence_.Size ());
    actionsSequence_.Erase (index);
}

bool Player::RemoveAction (Urho3D::Pair <PlayerActionType, Urho3D::Variant> &action)
{
    return actionsSequence_.Remove (action);
}

void Player::RemoveAllActions ()
{
    actionsSequence_.Clear ();
}

void Player::AddAction (Urho3D::Pair <PlayerActionType, Urho3D::Variant> &action)
{
    actionsSequence_.Push (action);
}

int Player::GetActionsCount ()
{
    return actionsSequence_.Size ();
}

Urho3D::String Player::GetName ()
{
    return name_;
}

float Player::GetGold ()
{
    return gold_;
}

void Player::SetGold (float gold)
{
    gold_ = gold;
}

Urho3D::Connection *Player::GetConnection ()
{
    return connection_;
}

float Player::GetPoints ()
{
    return points_;
}

void Player::SetPoints (float points)
{
    points_ = points;
}
}
