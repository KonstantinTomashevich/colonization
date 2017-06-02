#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
//@ASBindGen Enum
enum DiplomacyRequestPlayerStatus
{
    DRPSTATUS_WAITING = 0,
    DRPSTATUS_DECLINED = 1,
    DRPSTATUS_ACCEPTED = 2
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

    virtual void OnAddition () = 0;
    virtual bool UpdatePlayerStatus (Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status) = 0;
    virtual bool TimeUpdate (float timeStep) = 0;
    virtual Urho3D::String GetRequestTypeTag () const = 0;
};
}
