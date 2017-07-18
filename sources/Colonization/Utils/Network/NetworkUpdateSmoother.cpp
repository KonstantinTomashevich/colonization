#include <Colonization/BuildConfiguration.hpp>
#include "NetworkUpdateSmoother.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Network/NetworkEvents.h>

#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
NetworkUpdateSmoother::NetworkUpdateSmoother (Urho3D::Context *context) : Urho3D::Component (context),
    requests_ (),
    averageMarksPerUpdate_ (5),
    maxMarksPerUpdate_ (20)
{

}

NetworkUpdateSmoother::~NetworkUpdateSmoother ()
{

}

void NetworkUpdateSmoother::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <NetworkUpdateSmoother> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void NetworkUpdateSmoother::NetworkUpdate (Urho3D::StringHash, Urho3D::VariantMap &eventData)
{
    int toSend = averageMarksPerUpdate_;
    if (toSend > requests_.Size ())
    {
        toSend = requests_.Size ();
    }
    else if (toSend * 2 < requests_.Size ())
    {
        toSend = toSend * Urho3D::Sqrt ((toSend / requests_.Size ()) - 1);
    }

    if (toSend > maxMarksPerUpdate_)
    {
        toSend = maxMarksPerUpdate_;
    }

    while (toSend > 0 && !requests_.Empty ())
    {
        Urho3D::WeakPtr <Urho3D::Serializable> request = requests_.Front ();
        if (!request.Expired ())
        {
            request->MarkNetworkUpdate ();
            toSend--;
        }
        requests_.Erase (0);
    }
}

void NetworkUpdateSmoother::RequestNetworkUpdate (Urho3D::Serializable *object)
{
    requests_.Push (Urho3D::WeakPtr <Urho3D::Serializable> (object));
}

int NetworkUpdateSmoother::GetAverageMarksPerUpdate ()
{
    return averageMarksPerUpdate_;
}

void NetworkUpdateSmoother::SetAverageMarksPerUpdate (int averageMarksPerUpdate)
{
    averageMarksPerUpdate_ = averageMarksPerUpdate;
}

int NetworkUpdateSmoother::GetMaxMarksPerUpdate ()
{
    return maxMarksPerUpdate_;
}

void NetworkUpdateSmoother::SetMaxMarksPerUpdate (int maxMarksPerUpdate)
{
    maxMarksPerUpdate_ = maxMarksPerUpdate;
}

void NetworkUpdateSmoother::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (Urho3D::E_NETWORKUPDATE, URHO3D_HANDLER (NetworkUpdateSmoother, NetworkUpdate));
}
}

