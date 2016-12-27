#include <Colonization/BuildConfiguration.hpp>
#include "Map.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Core/Context.h>

namespace Colonization
{
Map::Map (Urho3D::Context *context) : Urho3D::Object (context), districts_ ()
{

}

Map::~Map ()
{
    districts_.Clear ();
}

void Map::UpdateDataNode (Urho3D::Node *dataNode, bool rewriteDistrictsPolygons)
{
    assert (dataNode);
    while (dataNode->GetChildren ().Size () < districts_.Size ())
        dataNode->CreateChild ();

    while (dataNode->GetChildren ().Size () > districts_.Size ())
        dataNode->RemoveChild (dataNode->GetChildren ().Back ());

    for (int index = 0; index < districts_.Size (); index++)
        if (districts_.At (index)->needDataUpdate_)
        {
            District *district = districts_.At (index);
            Urho3D::Node *node = dataNode->GetChildren ().At (index);
            node->SetName (district->name_);
            district->UpdateDataNode (node, rewriteDistrictsPolygons);
            district->needDataUpdate_ = false;
        }
}

void Map::ReadDataFromNode (Urho3D::Node *dataNode)
{
    assert (dataNode);
    districts_.Clear ();
    for (int index = 0; index < dataNode->GetChildren ().Size (); index++)
    {
        District *district = new District (context_);
        district->ReadDataFromNode (dataNode->GetChildren ().At (index).Get ());
        districts_.Push (district);
    }
    RecalculateDistrictsNeighbors ();
}

District *Map::GetDistrictByIndex (int index)
{
    assert (index < districts_.Size ());
    return districts_.At (index);
}

District *Map::GetDistrictByNameHash (Urho3D::StringHash nameHash)
{
    for (int index = 0; index < districts_.Size (); index++)
        if (Urho3D::StringHash (districts_.At (index)->name_) == nameHash)
            return districts_.At (index);
    return 0;
}

District *Map::GetDistrictByHash (Urho3D::StringHash hash)
{
    for (int index = 0; index < districts_.Size (); index++)
        if (districts_.At (index)->GetHash () == hash)
            return districts_.At (index);
    return 0;
}

int Map::GetDistrictsCount ()
{
    return districts_.Size ();
}

void Map::AddDistrict (Urho3D::SharedPtr <District> district)
{
    assert (district.NotNull ());
    districts_.Push (district);
    RecalculateDistrictsNeighbors ();
}

void Map::RecalculateDistrictsNeighbors ()
{
    for (int index = 0; index < districts_.Size (); index++)
        districts_.At (index)->CalculateNeighbors (districts_);
}

void Map::ClearDistricts ()
{
    districts_.Clear ();
}

Urho3D::PODVector <Urho3D::SharedPtr <District> > Map::FindPath (
        Urho3D::StringHash startDistrictHash, Urho3D::StringHash targetDistrictHash,
        Urho3D::String playerName, bool canGoThroughColonies, bool isColonizator)
{
    Urho3D::SharedPtr <District> start = GetDistrictByHash (startDistrictHash);
    Urho3D::SharedPtr <District> target = GetDistrictByHash (targetDistrictHash);

    assert (start.NotNull ());
    assert (target.NotNull ());

    Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                        "##############################\n"
                        "Map::FindPath called with arguments:\n"
                        "from: " + start->GetName ()+ "\n"
                        "to: " + target->GetName () + "\n"
                        "playerName: " + playerName + "\n"
                        "canGoTroughColonies: " + Urho3D::String (canGoThroughColonies) + "\n"
                        "isColonizator: " + Urho3D::String (isColonizator) + "\n");

