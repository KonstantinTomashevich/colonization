#include <Colonization/BuildConfiguration.hpp>
#include "FogOfWarCalculator.hpp"

#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Unit.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
void FogOfWarCalculator::OpenDistrictAndNeighbors (District *district)
{
    fogOfWarMap_ [district->GetHash ()] = true;
    Urho3D::PODVector <Urho3D::StringHash> neighbors = district->GetNeighborsHashes ();
    for (int index = 0; index < neighbors.Size (); index++)
        fogOfWarMap_ [neighbors.At (index)] = true;
}

void FogOfWarCalculator::OnSceneSet (Urho3D::Scene *scene)
{
    //UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (FogOfWarCalculator, Update));
}

FogOfWarCalculator::FogOfWarCalculator (Urho3D::Context *context) : Urho3D::Component (context),
    playerNameHash_ (),
    fogOfWarMap_ ()
{

}

FogOfWarCalculator::~FogOfWarCalculator ()
{

}

void FogOfWarCalculator::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <FogOfWarCalculator> (COLONIZATION_CLIENT_ONLY_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Player Name Hash", GetPlayerNameHash, SetPlayerNameHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
}

void FogOfWarCalculator::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        fogOfWarMap_.Clear ();
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        assert (map);

        Urho3D::PODVector <District *> districtsToOpen;
        for (int index = 0; index < map->GetDistrictsCount (); index++)
        {
            District *district = map->GetDistrictByIndex (index);
            if (district->HasColony () && Urho3D::StringHash (district->GetColonyOwnerName ()) == playerNameHash_)
                districtsToOpen.Push (district);
            else
                fogOfWarMap_ [district->GetHash ()] = false;
        }

        Urho3D::Node *unitsContainerNode = node_->GetScene ()->GetChild ("units");
        if (unitsContainerNode)
        {
            Urho3D::PODVector <Urho3D::Node *> unitsNodes;
            unitsContainerNode->GetChildrenWithComponent (unitsNodes, Unit::GetTypeStatic ());
            for (int index = 0; index < unitsNodes.Size (); index++)
            {
                Unit *unit = unitsNodes.At (index)->GetComponent <Unit> ();
                District *position = map->GetDistrictByHash (unit->GetPositionHash ());
                assert (position);
                if (!districtsToOpen.Contains (position))
                    districtsToOpen.Push (position);
            }
        }

        for (int index = 0; index < districtsToOpen.Size (); index++)
            OpenDistrictAndNeighbors (districtsToOpen.At (index));
    }
}

Urho3D::StringHash FogOfWarCalculator::GetPlayerNameHash () const
{
    return playerNameHash_;
}

void FogOfWarCalculator::SetPlayerNameHash (const Urho3D::StringHash &playerNameHash)
{
    playerNameHash_ = playerNameHash;
}

bool FogOfWarCalculator::IsDistrictVisible (Urho3D::StringHash districtHash)
{
    return fogOfWarMap_ [districtHash];
}

Urho3D::HashMap<Urho3D::StringHash, bool> FogOfWarCalculator::GetFogOfWarMap ()
{
    return fogOfWarMap_;
}
}

