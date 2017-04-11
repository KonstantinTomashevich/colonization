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
        UpdateWarsList ();
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
