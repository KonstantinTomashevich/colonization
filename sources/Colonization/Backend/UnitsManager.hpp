#pragma once
#include <Urho3D/Core/Object.h>

namespace Colonization
{
class UnitsManager : public Urho3D::Object
{
URHO3D_OBJECT (UnitsManager, Object)
protected:
    UnitsContainer *unitsContainer_;

    void SettleColonizator (ColonizatorsUnit *unit);
    void ProcessTrader (TradersUnit *unit);

public:
    UnitsManager (Urho3D::Context *context);
    virtual ~UnitsManager ();

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    UnitsContainer *GetUnitsContainer ();
};
}
