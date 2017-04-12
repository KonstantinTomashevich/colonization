#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Scene.h>

namespace Colonization
{
enum DiplomacyRequestPlayerStatus
{
    DPSTATUS_WAITING = 0,
    DPSTATUS_CANCELLED = 1,
    DPSTATUS_ACCEPTED = 2
};

class DiplomacyRequest : public Urho3D::Object
{
URHO3D_OBJECT (DiplomacyRequest, Object)
protected:
    unsigned id_;

public:
    DiplomacyRequest (Urho3D::Context *context);
    virtual ~DiplomacyRequest ();

    unsigned GetId () const;
    void SetId (unsigned id);

    virtual bool IsCanBeFinished () = 0;
    virtual void UpdatePlayerStatus (Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status) = 0;
    virtual void TimeUpdate (float timeStep) = 0;
    virtual void FinishProcessing (const Urho3D::Scene *scene) = 0;
};
}
