#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Serializable.h>

namespace Colonization
{
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

    Urho3D::Serializable *GetObject ();
    void SetObject (Urho3D::Serializable *object);

    float GetAccumulatedUpdatePoints ();
    void AddUpdatePoints (float points);
};

NetworkUpdateCounter *CreateNetworkUpdateCounterForComponent (Urho3D::Component *component);
NetworkUpdateCounter *CreateNetworkUpdateCounterForNode (Urho3D::Node *node);
}
