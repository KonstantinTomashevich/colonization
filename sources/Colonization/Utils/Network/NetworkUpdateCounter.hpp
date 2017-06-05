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
    Urho3D::Serializable *object_;
    /// When more than 100.0f points accumulated, MarkNetworkUpdate will be called.
    float accumulatedUpdatePoints_;

public:
    NetworkUpdateCounter (Urho3D::Context *context);
    virtual ~NetworkUpdateCounter ();
    static void RegisterObject (Urho3D::Context *context);

    //@ASBindGen Function AddRef_arg-1 OverrideName=get_object
    Urho3D::Serializable *GetObject ();
    //@ASBindGen Function AddRef_arg0 OverrideName=set_object
    void SetObject (Urho3D::Serializable *object);

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
