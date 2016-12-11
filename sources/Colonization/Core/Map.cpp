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
    ClearDistricts ();
}

void Map::UpdateDataNode (Urho3D::Node *dataNode, bool rewriteDistrictsPolygons)
{
    assert (dataNode);
    while (dataNode->GetChildren ().Size () < districts_.Size ())
        dataNode->CreateChild ();

    for (int index = 0; index < districts_.Size (); index++)
        if (districts_.At (index)->needDataUpdate_)
        {
            districts_.At (index)->UpdateDataNode (dataNode->GetChildren ().At (index), rewriteDistrictsPolygons);
            districts_.At (index)->needDataUpdate_ = false;
        }
}

void Map::ReadDataFromNode (Urho3D::Node *dataNode)
{
    assert (dataNode);
    ClearDistricts ();
    for (int index = 0; index < dataNode->GetChildren ().Size (); index++)
    {
        District *district = new District (context_);
        district->ReadDataFromNode (dataNode->GetChildren ().At (index).Get ());
        districts_.Push (district);
    }
    UpdateNeighborsOfDistricts ();
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

int Map::GetDistrictsCount ()
{
    return districts_.Size ();
}

void Map::AddDistrict (District *district)
{
    assert (district);
    districts_.Push (district);
    // Because if it called from script, script engine will release ref and delete this object if we don't add ref.
    district->AddRef ();
    UpdateNeighborsOfDistricts ();
}

void Map::UpdateNeighborsOfDistricts ()
{
    for (int index = 0; index < districts_.Size (); index++)
        districts_.At (index)->CalculateNeighbors (districts_);
}

void Map::ClearDistricts()
{
    while (!districts_.Empty ())
    {
        District *district = districts_.At (0);
        districts_.Remove (district);
        delete district;
    }
}

Urho3D::PODVector <District *> Map::FindPath (District *from, District *to, Urho3D::String playerName, bool canGoThroughColonies)
{
    assert (districts_.Contains (from));
    assert (districts_.Contains (to));

    float sailSpeed = context_->GetGlobalVar ("sailSpeed").GetFloat ();
    float marchSpeed = context_->GetGlobalVar ("marchSpeed").GetFloat ();
    float embarkationSpeed = context_->GetGlobalVar ("embarkationSpeed").GetFloat ();
    float disembarkationSpeed = context_->GetGlobalVar ("disembarkationSpeed").GetFloat ();

    Urho3D::HashMap <Urho3D::StringHash, District *> frontier;
    Urho3D::HashMap <Urho3D::StringHash, District *> cameFrom;
    Urho3D::HashMap <Urho3D::StringHash, float> costSoFar;

    frontier [Urho3D::StringHash (1)] = from;
    cameFrom [from->name_] = 0;
    costSoFar [from->name_] = 0.0f;

    while (!frontier.Empty ())
    {
        frontier.Sort ();
        District *current = frontier.Front ().second_;
        frontier.Erase (frontier.Front ().first_);

        if (current == to)
        {
            // Reconstruct way.
            Urho3D::PODVector <District *> reversedWay;
            District *previous = cameFrom [current->name_];
            while (previous)
            {
                reversedWay.Push (previous);
                previous = cameFrom [previous->name_];
            }

            Urho3D::PODVector <District *> way;
            for (int index = reversedWay.Size () - 1; index >= 0; index--)
                way.Push (reversedWay.At (index));
            way.Push (to);
            return way;
        }

        for (int index = 0; index < current->neighbors_.Size (); index++)
        {
            District *next = current->neighbors_.At (index);
            if (!next->isImpassable_ && (
                        next->isSea_ || (canGoThroughColonies && next->hasColony_ && next->colonyOwnerName_ == playerName)))
            {
                float newCost = costSoFar [current->name_];
                float distance = (current->unitPosition_ - next->unitPosition_).Length ();

                if (current->isSea_ && next->isSea_)
                    newCost += (distance / sailSpeed);
                else if (!current->isSea_ && !next->isSea_)
                    newCost += (distance / marchSpeed);
                else if (!current->isSea_ && next->isSea_)
                    newCost += (distance / embarkationSpeed);
                else if (current->isSea_ && !next->isSea_)
                    newCost += (distance / disembarkationSpeed);

                if (!costSoFar.Contains (next->name_) || newCost < costSoFar [next->name_])
                {
                    costSoFar [next->name_] = newCost;
                    int priority = static_cast <int> (1 + HeuristicDistanceForPathFinding (to, next) * 1000);
                    frontier [Urho3D::StringHash (priority)] = next;
                    cameFrom [next->name_] = current;
                }
            }
        }
    }

    // Failure.
    Urho3D::PODVector <District *> empty;
    return empty;
}

float HeuristicDistanceForPathFinding (District *goal, District *next)
{
    return (Urho3D::Abs (goal->unitPosition_.x_ - next->unitPosition_.x_) +
            Urho3D::Abs (goal->unitPosition_.y_ - next->unitPosition_.y_) +
            Urho3D::Abs (goal->unitPosition_.z_ - next->unitPosition_.z_));
}
}
