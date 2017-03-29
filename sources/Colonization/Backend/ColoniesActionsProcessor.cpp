#include <Colonization/BuildConfiguration.hpp>
#include "ColoniesActionsProcessor.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void ColoniesActionsProcessor::ProcessColonyActions (GameConfiguration *configuration, District *colony, float timeStep)
{

}

void ColoniesActionsProcessor::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (ColoniesActionsProcessor, Update));
}

ColoniesActionsProcessor::ColoniesActionsProcessor (Urho3D::Context *context) : Urho3D::Component (context)
{

}

ColoniesActionsProcessor::~ColoniesActionsProcessor ()
{

}

void ColoniesActionsProcessor::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <ColoniesActionsProcessor> (COLONIZATION_SERVER_ONLY_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void ColoniesActionsProcessor::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();

        assert (map);
        assert (configuration);
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();

        for (int index = 0; index < map->GetDistrictsCount (); index++)
        {
            District *district = map->GetDistrictByIndex (index);
            if (district->HasColony () && district->GetColonyActionsCount () > 0)
            {
                ProcessColonyActions (configuration, district, timeStep);
            }
        }
    }
}
}