    float sailSpeed = context_->GetGlobalVar ("sailSpeed").GetFloat ();
    float marchSpeed = context_->GetGlobalVar ("marchSpeed").GetFloat ();
    float embarkationSpeed = context_->GetGlobalVar ("embarkationSpeed").GetFloat ();
    float disembarkationSpeed = context_->GetGlobalVar ("disembarkationSpeed").GetFloat ();

    Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                        "\n"
                        "sailSpeed: " + Urho3D::String (sailSpeed) + "\n"
                        "marchSpeed: " + Urho3D::String (marchSpeed) + "\n"
                        "embarkationSpeed: " + Urho3D::String (embarkationSpeed) + "\n"
                        "disembarkationSpeed: " + Urho3D::String (disembarkationSpeed) + "\n");

    Urho3D::HashMap <Urho3D::StringHash, District *> frontier;
    Urho3D::HashMap <Urho3D::StringHash, District *> cameFrom;
    Urho3D::HashMap <Urho3D::StringHash, float> costSoFar;

    frontier [Urho3D::StringHash (1)] = start.Get ();
    cameFrom [from->GetName ()] = 0;
    costSoFar [from->GetName ()] = 0.0f;

    while (!frontier.Empty ())
    {
        Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                            "\n"
                            "frontierArraySize: " + Urho3D::String (frontier.Values ().Size ()) + "\n");

        frontier.Sort ();
        District *current = frontier.Values ().At (0);
        int currentPriority = frontier.Keys ().At (0).Value ();
        frontier.Erase (frontier.Keys ().At (0));

        Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                            "\n"
                            "frontierPriority: " + Urho3D::String (currentPriority) + "\n"
                            "frontier: " + current->GetName () + "\n");

        if (current == target.Get ())
        {
            // Reconstruct way.
            Urho3D::PODVector <District *> reversedWay;
            District *previous = cameFrom [current->name_];
            while (previous)
            {
                reversedWay.Push (previous);
                previous = cameFrom [previous->name_];
            }

            Urho3D::PODVector <Urho3D::SharedPtr <District> > way;
            for (int index = reversedWay.Size () - 1; index >= 0; index--)
                way.Push (Urho3D::SharedPtr <District> (reversedWay.At (index)));
            way.Push (target);
            return way;
        }

        for (int index = 0; index < current->neighbors_.Size (); index++)
        {
            District *next = current->neighbors_.At (index);
            Urho3D::Log::Write (Urho3D::LOG_DEBUG,
                                "\n"
                                "listingNeighbor (index): " + Urho3D::String (index) + "\n"
                                "listingNeighbor (name): " + next->name_ + "\n"
                                "isImpassable: " + Urho3D::String (next->isImpassable_) + "\n"
                                "isSea: " + Urho3D::String (next->isSea_) + "\n"
                                "hasColony: " + Urho3D::String (next->hasColony_) + "\n"
                                "colonyOwnerName: " + next->colonyOwnerName_ + "\n");

            if (!next->IsImpassable () && (
                        next->IsSea () || (canGoThroughColonies && next->HasColony () && next->GetColonyOwnerName () == playerName) ||
                        (isColonizator && next == target.Get ())))
            {
                Urho3D::Log::Write (Urho3D::LOG_DEBUG, "Can go through this neighbor.");

                float newCost = costSoFar [current->name_];
                float distance = (current->unitPosition_ - next->unitPosition_).Length ();

                if (current->IsSea () && next->IsSea ())
                    newCost += (distance / sailSpeed);
                else if (!current->IsSea () && !next->IsSea ())
                    newCost += (distance / marchSpeed);
                else if (!current->IsSea () && next->IsSea ())
                    newCost += (distance / embarkationSpeed);
                else if (current->IsSea () && !next->IsSea ())
                    newCost += (distance / disembarkationSpeed);

                Urho3D::Log::Write (Urho3D::LOG_DEBUG, "Cost: " + Urho3D::String (newCost));


                if (!costSoFar.Contains (next->name_) || newCost < costSoFar [next->name_])
                {
                    int priority = 1 + static_cast <int> (HeuristicDistanceForPathFinding (to, next) * 1000);
                    while (frontier [Urho3D::StringHash (priority)] && frontier [Urho3D::StringHash (priority)] != next)
                        priority++;

                    costSoFar [next->name_] = newCost;
                    frontier [Urho3D::StringHash (priority)] = next;
                    cameFrom [next->name_] = current;

                    Urho3D::Log::Write (Urho3D::LOG_DEBUG, "Priority: " + Urho3D::String (priority) + "\n"
                                        "Setted as lowerest cost to this district.\n");
                }

                Urho3D::Log::Write (Urho3D::LOG_DEBUG, "");
            }
        }
    }

    // Failure.
    Urho3D::PODVector <Urho3D::SharedPtr <District> > empty;
    return empty;
}

float HeuristicDistanceForPathFinding (District *goal, District *next)
{
    return (Urho3D::Abs (goal->GetUnitPosition ().x_ - next->GetUnitPosition ().x_) +
            Urho3D::Abs (goal->GetUnitPosition ().y_ - next->GetUnitPosition ().y_) +
            Urho3D::Abs (goal->GetUnitPosition ().z_ - next->GetUnitPosition ().z_));
}
}
