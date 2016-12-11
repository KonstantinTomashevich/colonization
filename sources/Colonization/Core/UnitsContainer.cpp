#include <Colonization/BuildConfiguration.hpp>
#include "UnitsContainer.hpp"

namespace Colonization
{
UnitsContainer::UnitsContainer (Urho3D::Context *context) : Urho3D::Object (context),
    units_ ()
{

}

UnitsContainer::~UnitsContainer ()
{
    RemoveAndDeleteAllUnits ();
}

void UnitsContainer::UpdateDataNode (Urho3D::Node *dataNode)
{
    assert (dataNode);
    while (dataNode->GetChildren ().Size () < units_.Size ())
        dataNode->CreateChild ();

    while (dataNode->GetChildren ().Size () > units_.Size ())
        dataNode->RemoveChild (dataNode->GetChildren ().Back ());

    for (int index = 0; index < units_.Size (); index++)
    {
        Unit *unit = units_.At (index);
        unit->UpdateDataNode (dataNode->GetChildren ().At (index));
    }
}

void UnitsContainer::ReadDataFromNode (Urho3D::Node *dataNode, Map *map)
{
    assert (dataNode);
    RemoveAndDeleteAllUnits ();
    Urho3D::PODVector <Urho3D::Node *> unitsNodes;
    dataNode->GetChildren (unitsNodes, false);

    for (int index = 0; index < unitsNodes.Size (); index++)
    {
        Urho3D::Node *node = unitsNodes.At (index);
        Unit *unit = 0;
        UnitType unitType = static_cast <UnitType> (node->GetVar ("unitType").GetInt ());

        if (unitType == UNIT_FLEET)
            unit = new FleetUnit (context_);
        else if (unitType == UNIT_TRADERS)
            unit = new TradersUnit (context_);
        else if (unitType == UNIT_COLONIZATORS)
            unit = new ColonizatorsUnit (context_);
        else if (unitType == UNIT_ARMY)
            unit = new ArmyUnit (context_);

        unit->ReadDataFromNode (node, map);
        units_.Push (unit);
    }
}

int UnitsContainer::GetUnitsCount ()
{
    return units_.Size ();
}

Unit *UnitsContainer::GetUnitByIndex (int index)
{
    assert (index < units_.Size ());
    return units_.At (index);
}

Urho3D::PODVector <Unit *> UnitsContainer::GetUnitsOfPlayer (Urho3D::StringHash playerNameHash)
{
    Urho3D::PODVector <Unit *> unitsOfPlayer;
    for (int index = 0; index < units_.Size (); index++)
    {
        Unit *unit = units_.At (index);
        if (Urho3D::StringHash (unit->ownerPlayer_) == playerNameHash)
            unitsOfPlayer.Push (unit);
    }
    return unitsOfPlayer;
}

void UnitsContainer::AddUnit (Unit *unit)
{
    units_.Push (unit);
    // Because if it called from script, script engine will release ref and delete this object if we don't add ref.
    unit->AddRef ();
}

bool UnitsContainer::RemoveAndDeleteUnit (Unit *unit)
{
    bool result = units_.Remove (unit);
    delete unit;
    return result;
}

void UnitsContainer::RemoveAndDeleteAllUnits ()
{
    while (!units_.Empty ())
    {
        Unit *unit = units_.At (0);
        units_.Remove (unit);
        delete unit;
    }
}
}
