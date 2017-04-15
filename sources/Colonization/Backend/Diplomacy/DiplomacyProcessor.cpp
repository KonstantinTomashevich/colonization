#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyProcessor.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void DiplomacyProcessor::UpdateDiplomacyRequests (float timeStep)
{
    Urho3D::Node *requestsRoot = GetOrCreateRequestsNode ();
    const Urho3D::Vector <Urho3D::SharedPtr <Urho3D::Node> > children = requestsRoot->GetChildren ();

    for (int index = 0; index < children.Size (); index++)
    {
        Urho3D::Node *requestNode = children.At (index).Get ();
        DiplomacyRequest *request = requestNode->GetDerivedComponent <DiplomacyRequest> ();
        if (request->TimeUpdate (timeStep))
        {
            requestNode->Remove ();
        }
    }
}

Urho3D::Node *DiplomacyProcessor::GetDiplomacyRequestNodeById (unsigned requestId)
{
    Urho3D::Node *requestsRoot = GetOrCreateRequestsNode ();
    const Urho3D::Vector <Urho3D::SharedPtr <Urho3D::Node> > children = requestsRoot->GetChildren ();

    for (int index = 0; index < children.Size (); index++)
    {
        Urho3D::Node *requestNode = children.At (index).Get ();
        DiplomacyRequest *request = requestNode->GetDerivedComponent <DiplomacyRequest> ();
        if (request->GetRequestId () == requestId)
        {
            return requestNode;
        }
    }
    return 0;
}

unsigned DiplomacyProcessor::GetFreeDiplomacyRequestId ()
{
    unsigned id = 1;
    Urho3D::Node *requestsRoot = GetOrCreateRequestsNode ();
    const Urho3D::Vector <Urho3D::SharedPtr <Urho3D::Node> > children = requestsRoot->GetChildren ();

    for (int index = 0; index < children.Size (); index++)
    {
        Urho3D::Node *requestNode = children.At (index).Get ();
        DiplomacyRequest *request = requestNode->GetDerivedComponent <DiplomacyRequest> ();
        if (request->GetRequestId () >= id)
        {
            id = request->GetRequestId () + 1;
        }
    }
    return id;
}

Urho3D::Node *DiplomacyProcessor::GetOrCreateRequestsNode ()
{
    Urho3D::Node *requestsNode = node_->GetChild ("requests");
    if (!requestsNode)
    {
        requestsNode = node_->CreateChild ("requests", Urho3D::LOCAL);
    }
    return requestsNode;
}

void DiplomacyProcessor::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (DiplomacyProcessor, Update));
}

DiplomacyProcessor::DiplomacyProcessor (Urho3D::Context *context) : Urho3D::Component (context),
    wars_ ()
{

}

DiplomacyProcessor::~DiplomacyProcessor ()
{

}

void DiplomacyProcessor::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <DiplomacyProcessor> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void DiplomacyProcessor::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
        UpdateWarsList ();
        UpdateDiplomacyRequests (timeStep);
        // TODO: Process wars escalation.
    }
}

void DiplomacyProcessor::AddDiplomacyRequest (DiplomacyRequest *request)
{
    request->SetRequestId (GetFreeDiplomacyRequestId ());
    Urho3D::Node *requestNode = GetOrCreateRequestsNode ()->
            CreateChild ("Request" + Urho3D::String (request->GetRequestId ()), Urho3D::LOCAL);
    requestNode->AddComponent (request, requestNode->GetID () + 1, Urho3D::LOCAL);
    request->OnAddition ();
}

void DiplomacyProcessor::UpdateDiplomacyRequestPlayerStatus (unsigned requestId, Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status)
{
    Urho3D::Node *requestNode = GetDiplomacyRequestNodeById (requestId);
    if (requestNode)
    {
        DiplomacyRequest *request = requestNode->GetDerivedComponent <DiplomacyRequest> ();
        if (request->UpdatePlayerStatus (playerNameHash, status))
        {
            requestNode->Remove ();
        }
    }
}

int DiplomacyProcessor::GetWarsCount () const
{
    return wars_.Size ();
}

void DiplomacyProcessor::UpdateWarsList ()
{
    assert (node_);
    wars_.Clear ();
    Urho3D::PODVector <Urho3D::Node *> nodesWithWars;
    node_->GetChildrenWithComponent (nodesWithWars, DiplomacyWar::GetTypeStatic ());

    for (int index = 0; index < nodesWithWars.Size (); index++)
    {
        Urho3D::Node *node = nodesWithWars.At (index);
        if (node->GetID () < Urho3D::FIRST_LOCAL_ID)
        {
            DiplomacyWar *war = node->GetComponent <DiplomacyWar> ();
            wars_.Push (Urho3D::SharedPtr <DiplomacyWar> (war));
        }
    }
}

DiplomacyWar *DiplomacyProcessor::CreateWar ()
{
    assert (node_);
    Urho3D::Node *warNode = node_->CreateChild ("War", Urho3D::REPLICATED);
    warNode->SetName ("War" + Urho3D::String (warNode->GetID ()));
    Urho3D::SharedPtr <DiplomacyWar> war (warNode->CreateComponent <DiplomacyWar> (Urho3D::REPLICATED));
    wars_.Push (war);
    war->UpdateHash (this);
    return war;
}

DiplomacyWar *DiplomacyProcessor::GetWarByIndex (int index)
{
    assert (index < wars_.Size ());
    return wars_.At (index);
}

DiplomacyWar *DiplomacyProcessor::GetWarByHash (Urho3D::StringHash hash)
{
    for (int index = 0; index < wars_.Size (); index++)
    {
        DiplomacyWar *war = wars_.At (index);
        if (war->GetHash () == hash)
        {
            return war;
        }
    }
    return 0;
}

bool DiplomacyProcessor::RemoveWarByHash (Urho3D::StringHash hash)
{
    Urho3D::SharedPtr <DiplomacyWar> war (GetWarByHash (hash));
    if (war.NotNull ())
    {
        wars_.Remove (war);
        war->GetNode ()->Remove ();
        return true;
    }
    else
    {
        return false;
    }
}
}
