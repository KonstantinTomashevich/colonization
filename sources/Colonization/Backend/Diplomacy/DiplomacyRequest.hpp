#pragma once
#include <Urho3D/Scene/Component.h>
#include <Colonization/Backend/Diplomacy/DiplomacyActionsExecutor.hpp>

namespace Colonization
{
enum DiplomacyRequestPlayerStatus
{
    DPSTATUS_WAITING = 0,
    DPSTATUS_CANCELLED = 1,
    DPSTATUS_ACCEPTED = 2
};

class DiplomacyRequest : public Urho3D::Component
{
URHO3D_OBJECT (DiplomacyRequest, Component)
protected:
    unsigned requestId_;

public:
    DiplomacyRequest (Urho3D::Context *context);
    virtual ~DiplomacyRequest ();
    static void RegisterObject (Urho3D::Context *context);

    unsigned GetRequestId () const;
    void SetRequestId (unsigned id);

    virtual bool UpdatePlayerStatus (DiplomacyActionsExecutor *diplomacyActionsExecutor, Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status) = 0;
    virtual bool TimeUpdate (DiplomacyActionsExecutor *diplomacyActionsExecutor, float timeStep) = 0;
};
}
