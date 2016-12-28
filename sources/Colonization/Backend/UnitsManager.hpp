#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Colonization/Core/Unit.hpp>

namespace Colonization
{
class UnitsManager : public Urho3D::Component
{
URHO3D_OBJECT (UnitsManager, Component)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <Unit> > units_;

    void UpdateUnitsList ();
    void SettleColonizator (Unit *unit, Map *map);
    void ProcessTrader (Unit *unit);

public:
    UnitsManager (Urho3D::Context *context);
    virtual ~UnitsManager ();

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    int GetUnitsCount ();
    Unit *GetUnitByIndex (int index);
    Unit *GetUnitByHash (Urho3D::StringHash hash);
};
}
