#include <Colonization/BuildConfiguration.hpp>
#include "FogOfWarCalculator.hpp"

#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Unit/Unit.hpp>
#include <Colonization/Core/Unit/UnitTags.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void FogOfWarCalculator::OpenDistrictAndNeighbors (District *district)
{
    fogOfWarMap_ [district->GetHash ()] = true;
    Urho3D::PODVector <Urho3D::StringHash> neighbors = district->GetNeighborsHashes ();
    for (int index = 0; index < neighbors.Size (); index++)
    {
        fogOfWarMap_ [neighbors.At (index)] = true;
    }
}

void FogOfWarCalculator::OnSceneSet (Urho3D::Scene *scene)
{
    //UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (FogOfWarCalculator, Update));
}

FogOfWarCalculator::FogOfWarCalculator (Urho3D::Context *context) : Urho3D::Component (context),
    playerName_ (),
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
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Player Name", GetPlayerName, SetPlayerName, Urho3D::String, Urho3D::String::EMPTY, Urho3D::AM_DEFAULT);
}

void FogOfWarCalculator::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Urho3D::StringHash playerNameHash = Urho3D::StringHash (playerName_);
        fogOfWarMap_.Clear ();
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        assert (map);

        Urho3D::PODVector <District *> districtsToOpen;
        for (int index = 0; index < map->GetDistrictsCount (); index++)
        {
            District *district = map->GetDistrictByIndex (index);
            if (district->GetHasColony () && Urho3D::StringHash (district->GetColonyOwnerName ()) == playerNameHash)
            {
                districtsToOpen.Push (district);
            }
            else
            {
                fogOfWarMap_ [district->GetHash ()] = false;
            }
        }

        Urho3D::Node *unitsContainerNode = node_->GetScene ()->GetChild ("units");
        if (unitsContainerNode)
        {
            Urho3D::PODVector <Urho3D::Node *> unitsNodes;
            unitsContainerNode->GetChildrenWithTag (unitsNodes, TAG_UNIT);
            for (int index = 0; index < unitsNodes.Size (); index++)
            {
                Unit *unit = unitsNodes.At (index)->GetDerivedComponent <Unit> ();
                if (unit && Urho3D::StringHash (unit->GetOwnerPlayerName ()) == playerNameHash)
                {
                    District *position = map->GetDistrictByHash (unit->GetPositionHash ());
                    assert (position);
                    if (!districtsToOpen.Contains (position))
                    {
                        districtsToOpen.Push (position);
                    }
                }
            }
        }

        for (int index = 0; index < districtsToOpen.Size (); index++)
        {
            OpenDistrictAndNeighbors (districtsToOpen.At (index));
        }
    }
}

Urho3D::String FogOfWarCalculator::GetPlayerName () const
{
    return playerName_;
}

void FogOfWarCalculator::SetPlayerName (const Urho3D::String &playerName)
{
    playerName_ = playerName;
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

