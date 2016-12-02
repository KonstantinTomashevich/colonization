#include <Colonization/BuildConfiguration.hpp>
#include "Player.hpp"

namespace Colonization
{
Player::Player (Urho3D::Context *context, Urho3D::String name) : Urho3D::Object (context),
    name_ (name),
    gold_ (0.0f),
    actionsSequence_ ()
{

}

Player::~Player ()
{

}

void Player::Update (float delta)
{
    while (!actionsSequence_.Empty ())
    {
        Urho3D::Pair <PlayerActionType, Urho3D::VariantMap> action = actionsSequence_.Front ();
        /* Do action */
        actionsSequence_.Remove (action);
    }
}

Urho3D::Pair <PlayerActionType, Urho3D::VariantMap> Player::GetAction (int index)
{
    assert (index < actionsSequence_.Size ());
    return actionsSequence_.At (index);
}

void Player::RemoveAction (int index)
{
    assert (index < actionsSequence_.Size ());
    actionsSequence_.Erase (index);
}

bool Player::RemoveAction (Urho3D::Pair <PlayerActionType, Urho3D::VariantMap> &action)
{
    return actionsSequence_.Remove (action);
}

void Player::RemoveAllActions ()
{
    actionsSequence_.Clear ();
}

void Player::AddAction (Urho3D::Pair<PlayerActionType, Urho3D::VariantMap> &action)
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
}
