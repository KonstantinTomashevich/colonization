#include <Colonization/BuildConfiguration.hpp>
#include "Player.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/ColoniesManager.hpp>

namespace Colonization
{
void Player::ProcessSetUnitMoveTargetAction (Urho3D::VectorBuffer data)
{
    Map *map = scene_->GetChild ("map")->GetComponent <Map> ();
    UnitsManager *unitsManager = scene_->GetChild ("units")->GetComponent <UnitsManager> ();
    // Skip action type.
    data.ReadInt ();

    assert (map);
    assert (unitsManager);

    Urho3D::StringHash unitHash = data.ReadStringHash ();
    Urho3D::StringHash targetDistrictHash = data.ReadStringHash ();

    Unit *unit = unitsManager->GetUnitByHash (unitHash);
    District *target = map->GetDistrictByHash (targetDistrictHash);

    assert (unit);
    assert (target);

    if (unit->GetOwnerPlayerName () != name_)
    {
        return;
    }

    if ((unit->GetUnitType () == UNIT_FLEET && target->IsSea ()) ||
            (unit->GetUnitType () != UNIT_FLEET && target->HasColony () && target->GetColonyOwnerName () == name_) ||
            (unit->GetUnitType () == UNIT_COLONIZATORS && !target->IsSea ()))
    {
        unit->SetWay (map->FindPath (unit->GetPositionHash (), target->GetHash (), name_,
                                     unit->GetUnitType () != UNIT_FLEET, unit->GetUnitType () == UNIT_COLONIZATORS));
        unit->MarkNetworkUpdate ();
    }
}

void Player::ProcessInvestToColonyAction (Urho3D::VectorBuffer data)
{
    Map *map = scene_->GetChild ("map")->GetComponent <Map> ();
    ColoniesManager *coloniesManager = scene_->GetComponent <ColoniesManager> ();
    // Skip action type.
    data.ReadInt ();

    Urho3D::StringHash targetDistrictHash = data.ReadStringHash ();
    District *targetDistrict = map->GetDistrictByHash (targetDistrictHash);
    assert (targetDistrict);
    assert (!targetDistrict->IsSea ());
    assert (!targetDistrict->IsImpassable ());
    assert (targetDistrict->HasColony () && targetDistrict->GetColonyOwnerName () == name_);

    Urho3D::StringHash investitionType = data.ReadStringHash ();
    float money = data.ReadFloat ();

    if (gold_ >= money)
    {
        coloniesManager->Invest (targetDistrict, investitionType, money);
        gold_ -= money;
    }
}

void Player::ProcessRequestColonizatorsFromEuropeAction (Urho3D::VectorBuffer data)
{
    // TODO: It's not a final version. May be rewrited later.
    if (gold_ >= 100.0f)
    {
        Map *map = scene_->GetChild ("map")->GetComponent <Map> ();
        UnitsManager *unitsManager = scene_->GetChild ("units")->GetComponent <UnitsManager> ();
        // Skip action type.
        data.ReadInt ();

        assert (map);
        assert (unitsManager);

        Urho3D::StringHash targetDistrictHash = data.ReadStringHash ();
        District *targetDistrict = map->GetDistrictByHash (targetDistrictHash);
        assert (targetDistrict);
        assert (!targetDistrict->IsSea ());
        assert (!targetDistrict->IsImpassable ());
        assert (!targetDistrict->HasColony () || (targetDistrict->HasColony () && targetDistrict->GetColonyOwnerName () == name_));

        GameConfiguration *configuration = scene_->GetComponent <GameConfiguration> ();
        District *nearestEuropeDistrict = 0;
        Urho3D::PODVector <Urho3D::StringHash> wayToEuropeDistricts = configuration->GetWayToEuropeDistricts ();
        nearestEuropeDistrict = map->GetDistrictByHash (wayToEuropeDistricts.At (0));
        assert (nearestEuropeDistrict);
        float minDistance = (nearestEuropeDistrict->GetUnitPosition () - targetDistrict->GetUnitPosition ()).Length ();

        if (wayToEuropeDistricts.Size () > 1)
        {
            for (int index = 1; index < wayToEuropeDistricts.Size (); index++)
            {
                District *district = map->GetDistrictByHash (wayToEuropeDistricts.At (index));
                assert (district);
                if ((district->GetUnitPosition () - targetDistrict->GetUnitPosition ()).Length () < minDistance)
                {
                    nearestEuropeDistrict = district;
                    minDistance = (district->GetUnitPosition () - targetDistrict->GetUnitPosition ()).Length ();
                }
            }
        }

        Urho3D::PODVector <Urho3D::StringHash> way = map->FindPath (
                    nearestEuropeDistrict->GetHash (), targetDistrict->GetHash (), name_, true, true);
        if (!way.Empty ())
        {
            gold_ -= 100.0f;
            Unit *unit = unitsManager->CreateUnit ();
            unit->SetUnitType (UNIT_COLONIZATORS);
            unit->ColonizatorsUnitSetColonizatorsCount (100);
            unit->SetOwnerPlayerName (name_);

            unit->SetPositionHash (nearestEuropeDistrict->GetHash ());
            unit->SetWay (way);
            unit->UpdateHash (unitsManager);
            unit->MarkNetworkUpdate ();
        }
        else
        {
            Urho3D::Log::Write (Urho3D::LOG_ERROR, "Can't send colonizators. Can't find way from " +
                                nearestEuropeDistrict->GetName () + " to " +
                                targetDistrict->GetName () + "!");
        }
    }
}

Player::Player (Urho3D::Context *context, Urho3D::String name, Urho3D::Color color, Urho3D::Connection *connection, Urho3D::Scene *scene) :
    Urho3D::Object (context),
    scene_ (scene),
    name_ (name),
    color_ (color),
    gold_ (0.0f),
    points_ (0.0f),
    timeUntilNewChatMessage_ (0.0f),
    actionsSequence_ (),
    connection_ (connection)
{
    assert (scene_);
    assert (connection);
    assert (!name.Empty ());
}

Player::~Player ()
{

}

void Player::Update (float timeStep)
{
    if (timeUntilNewChatMessage_ >= 0.0f)
    {
        timeUntilNewChatMessage_ -= timeStep;
    }

    while (!actionsSequence_.Empty ())
    {
        Urho3D::Pair <PlayerActionType, Urho3D::Variant> action = actionsSequence_.At (0);
        if (action.first_ == PLAYER_ACTION_SET_UNIT_MOVE_TARGET)
        {
            ProcessSetUnitMoveTargetAction (action.second_.GetVectorBuffer ());
        }
        else if (action.first_ == PLAYER_ACTION_INVEST_TO_COLONY)
        {
            ProcessInvestToColonyAction (action.second_.GetVectorBuffer ());
        }
        else if (action.first_ == PLAYER_ACTION_REQUEST_COLONIZATORS_FROM_EUROPE)
        {
            ProcessRequestColonizatorsFromEuropeAction (action.second_.GetVectorBuffer ());
        }
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

Urho3D::Color Player::GetColor ()
{
    return color_;
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

Urho3D::Scene *Player::GetScene ()
{
    return scene_;
}

float Player::GetPoints ()
{
    return points_;
}

void Player::SetPoints (float points)
{
    points_ = points;
}

float Player::GetTimeUntilNewChatMessage ()
{
    return timeUntilNewChatMessage_;
}

void Player::OnChatMessageSended ()
{
    timeUntilNewChatMessage_ = PLAYER_NEW_CHAT_MESSAGE_DELAY;
}
}
