#pragma once
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Container/Vector.h>
#include <Colonization/Core/District.hpp>

namespace Colonization
{
class Map : public Urho3D::Object
{
URHO3D_OBJECT (Map, Object)
protected:
    Urho3D::Vector <District *> districts_;
    void UpdateNeighborsOfDistricts ();
    void ClearDistricts ();

public:
    Map (Urho3D::Context *context);
    virtual ~Map ();

    void UpdateDataNode (Urho3D::Node *dataNode, bool rewriteDistrictsPolygons = false);
    void ReadDataFromNode (Urho3D::Node *dataNode);

    District *GetDistrictByIndex (int index);
    int GetDistrictsCount ();
    void AddDistrict (District *district);
};
}
