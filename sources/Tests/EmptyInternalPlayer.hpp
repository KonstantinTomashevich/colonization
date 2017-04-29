#pragma once
#include <Colonization/Backend/Player/Player.hpp>

namespace Tests
{
class EmptyInternalPlayer : public Colonization::Player
{
URHO3D_OBJECT (EmptyInternalPlayer, Player)
protected:
    virtual void AfterActionsProcessing (float timeStep) {}

public:
    EmptyInternalPlayer (Urho3D::Context *context, Urho3D::String name, Urho3D::Color color, Urho3D::Scene *scene) : Colonization::Player (context, name, color, 0, scene) {}
    virtual ~EmptyInternalPlayer () {}

    virtual void SendMessage (int messageId, bool reliable, bool inOrder, const Urho3D::VectorBuffer &message, unsigned contentId = 0) {}
    virtual void Disconnect (int wait) {}
    virtual bool IsInternal () const { return true; }
};
}
