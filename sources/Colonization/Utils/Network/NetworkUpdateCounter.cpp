#include <Colonization/BuildConfiguration.hpp>
#include "NetworkUpdateCounter.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
NetworkUpdateCounter::NetworkUpdateCounter (Urho3D::Context *context) : Urho3D::Component (context),
    trackingObject_ (0),
    accumulatedUpdatePoints_ (0.0f)
{

}

NetworkUpdateCounter::~NetworkUpdateCounter ()
{

}

void NetworkUpdateCounter::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <NetworkUpdateCounter> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

Urho3D::Serializable *NetworkUpdateCounter::GetTrackingObject ()
{
    return trackingObject_;
}

void NetworkUpdateCounter::SetTrackingObject (Urho3D::Serializable *trackingObject)
{
    assert (trackingObject);
    trackingObject_ = trackingObject;
}

float NetworkUpdateCounter::GetAccumulatedUpdatePoints ()
{
    return accumulatedUpdatePoints_;
}

void NetworkUpdateCounter::AddUpdatePoints (float points)
{
    assert (points >= 0.0f);
    if (points > 0.0f)
    {
        accumulatedUpdatePoints_ += points;
    }

    if (accumulatedUpdatePoints_ >= 100.0f)
    {
        if (trackingObject_)
        {
            NetworkUpdateSmoother *smoother = node_->GetScene ()->GetComponent <NetworkUpdateSmoother> ();
            smoother->RequestNetworkUpdate (trackingObject_);
        }
        accumulatedUpdatePoints_ = 0.0f;
    }
}
}
