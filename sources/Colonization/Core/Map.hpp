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
    static void RegisterObject (Urho3D::Context *context);

    District *GetDistrictByIndex (int index);
    District *GetDistrictByNameHash (Urho3D::StringHash nameHash);
    District *GetDistrictByHash (Urho3D::StringHash hash);
    int GetDistrictsCount ();
    District *CreateDistrict (Urho3D::String districtName);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void RecalculateDistrictsNeighbors ();
    void ClearAndRemoveDistricts ();
    Urho3D::Vector <Urho3D::SharedPtr<District> > FindPath (
            Urho3D::StringHash startDistrictHash, Urho3D::StringHash targetDistrictHash,
            Urho3D::String playerName, bool canGoThroughColonies, bool isColonizator);
};
}
