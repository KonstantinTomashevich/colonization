#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Backend/UnitsManager.hpp>

namespace Colonization
{
class GameConfiguration;
class ColoniesActionsProcessor : public Urho3D::Component
{
URHO3D_OBJECT (ColoniesActionsProcessor, Component)
public:
    explicit ColoniesActionsProcessor (Urho3D::Context *context);
    virtual ~ColoniesActionsProcessor ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

protected:
    virtual void OnSceneSet (Urho3D::Scene* scene);

private:
    void ProcessColonyActions (District *colony, float timeStep);
    /// Return true if action completed.
    bool ProcessAction (District *colony, Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> &action, float timeStep);
    bool ProcessBuildWarShipAction (GameConfiguration *configuration, Map *map, UnitsManager *unitsManager,
                                    District *colony, Urho3D::VariantMap &actionData, float timeStep);
    bool ProcessFormArmyAction (GameConfiguration *configuration, Map *map, UnitsManager *unitsManager,
                                 District *colony, Urho3D::VariantMap &actionData, float timeStep);
};
}
