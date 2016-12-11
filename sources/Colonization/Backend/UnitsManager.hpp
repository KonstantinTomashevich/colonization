#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/UnitsContainer.hpp>

namespace Colonization
{
class UnitsManager : public Urho3D::Object
{
URHO3D_OBJECT (UnitsManager, Object)
protected:
    UnitsContainer *unitsContainer_;

public:
    UnitsManager (Urho3D::Context *context);
    virtual ~UnitsManager ();

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    UnitsContainer *GetUnitsContainer ();
};
}
