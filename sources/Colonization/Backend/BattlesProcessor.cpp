#include <Colonization/BuildConfiguration.hpp>
#include "BattlesProcessor.hpp"
#include <Urho3D/Core/Context.h>

#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{

void BattlesProcessor::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (BattlesProcessor, Update));
}

BattlesProcessor::BattlesProcessor (Urho3D::Context *context) : Urho3D::Component (context),
    battles_ ()
{

}

BattlesProcessor::~BattlesProcessor ()
{

}

void BattlesProcessor::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <BattlesProcessor> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void BattlesProcessor::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{

}

int BattlesProcessor::GetBattlesCount () const
{
    return battles_.Size ();
}

void BattlesProcessor::UpdateBattlesList ()
{
    // Reload battles array from child nodes.
    assert (node_);
    battles_.Clear ();
    Urho3D::PODVector <Urho3D::Node *> battlesNodes;
    node_->GetChildrenWithTag (battlesNodes, TAG_BATTLE);
    for (int index = 0; index < battlesNodes.Size (); index++)
    {
        Urho3D::Node *battleNode = battlesNodes.At (index);
        if (battleNode->GetID () < Urho3D::FIRST_LOCAL_ID)
        {
            Battle *battle = battleNode->GetComponent <Battle> ();
            if (battle)
            {
                battles_.Push (Urho3D::SharedPtr <Battle> (battle));
            }
        }
    }
}

Battle *BattlesProcessor::GetBattleByIndex (int index) const
{
    assert (index < battles_.Size ());
    return battles_.At (index);
}

Battle *BattlesProcessor::GetBattleByHash (Urho3D::StringHash battleHash) const
{
    for (int index = 0; index < battles_.Size (); index++)
    {
        Battle *battle = battles_.At (index);
        if (battle->GetHash () == battleHash)
        {
            return battle;
        }
    }
    return 0;
}

Urho3D::PODVector <Urho3D::StringHash> BattlesProcessor::GetCurrentBattlesOfWar (Urho3D::StringHash warHash) const
{
    Urho3D::PODVector <Urho3D::StringHash> currentBattlesOfWar;
    for (int index = 0; index < battles_.Size (); index++)
    {
        Battle *battle = battles_.At (index);
        if (battle->GetWarHash () == warHash)
        {
            currentBattlesOfWar.Push (battle->GetHash ());
        }
    }
    return currentBattlesOfWar;
}
}
