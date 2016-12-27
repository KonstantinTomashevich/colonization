#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Node.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Unit.hpp>

namespace Colonization
{
class UnitsContainer : public Urho3D::Object
{
URHO3D_OBJECT (UnitsContainer, Object)
protected:
    Urho3D::PODVector <Unit *> units_;

public:
    UnitsContainer (Urho3D::Context *context);
    virtual ~UnitsContainer ();

    void UpdateDataNode (Urho3D::Node *dataNode);
    void ReadDataFromNode (Urho3D::Node *dataNode, Map *map);

    int GetUnitsCount ();
    Unit *GetUnitByIndex (int index);
    Unit *GetUnitByHash (Urho3D::StringHash hash);
    Urho3D::PODVector <Unit *> GetUnitsOfPlayer (Urho3D::StringHash playerNameHash);

    void AddUnit (Unit *unit);
    bool RemoveAndDeleteUnit (Unit *unit);
    void RemoveAndDeleteAllUnits ();
};
}

