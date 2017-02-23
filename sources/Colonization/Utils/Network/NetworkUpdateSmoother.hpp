#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/Container/Vector.h>

namespace Colonization
{
class NetworkUpdateSmoother : public Urho3D::Component
{
URHO3D_OBJECT (NetworkUpdateSmoother, Component)
protected:
    Urho3D::Vector <Urho3D::WeakPtr <Urho3D::Serializable> > requests_;
    int averageMarksPerUpdate_;
    int maxMarksPerUpdate_;
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    NetworkUpdateSmoother (Urho3D::Context *context);
    virtual ~NetworkUpdateSmoother ();

    static void RegisterObject (Urho3D::Context *context);
    void NetworkUpdate (Urho3D::StringHash, Urho3D::VariantMap &eventData);
    void RequestNetworkUpdate (Urho3D::Serializable *object);

    int GetAverageMarksPerUpdate ();
    void SetAverageMarksPerUpdate (int averageMarksPerUpdate);

    int GetMaxMarksPerUpdate ();
    void SetMaxMarksPerUpdate (int maxMarksPerUpdate);
};
}
