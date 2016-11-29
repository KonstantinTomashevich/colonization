#include <Colonization/BuildConfiguration.hpp>
#include "Map.hpp"

namespace Colonization
{
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

Map::Map (Urho3D::Context *context) : Urho3D::Object (context), districts_ ()
{

}

Map::~Map ()
{
    ClearDistricts ();
}

void Map::UpdateDataNode (Urho3D::Node *dataNode, bool rewriteDistrictsPolygons)
{
    dataNode->SetName ("Map");
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

int Map::GetDistrictsCount ()
{
    return districts_.Size ();
}

void Map::AddDistrict (District *district)
{
    assert (district);
    districts_.Push (district);
    UpdateNeighborsOfDistricts ();
}
}
