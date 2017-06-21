#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Colonization/Core/Unit/Unit.hpp>
#include <Colonization/Core/Unit/FleetUnit.hpp>
#include <Colonization/Core/Unit/TradersUnit.hpp>
#include <Colonization/Core/Unit/ColonizatorsUnit.hpp>
#include <Colonization/Core/Unit/ArmyUnit.hpp>

namespace Colonization
{
class GameConfiguration;
//@ASBindGen Function ReturnHandleArray
Urho3D::PODVector <Unit *> GetUnitsInDistrict (Urho3D::Scene *scene, Urho3D::StringHash districtHash);

class UnitsManager : public Urho3D::Component
{
URHO3D_OBJECT (UnitsManager, Component)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <Unit> > units_;

    void SettleColonizator (ColonizatorsUnit *unit, Map *map);
    void ProcessTrader (GameConfiguration *configuration, TradersUnit *unit);
    float GetUnitSpeedBetween (District *position, District *target, GameConfiguration *configuration);
    /// Returns true if unit exists after it, otherwise false.
    bool OnNextTargetReached (Unit *unit, Urho3D::PODVector <Urho3D::StringHash> &unitWay,
                              Map *map, GameConfiguration *configuration);
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    UnitsManager (Urho3D::Context *context);
    virtual ~UnitsManager ();

    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);
    static void RegisterObject (Urho3D::Context *context);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    int GetUnitsCount () const;
    void UpdateUnitsList ();

    Unit *GetUnitByIndex (int index) const;
    Unit *GetUnitByHash (Urho3D::StringHash hash) const;
    Urho3D::PODVector <Urho3D::StringHash> GetUnitsOfPlayer (Urho3D::StringHash playerNameHash) const;
    Unit *CreateUnit (UnitType unitType);
};
}
