#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Serializable.h>

namespace Colonization
{
//@ASBindGen Class ObjectType=Ref
class NetworkUpdateCounter : public Urho3D::Component
{
URHO3D_OBJECT (NetworkUpdateCounter, Component)
protected:
    Urho3D::Serializable *trackingObject_;
    /// When more than 100.0f points accumulated, MarkNetworkUpdate will be called.
    float accumulatedUpdatePoints_;

public:
    NetworkUpdateCounter (Urho3D::Context *context);
    virtual ~NetworkUpdateCounter ();
    static void RegisterObject (Urho3D::Context *context);

    //@ASBindGen Function AddRef_arg-1 OverrideName=get_trackingObject
    Urho3D::Serializable *GetTrackingObject ();
    //@ASBindGen Function AddRef_arg0 OverrideName=set_trackingObject
    void SetTrackingObject (Urho3D::Serializable *trackingObject);

    //@ASBindGen Function OverrideName=get_accumulatedUpdatePoints
    float GetAccumulatedUpdatePoints ();
    //@ASBindGen Function
    void AddUpdatePoints (float points);
};

//@ASBindGen Function AddRef_arg-1
NetworkUpdateCounter *CreateNetworkUpdateCounterForComponent (Urho3D::Component *component);
//@ASBindGen Function AddRef_arg-1
NetworkUpdateCounter *CreateNetworkUpdateCounterForNode (Urho3D::Node *node);
}
