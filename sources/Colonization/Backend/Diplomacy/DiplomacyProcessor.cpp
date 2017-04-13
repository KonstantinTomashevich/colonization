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
    for (Urho3D::Vector <Urho3D::SharedPtr <DiplomacyRequest> >::Iterator iterator = requests_.Begin ();
         iterator != requests_.End (); iterator++)
    {
        DiplomacyRequest *request = iterator->Get ();
        request->TimeUpdate (diplomacyActionsExexutor_, timeStep);
        if (request->IsFinished ())
        {
            iterator = requests_.Erase (iterator);
        }
    }
}

Urho3D::Vector <Urho3D::SharedPtr <DiplomacyRequest> >::Iterator DiplomacyProcessor::GetDiplomacyRequestIteratorById (unsigned requestId)
{
    for (Urho3D::Vector <Urho3D::SharedPtr <DiplomacyRequest> >::Iterator iterator = requests_.Begin ();
         iterator != requests_.End (); iterator++)
    {
        DiplomacyRequest *request = iterator->Get ();
        if (request->GetId () == requestId)
        {
            return iterator;
        }
    }
    return requests_.End ();
}

unsigned DiplomacyProcessor::GetFreeDiplomacyRequestId()
{
    unsigned id = 1;
    for (int index = 0; index < requests_.Size (); index++)
    {
        DiplomacyRequest *request = requests_.At (index);
        if (request->GetId () >= id)
        {
            id = request->GetId () + 1;
        }
    }
    return id;
}

void DiplomacyProcessor::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (DiplomacyProcessor, Update));

    if (diplomacyActionsExexutor_)
    {
        delete diplomacyActionsExexutor_;
    }
    diplomacyActionsExexutor_ = new DiplomacyActionsExecutor (scene);
}

DiplomacyProcessor::DiplomacyProcessor (Urho3D::Context *context) : Urho3D::Component (context),
    diplomacyActionsExexutor_ (0),
    requests_ (),
    wars_ ()
{

}

DiplomacyProcessor::~DiplomacyProcessor ()
{
    if (diplomacyActionsExexutor_)
    {
        delete diplomacyActionsExexutor_;
    }
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
    request->SetId (GetFreeDiplomacyRequestId ());
    requests_.Push (Urho3D::SharedPtr <DiplomacyRequest> (request));
}

void DiplomacyProcessor::UpdateDiplomacyRequestPlayerStatus (unsigned requestId, Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status)
{
    Urho3D::Vector <Urho3D::SharedPtr <DiplomacyRequest> >::Iterator requestIterator = GetDiplomacyRequestIteratorById (requestId);
    if (requestIterator != requests_.End ())
    {
        DiplomacyRequest *request = requestIterator->Get ();
        request->UpdatePlayerStatus (diplomacyActionsExexutor_, playerNameHash, status);
        if (request->IsFinished ())
        {
            requests_.Erase (requestIterator);
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
}
