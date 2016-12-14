#pragma once
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Container/Vector.h>
#include <Colonization/Core/District.hpp>

namespace Colonization
{
float HeuristicDistanceForPathFinding (District *goal, District *next);

class Map : public Urho3D::Object
{
URHO3D_OBJECT (Map, Object)
protected:
    Urho3D::PODVector <District *> districts_;

public:
    Map (Urho3D::Context *context);
    virtual ~Map ();

    void UpdateDataNode (Urho3D::Node *dataNode, bool rewriteDistrictsPolygons);
    void ReadDataFromNode (Urho3D::Node *dataNode);

    District *GetDistrictByIndex (int index);
    District *GetDistrictByNameHash (Urho3D::StringHash nameHash);
    District *GetDistrictByHash (Urho3D::StringHash hash);
    int GetDistrictsCount ();
    void AddDistrict (District *district);

    void UpdateNeighborsOfDistricts ();
    void ClearDistricts ();
    Urho3D::PODVector <District *> FindPath(District *from, District *to,
                                            Urho3D::String playerName, bool canGoThroughColonies, bool isColonizator);
};
}
