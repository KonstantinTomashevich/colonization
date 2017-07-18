#include <Colonization/BuildConfiguration.hpp>
#include "NetworkUpdateCounterUtils.hpp"
#include <Urho3D/Scene/Node.h>

namespace Colonization
{
namespace NetworkUpdateCounterUtils
{
NetworkUpdateCounter *CreateNetworkUpdateCounterForComponent(Urho3D::Component *component)
{
    NetworkUpdateCounter *counter = component->GetNode ()->CreateComponent <NetworkUpdateCounter> (Urho3D::LOCAL);
    counter->SetTrackingObject (component);
    return counter;
}

NetworkUpdateCounter *CreateNetworkUpdateCounterForNode(Urho3D::Node *node)
{
    NetworkUpdateCounter *counter = node->CreateComponent <NetworkUpdateCounter> (Urho3D::LOCAL);
    counter->SetTrackingObject (node);
    return counter;
}

void AddNetworkUpdatePointsToComponentCounter (Urho3D::Component *component, float updatePoints)
{
    NetworkUpdateCounter *counter = component->GetNode ()->GetComponent <NetworkUpdateCounter> ();
    if (!counter)
    {
        counter = CreateNetworkUpdateCounterForComponent (component);
    }
    counter->AddUpdatePoints (updatePoints);
}

void AddNetworkUpdatePointsToNodeCounter (Urho3D::Node *node, float updatePoints)
{
    NetworkUpdateCounter *counter = node->GetComponent <NetworkUpdateCounter> ();
    if (!counter)
    {
        counter = CreateNetworkUpdateCounterForNode (node);
    }
    counter->AddUpdatePoints (updatePoints);
}
}
}
