#pragma once
#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>

namespace Colonization
{
namespace NetworkUpdateCounterUtils
{
//@ASBindGen Function AddRef_arg-1 AddNamespaceToCallName=NetworkUpdateCounterUtils
NetworkUpdateCounter *CreateNetworkUpdateCounterForComponent (Urho3D::Component *component);
//@ASBindGen Function AddRef_arg-1 AddNamespaceToCallName=NetworkUpdateCounterUtils
NetworkUpdateCounter *CreateNetworkUpdateCounterForNode (Urho3D::Node *node);
//@ASBindGen Function AddNamespaceToCallName=NetworkUpdateCounterUtils
void AddNetworkUpdatePointsToComponentCounter (Urho3D::Component *component, float updatePoints);
//@ASBindGen Function AddNamespaceToCallName=NetworkUpdateCounterUtils
void AddNetworkUpdatePointsToNodeCounter (Urho3D::Node *node, float updatePoints);
}
}
