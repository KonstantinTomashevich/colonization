#pragma once
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Container/Vector.h>
#include <Colonization/Core/District.hpp>

namespace Colonization
{
class Unit;
float HeuristicDistanceForPathFinding (District *goal, District *next);

class Map : public Urho3D::Component
{
URHO3D_OBJECT (Map, Component)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <District> > districts_;
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    Map (Urho3D::Context *context);
    virtual ~Map ();

    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);
    static void RegisterObject (Urho3D::Context *context);

    District *GetDistrictByIndex (unsigned index) const;
    District *GetDistrictByNameHash (Urho3D::StringHash nameHash) const;
    District *GetDistrictByHash (Urho3D::StringHash hash) const;
    unsigned GetDistrictsCount () const;
    Urho3D::PODVector <Urho3D::StringHash> GetColoniesOfPlayer (Urho3D::StringHash playerNameHash);
    District *CreateDistrict (Urho3D::String districtName);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void UpdateDistrictsList ();
    void ClearAndRemoveDistricts ();

    void RecalculateDistrictsHashes ();
    void RecalculateDistrictsNeighbors ();
    Urho3D::PODVector <Urho3D::StringHash> FindPath (const Urho3D::StringHash &targetDistrictHash, Unit *unit,
                                                     bool setUnitWay = true) const;
};
}
