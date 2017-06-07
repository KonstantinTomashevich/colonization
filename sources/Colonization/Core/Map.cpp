#include <Colonization/BuildConfiguration.hpp>
#include "Map.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/Unit/Unit.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void Map::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (Map, Update));
}

Map::Map (Urho3D::Context *context) : Urho3D::Component (context),
    districts_ ()
{

}

Map::~Map ()
{
    districts_.Clear ();
}

void Map::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    for (int index = 0; index < districts_.Size (); index++)
    {
        districts_.At (index)->DrawDebugGeometry (debug, depthTest);
    }
}

void Map::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <Map> (COLONIZATION_SHARED_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

District *Map::GetDistrictByIndex (unsigned index) const
{
    assert (index < districts_.Size ());
    return districts_.At (index);
}

District *Map::GetDistrictByNameHash (Urho3D::StringHash nameHash) const
{
    for (int index = 0; index < districts_.Size (); index++)
    {
        if (Urho3D::StringHash (districts_.At (index)->GetName ()) == nameHash)
        {
            return districts_.At (index);
        }
    }
    return 0;
}

District *Map::GetDistrictByHash (Urho3D::StringHash hash) const
{
    for (int index = 0; index < districts_.Size (); index++)
    {
        if (districts_.At (index)->GetHash () == hash)
        {
            return districts_.At (index);
        }
    }
    return 0;
}

unsigned Map::GetDistrictsCount () const
{
    return districts_.Size ();
}

Urho3D::PODVector <Urho3D::StringHash> Map::GetColoniesOfPlayer (Urho3D::StringHash playerNameHash)
{
    Urho3D::PODVector <Urho3D::StringHash> colonies;
    for (int index = 0; index < districts_.Size (); index++)
    {
        District *district = districts_.At (index);
        if (district->GetHasColony () && Urho3D::StringHash (district->GetColonyOwnerName ()) == playerNameHash)
        {
            colonies.Push (district->GetHash ());
        }
    }
    return colonies;
}

District *Map::CreateDistrict (Urho3D::String districtName)
{
    assert (node_);
    Urho3D::Node *districtNode = node_->CreateChild (districtName, Urho3D::REPLICATED);
    Urho3D::SharedPtr <District> district (districtNode->CreateComponent <District> (Urho3D::REPLICATED));
    districtNode->AddTag (TAG_DISTRICT);
    district->SetName (districtName);
    districts_.Push (district);
    district->UpdateHash (this);
    return district;
}

void Map::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    // Reload districts array from child nodes.
    UpdateDistrictsList ();
}

void Map::UpdateDistrictsList ()
{
    districts_.Clear ();
    Urho3D::PODVector <Urho3D::Node *> districtsNodes;
    node_->GetChildrenWithComponent (districtsNodes, District::GetTypeStatic ());
    for (int index = 0; index < districtsNodes.Size (); index++)
    {
        Urho3D::Node *districtNode = districtsNodes.At (index);
        if (districtNode->GetID () < Urho3D::FIRST_LOCAL_ID)
        {
            districts_.Push (Urho3D::SharedPtr <District> (districtNode->GetComponent <District> ()));
        }
    }
}

void Map::ClearAndRemoveDistricts ()
{
    assert (node_);
    districts_.Clear ();
    Urho3D::PODVector <Urho3D::Node *> districtsNodes;
    node_->GetChildrenWithComponent (districtsNodes, District::GetTypeStatic ());
    for (int index = 0; index < districtsNodes.Size (); index++)
    {
        node_->RemoveChild (districtsNodes.At (index));
    }
}

void Map::RecalculateDistrictsHashes ()
{
    for (int index = 0; index < districts_.Size (); index++)
    {
        districts_.At (index)->UpdateHash (this);
    }
}

void Map::RecalculateDistrictsNeighbors ()
{
    for (int index = 0; index < districts_.Size (); index++)
    {
        districts_.At (index)->CalculateNeighbors (districts_);
    }
}

