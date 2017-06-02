#pragma once
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Container/Vector.h>
#include <Colonization/Core/District/District.hpp>

namespace Colonization
{
class Unit;
//@ASBindGen Function
float HeuristicDistanceForPathFinding (District *goal, District *next);
//@ASBindGen Constant
const Urho3D::String TAG_DISTRICT ("District");

//@ASBindGen Class ObjectType=Ref
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

    //@ASBindGen Function AddRef_arg-1
    District *GetDistrictByIndex (unsigned index) const;
    //@ASBindGen Function AddRef_arg-1
    District *GetDistrictByNameHash (Urho3D::StringHash nameHash) const;
    //@ASBindGen Function AddRef_arg-1
    District *GetDistrictByHash (Urho3D::StringHash hash) const;

    //@ASBindGen Function OverrideName=get_districtsCount
    unsigned GetDistrictsCount () const;
    //@ASBindGen Function
    Urho3D::PODVector <Urho3D::StringHash> GetColoniesOfPlayer (Urho3D::StringHash playerNameHash);
    //@ASBindGen Function AddRef_arg-1
    District *CreateDistrict (Urho3D::String districtName);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    //@ASBindGen Function
    void UpdateDistrictsList ();
    //@ASBindGen Function
    void ClearAndRemoveDistricts ();

    //@ASBindGen Function
    void RecalculateDistrictsHashes ();
    //@ASBindGen Function
    void RecalculateDistrictsNeighbors ();
    //@ASBindGen Function
    Urho3D::PODVector <Urho3D::StringHash> FindPath (const Urho3D::StringHash &targetDistrictHash, Unit *unit,
                                                     bool setUnitWay = true) const;
};
}
