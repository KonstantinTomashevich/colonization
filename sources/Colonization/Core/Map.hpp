#pragma once
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Container/Vector.h>
#include <Colonization/Core/District.hpp>

namespace Colonization
{
float HeuristicDistanceForPathFinding (District *goal, District *next);

class Map : public Urho3D::Component
{
URHO3D_OBJECT (Map, Component)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <District> > districts_;

public:
    Map (Urho3D::Context *context);
    virtual ~Map ();

    void UpdateDataNode (Urho3D::Node *dataNode, bool rewriteDistrictsPolygons);
    void ReadDataFromNode (Urho3D::Node *dataNode);

    District *GetDistrictByIndex(int index);
    District *GetDistrictByNameHash(Urho3D::StringHash nameHash);
    District *GetDistrictByHash (Urho3D::StringHash hash);
    int GetDistrictsCount ();
    void AddDistrict (Urho3D::SharedPtr<District> district);

    void RecalculateDistrictsNeighbors ();
    void ClearDistricts ();
    Urho3D::PODVector < Urho3D::SharedPtr<District> > FindPath(
            Urho3D::StringHash startDistrictHash, Urho3D::StringHash targetDistrictHash,
            Urho3D::String playerName, bool canGoThroughColonies, bool isColonizator);
};
}
