#include <Colonization/BuildConfiguration.hpp>
#include "Player.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/ColoniesEvolutionManager.hpp>

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
        map->FindPath (target->GetHash (), unit);

        NetworkUpdateCounter *counter = unit->GetNode ()->GetComponent <NetworkUpdateCounter> ();
        if (!counter)
        {
            counter = CreateNetworkUpdateCounterForComponent (unit);
        }
        counter->AddUpdatePoints (100.0f);
    }
}

void Player::ProcessInvestToColonyAction (Urho3D::VectorBuffer data)
{
    Map *map = scene_->GetChild ("map")->GetComponent <Map> ();
    ColoniesEvolutionManager *coloniesManager = scene_->GetComponent <ColoniesEvolutionManager> ();
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
        targetDistrict->Invest (investitionType, money);
        gold_ -= money;

        NetworkUpdateCounter *counter = targetDistrict->GetNode ()->GetComponent <NetworkUpdateCounter> ();
        if (!counter)
        {
            counter = CreateNetworkUpdateCounterForComponent (targetDistrict);
        }
        counter->AddUpdatePoints (100.0f);
    }
}

void Player::ProcessRequestColonizatorsFromEuropeAction (Urho3D::VectorBuffer data)
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
    int colonizatorsCount = data.ReadInt ();
    float cost = colonizatorsCount * configuration->GetOneColonizatorSendingCost ();

    if (gold_ > cost)
    {
        District *nearestEuropeDistrict = map->GetDistrictByHash (
                    configuration->GetHeuristicNearestWayToEuropeDistrict (map, targetDistrict));

        Unit *unit = unitsManager->CreateUnit ();
        unit->SetOwnerPlayerName (name_);
        unit->SetPositionHash (nearestEuropeDistrict->GetHash ());
        unit->SetUnitType (UNIT_COLONIZATORS);

        if (!map->FindPath (targetDistrict->GetHash (), unit).Empty ())
        {
            gold_ -= cost;
            unit->ColonizatorsUnitSetColonizatorsCount (colonizatorsCount);

            NetworkUpdateCounter *counter = unit->GetNode ()->GetComponent <NetworkUpdateCounter> ();
            if (!counter)
            {
                counter = CreateNetworkUpdateCounterForComponent (unit);
            }
            counter->AddUpdatePoints (100.0f);
        }
        else
        {
            unit->GetNode ()->Remove ();
            Urho3D::Log::Write (Urho3D::LOG_ERROR, "Can't send colonizators. Can't find way from " +
                                nearestEuropeDistrict->GetName () + " to " +
                                targetDistrict->GetName () + "!");
        }
    }
}

void Player::ProcessAddColonyActionAction (Urho3D::VectorBuffer data)
{
    Map *map = scene_->GetChild ("map")->GetComponent <Map> ();
    // Skip action type.
    data.ReadInt ();

    Urho3D::StringHash colonyHash = data.ReadStringHash ();
    District *colony = map->GetDistrictByHash (colonyHash);
    assert (colony);

    if (colony->HasColony () && colony->GetColonyOwnerName () == name_)
    {
        Urho3D::StringHash actionType = data.ReadStringHash ();
        Urho3D::VariantMap actionData = data.ReadVariantMap ();
        colony->AddColonyAction (actionType, actionData);
    }
}

void Player::ProcessRemoveColonyActionAction (Urho3D::VectorBuffer data)
{
    Map *map = scene_->GetChild ("map")->GetComponent <Map> ();
    // Skip action type.
    data.ReadInt ();

    Urho3D::StringHash colonyHash = data.ReadStringHash ();
    District *colony = map->GetDistrictByHash (colonyHash);
    assert (colony);

    if (colony->HasColony () && colony->GetColonyOwnerName () == name_)
    {
        Urho3D::StringHash actionId = data.ReadStringHash ();
        colony->RemoveColonyActionById (actionId);
    }
}

Player::Player (Urho3D::Context *context, Urho3D::String name, Urho3D::Color color, Urho3D::Connection *connection, Urho3D::Scene *scene) :
    Urho3D::Object (context),
    name_ (name),
    color_ (color),
    gold_ (0.0f),
    points_ (0.0f),
    isReadyForStart_ (false),

    scene_ (scene),
    timeUntilNewChatMessage_ (0.0f),
    actionsSequence_ (),
    enemies_ (),
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
        else if (action.first_ == PLAYER_ACTION_ADD_COLONY_ACTION)
        {
            ProcessAddColonyActionAction (action.second_.GetVectorBuffer ());
        }
        else if (action.first_ == PLAYER_ACTION_REMOVE_COLONY_ACTION)
        {
            ProcessRemoveColonyActionAction (action.second_.GetVectorBuffer ());
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

int Player::GetActionsCount () const
{
    return actionsSequence_.Size ();
}

Urho3D::String Player::GetName () const
{
    return name_;
}

Urho3D::Color Player::GetColor () const
{
    return color_;
}

void Player::SetColor (Urho3D::Color color)
{
    color_ = color;
}

float Player::GetGold () const
{
    return gold_;
}

void Player::SetGold (float gold)
{
    gold_ = gold;
}

Urho3D::Connection *Player::GetConnection () const
{
    return connection_;
}

Urho3D::Scene *Player::GetScene () const
{
    return scene_;
}

float Player::GetPoints () const
{
    return points_;
}

void Player::SetPoints (float points)
{
    points_ = points;
}

float Player::GetTimeUntilNewChatMessage () const
{
    return timeUntilNewChatMessage_;
}

void Player::OnChatMessageSended ()
{
    timeUntilNewChatMessage_ = PLAYER_NEW_CHAT_MESSAGE_DELAY;
}

bool Player::IsReadyForStart () const
{
    return isReadyForStart_;
}

void Player::SetIsReadyForStart (bool isReadyForStart)
{
    isReadyForStart_ = isReadyForStart;
}

int Player::GetEnemiesCount () const
{
    return enemies_.Size ();
}

Urho3D::PODVector <Urho3D::StringHash> Player::GetEnemies () const
{
    return enemies_;
}

Urho3D::StringHash Player::GetEnemyByIndex (int index) const
{
    assert (index < enemies_.Size ());
    return enemies_.At (index);
}

bool Player::IsAtWarWith (Urho3D::StringHash anotherPlayerNameHash) const
{
    return enemies_.Contains (anotherPlayerNameHash);
}

bool Player::AddEnemy (Urho3D::StringHash anotherPlayerNameHash)
{
    assert (anotherPlayerNameHash != Urho3D::StringHash (name_));
    if (!enemies_.Contains (anotherPlayerNameHash))
    {
        enemies_.Push (anotherPlayerNameHash);
        return true;
    }
    else
    {
        return false;
    }
}

bool Player::RemoveEnemy (Urho3D::StringHash anotherPlayerNameHash)
{
    return enemies_.Remove (anotherPlayerNameHash);
}

void Player::RemoveAllEnemies ()
{
    enemies_.Clear ();
}
}