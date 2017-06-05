#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Network/Connection.h>
#include <Colonization/Backend/Player/PlayerActionType.hpp>

namespace Colonization
{
//@ASBindGen Constant
const float PLAYER_NEW_CHAT_MESSAGE_DELAY = 1.0f;
class Player : public Urho3D::Object
{
URHO3D_OBJECT (Player, Object)
protected:
    Urho3D::String name_;
    Urho3D::Color color_;
    float points_;
    float gold_;
    bool isReadyForStart_;

    Urho3D::Scene *scene_;
    float timeUntilNewChatMessage_;
    Urho3D::Vector <Urho3D::Pair <PlayerActionType, Urho3D::Variant> > actionsSequence_;
    Urho3D::PODVector <Urho3D::StringHash> enemies_;
    Urho3D::Connection *connection_;

    void ProcessSetUnitMoveTargetAction (Urho3D::VectorBuffer data);
    void ProcessInvestToColonyAction (Urho3D::VectorBuffer data);
    void ProcessRequestColonizatorsFromEuropeAction (Urho3D::VectorBuffer data);
    void ProcessAddColonyActionAction (Urho3D::VectorBuffer data);
    void ProcessRemoveColonyActionAction (Urho3D::VectorBuffer data);

    void ProcessDeclareWarAction (Urho3D::VectorBuffer data);
    void ProcessSendPeaceTreatyAction (Urho3D::VectorBuffer data);
    void ProcessResponceToDiplomacyOfferAction (Urho3D::VectorBuffer data);
    virtual void AfterActionsProcessing (float timeStep);
public:
    Player (Urho3D::Context *context, Urho3D::String name, Urho3D::Color color, Urho3D::Connection *connection, Urho3D::Scene *scene);
    virtual ~Player ();

    void Update (float timeStep);
    virtual void SendMessage (int messageId, bool reliable, bool inOrder, const Urho3D::VectorBuffer &message, unsigned contentId = 0);
    virtual void Disconnect (int wait);
    virtual bool IsInternal () const;

    Urho3D::Pair <PlayerActionType, Urho3D::Variant> GetAction (int index);
    void RemoveAction (int index);
    bool RemoveAction (Urho3D::Pair <PlayerActionType, Urho3D::Variant> &action);
    void RemoveAllActions ();
    void AddAction (Urho3D::Pair <PlayerActionType, Urho3D::Variant> &action);
    int GetActionsCount () const;

    Urho3D::String GetName () const;
    Urho3D::Color GetColor () const;
    void SetColor (Urho3D::Color color);

    Urho3D::Connection *GetConnection () const;
    Urho3D::Scene *GetScene () const;

    float GetGold () const;
    void SetGold (float gold);   

    float GetPoints () const;
    void SetPoints (float points);

    float GetTimeUntilNewChatMessage () const;
    void OnChatMessageSended ();

    bool IsReadyForStart () const;
    void SetIsReadyForStart (bool isReadyForStart);

    int GetEnemiesCount () const;
    Urho3D::PODVector <Urho3D::StringHash> GetEnemies () const;
    Urho3D::StringHash GetEnemyByIndex (int index) const;
    bool IsAtWarWith (Urho3D::StringHash anotherPlayerNameHash) const;
    bool AddEnemy (Urho3D::StringHash anotherPlayerNameHash);
    bool RemoveEnemy (Urho3D::StringHash anotherPlayerNameHash);
    void RemoveAllEnemies ();
};
}
