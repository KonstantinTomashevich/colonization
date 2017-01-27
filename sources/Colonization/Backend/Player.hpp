#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Network/Connection.h>

namespace Colonization
{
enum PlayerActionType
{
    PLAYER_ACTION_NONE = 0,
    PLAYER_ACTION_SET_UNIT_MOVE_TARGET = 1,

    PLAYER_ACTION_INVEST_TO_COLONY = 2,
    PLAYER_ACTION_CHANGE_INTERNAL_TRADE_TAXES = 3,
    PLAYER_ACTION_CHANGE_EUROPE_TRADE_TAXES = 4,

    PLAYER_ACTION_REQUEST_COLONIZATORS_FROM_EUROPE = 5,
    PLAYER_ACTION_REQUEST_ARMY_FROM_EUROPE = 6,
    PLAYER_ACTION_REQUEST_FLEET_FROM_EUROPE = 7,

    PLAYER_ACTION_MOBILIZE_ARMY = 8,
    PLAYER_ACTION_IMMOBILIZE_ARMY = 9,

    PLAYER_ACTION_START_WAR = 10,
    PLAYER_ACTION_SEND_PEACE_TREATY = 11,
    PLAYER_ACTION_ACCEPT_PEACE_TREATY = 12,
    PLAYER_ACTION_SEND_GIFT_TO_THE_KING = 13

    // TODO: To be continued.
};

class Player : public Urho3D::Object
{
URHO3D_OBJECT (Player, Object)
protected:
    Urho3D::Scene *scene_;
    Urho3D::String name_;
    Urho3D::Color color_;
    float points_;
    float gold_;
    Urho3D::Vector <Urho3D::Pair <PlayerActionType, Urho3D::Variant> > actionsSequence_;
    Urho3D::Connection *connection_;

    void ProcessSetUnitMoveTargetAction (Urho3D::VectorBuffer data);
    void ProcessInvestToColonyAction (Urho3D::VectorBuffer data);
    void ProcessRequestColonizatorsFromEuropeAction (Urho3D::VectorBuffer data);
public:
    Player (Urho3D::Context *context, Urho3D::String name, Urho3D::Color color, Urho3D::Connection *connection, Urho3D::Scene *scene);
    virtual ~Player ();

    void Update (float delta);
    Urho3D::Pair <PlayerActionType, Urho3D::Variant> GetAction (int index);
    void RemoveAction (int index);
    bool RemoveAction (Urho3D::Pair <PlayerActionType, Urho3D::Variant> &action);
    void RemoveAllActions ();
    void AddAction (Urho3D::Pair <PlayerActionType, Urho3D::Variant> &action);
    int GetActionsCount ();

    Urho3D::String GetName ();
    Urho3D::Color GetColor ();

    Urho3D::Connection *GetConnection ();
    Urho3D::Scene *GetScene ();

    float GetGold ();
    void SetGold (float gold);   

    float GetPoints ();
    void SetPoints (float points);
};
}