Urho3D::PODVector<Urho3D::StringHash> Map::FindPath(const Urho3D::StringHash &targetDistrictHash, Unit *unit, bool setUnitWay) const
{
    assert (unit);
    District *start = GetDistrictByHash (unit->GetPositionHash ());
    District *target = GetDistrictByHash (targetDistrictHash);

    assert (start);
    assert (target);

    Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                        "##############################\n"
                        "Map::FindPath called with arguments:\n"
                        "from: " + start->GetName ()+ "\n"
                        "to: " + target->GetName () + "\n"
                        "playerName: " + unit->GetOwnerPlayerName () + "\n"
                        "unitType: " + Urho3D::String (static_cast <int> (unit->GetUnitType ())) + "\n");

    GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
    assert (configuration);
    float sailSpeed = configuration->GetSailSpeed ();
    float marchSpeed = configuration->GetMarchSpeed ();
    float embarkationSpeed = configuration->GetEmbarkationSpeed ();
    float disembarkationSpeed = configuration->GetDisembarkationSpeed ();

    Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                        "\n"
                        "sailSpeed: " + Urho3D::String (sailSpeed) + "\n"
                        "marchSpeed: " + Urho3D::String (marchSpeed) + "\n"
                        "embarkationSpeed: " + Urho3D::String (embarkationSpeed) + "\n"
                        "disembarkationSpeed: " + Urho3D::String (disembarkationSpeed) + "\n");

    Urho3D::HashMap <Urho3D::StringHash, District *> frontier;
    Urho3D::HashMap <Urho3D::StringHash, District *> cameFrom;
    Urho3D::HashMap <Urho3D::StringHash, float> costSoFar;

    frontier [Urho3D::StringHash (1)] = start;
    cameFrom [start->GetName ()] = 0;
    costSoFar [start->GetName ()] = 0.0f;

    while (!frontier.Empty ())
    {
        Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                            "\n"
                            "frontierArraySize: " + Urho3D::String (frontier.Size ()) + "\n");

        frontier.Sort ();
        District *current = frontier.Front ().second_;
        int currentPriority = frontier.Front ().first_.Value ();
        frontier.Erase (frontier.Front ().first_);

        Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                            "\n"
                            "frontierPriority: " + Urho3D::String (currentPriority) + "\n"
                            "frontier: " + current->GetName () + "\n");

        if (current == target)
        {
            // Reconstruct way.
            Urho3D::PODVector <Urho3D::StringHash> reversedWay;
            District *previous = cameFrom [current->GetName ()];
            while (previous)
            {
                reversedWay.Push (previous->GetHash ());
                previous = cameFrom [previous->GetName ()];
            }

            Urho3D::PODVector <Urho3D::StringHash> way;
            for (int index = reversedWay.Size () - 1; index >= 0; index--)
            {
                way.Push (reversedWay.At (index));
            }
            way.Push (target->GetHash ());
            if (setUnitWay)
            {
                unit->SetWay (way);
            }
            return way;
        }

        Urho3D::PODVector <Urho3D::StringHash> neighbors = current->GetNeighborsHashes ();
        for (int index = 0; index < neighbors.Size (); index++)
        {
            District *next = GetDistrictByHash (neighbors.At (index));
            Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                                "\n"
                                "listingNeighbor (index): " + Urho3D::String (index) + "\n"
                                "listingNeighbor (name): " + next->GetName () + "\n"
                                "isImpassable: " + Urho3D::String (next->GetIsImpassable ()) + "\n"
                                "isSea: " + Urho3D::String (next->GetIsSea ()) + "\n"
                                "hasColony: " + Urho3D::String (next->GetHasColony ()) + "\n"
                                "colonyOwnerName: " + next->GetColonyOwnerName () + "\n");

            if (unit->IsCanGoTo (next, this, current->GetHash ()))
            {
                Urho3D::Log::Write (Urho3D::LOG_DEBUG, "Can go through this neighbor.");

                float newCost = costSoFar [current->GetName ()];
                float distance = (current->GetUnitPosition () - next->GetUnitPosition ()).Length ();

                if (current->GetIsSea () && next->GetIsSea ())
                {
                    newCost += (distance / sailSpeed);
                }
                else if (!current->GetIsSea () && !next->GetIsSea ())
                {
                    newCost += (distance / marchSpeed);
                }
                else if (!current->GetIsSea () && next->GetIsSea ())
                {
                    newCost += (distance / embarkationSpeed);
                }
                else if (current->GetIsSea () && !next->GetIsSea ())
                {
                    newCost += (distance / disembarkationSpeed);
                }

                Urho3D::Log::Write (Urho3D::LOG_DEBUG, "Cost: " + Urho3D::String (newCost));

                if (!costSoFar.Contains (next->GetName ()) || newCost < costSoFar [next->GetName ()])
                {
                    int priority = 1 + static_cast <int> (HeuristicDistanceForPathFinding (target, next) * 1000);
                    while (frontier [Urho3D::StringHash (priority)] && frontier [Urho3D::StringHash (priority)] != next)
                    {
                        priority++;
                    }

                    costSoFar [next->GetName ()] = newCost;
                    frontier [Urho3D::StringHash (priority)] = next;
                    cameFrom [next->GetName ()] = current;

                    Urho3D::Log::Write (Urho3D::LOG_DEBUG, "Priority: " + Urho3D::String (priority) + "\n"
                                        "Setted as lowerest cost to this district.\n");
                }

                Urho3D::Log::Write (Urho3D::LOG_DEBUG, "");
            }
        }
    }

    // Failure.
    Urho3D::PODVector <Urho3D::StringHash> empty;
    if (setUnitWay)
    {
        unit->SetWay (empty);
    }
    return empty;
}

float HeuristicDistanceForPathFinding (District *goal, District *next)
{
    return (Urho3D::Abs (goal->GetUnitPosition ().x_ - next->GetUnitPosition ().x_) +
            Urho3D::Abs (goal->GetUnitPosition ().y_ - next->GetUnitPosition ().y_) +
            Urho3D::Abs (goal->GetUnitPosition ().z_ - next->GetUnitPosition ().z_));
}
}
