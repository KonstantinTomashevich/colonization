#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyProcessor.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/Player/Player.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyRequestsUtils.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyEvents.hpp>
#include <Colonization/Backend/PlayersManager.hpp>

#include <Colonization/Core/Diplomacy/DiplomacyTags.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
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
        // TODO: Process wars update points.
    }
}

void DiplomacyProcessor::HandlePlayerWillBeDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Player *player = static_cast <Player *> (eventData [PlayerWillBeDisconnected::PLAYER].GetPtr ());
    Urho3D::StringHash playerNameHash (player->GetName ());
    for (int index = 0; index < wars_.Size (); index++)
    {
        DiplomacyWar *war = wars_.At (index);
        if (war->RemoveAttackerNameHash (playerNameHash) || war->RemoveDefenderNameHash (playerNameHash))
        {
            if (war->GetAttackersCount () > 0)
            {
                for (int attackerIndex = 0; attackerIndex < war->GetAttackersCount (); attackerIndex++)
                {
                    DiplomacyRequestsUtils::UpdatePlayerEnemies (node_->GetScene (), war->GetAttackerNameHashByIndex (attackerIndex));
                }
            }

            if (war->GetDefendersCount () > 0)
            {
                for (int defenderIndex = 0; defenderIndex < war->GetDefendersCount (); defenderIndex++)
                {
                    DiplomacyRequestsUtils::UpdatePlayerEnemies (node_->GetScene (), war->GetDefenderNameHashByIndex (defenderIndex));
                }
            }

            if (war->GetAttackersCount () == 0 || war->GetDefendersCount () == 0)
            {
                RemoveWarByPtr (war);
            }
        }
    }
}

void DiplomacyProcessor::AddDiplomacyRequest (DiplomacyRequest *request)
{
    request->SetRequestId (GetFreeDiplomacyRequestId ());
    Urho3D::Node *requestNode = GetOrCreateRequestsNode ()->
            CreateChild ("Request" + Urho3D::String (request->GetRequestId ()), Urho3D::LOCAL);
    requestNode->AddComponent (request, requestNode->GetID () + 1, Urho3D::LOCAL);
    requestNode->AddTag (TAG_DIPLOMACY_REQUEST);
    requestNode->AddTag (request->GetRequestTypeTag ());
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
    warNode->AddTag (TAG_DIPLOMACY_RELATION);
    warNode->AddTag (TAG_WAR);

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

bool DiplomacyProcessor::RemoveWarByPtr (DiplomacyWar *war)
{
    assert (war);
    Urho3D::VariantMap eventData;
    eventData [WarEnded::WAR] = Urho3D::Variant (war);
    SendEvent (EVENT_WAR_ENDED, eventData);

    bool result = wars_.Remove (Urho3D::SharedPtr <DiplomacyWar> (war));
    war->GetNode ()->Remove ();
    return result;
}

bool DiplomacyProcessor::RemoveWarByHash (Urho3D::StringHash hash)
{
    Urho3D::SharedPtr <DiplomacyWar> war (GetWarByHash (hash));
    if (war.NotNull ())
    {
        RemoveWarByPtr (war.Get ());
    }
    else
    {
        return false;
    }
}

Urho3D::PODVector <Urho3D::StringHash> DiplomacyProcessor::GetWarsOfPlayer (Urho3D::StringHash playerNameHash)
{
    Urho3D::PODVector <Urho3D::StringHash> warsOfPlayer;
    for (int index = 0; index < wars_.Size (); index++)
    {
        DiplomacyWar *war = wars_.At (index);
        if (war->IsAttacker (playerNameHash) || war->IsDefender (playerNameHash))
        {
            warsOfPlayer.Push (war->GetHash ());
        }
    }
    return warsOfPlayer;
}

void DiplomacyProcessor::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (DiplomacyProcessor, Update));
    SubscribeToEvent (EVENT_PLAYER_WILL_BE_DISCONNECTED, URHO3D_HANDLER (DiplomacyProcessor, HandlePlayerWillBeDisconnected));
}

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
}
