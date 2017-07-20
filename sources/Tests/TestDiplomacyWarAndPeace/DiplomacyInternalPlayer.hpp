#pragma once
#include <Colonization/Backend/Player/Player.hpp>

class DiplomacyInternalPlayer : public Colonization::Player
{
URHO3D_OBJECT (DiplomacyInternalPlayer, Player)
protected:
    int declinesCount_;
    virtual void AfterActionsProcessing (float timeStep) {}

public:
    DiplomacyInternalPlayer (Urho3D::Context *context, Urho3D::String name, Urho3D::Color color, Urho3D::Scene *scene);
    virtual ~DiplomacyInternalPlayer ();

    virtual void SendMessage (int messageId, bool reliable, bool inOrder, const Urho3D::VectorBuffer &message, unsigned contentId = 0);
    virtual void Disconnect (int wait);
    virtual bool IsInternal () const;
};
