#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Backend/UnitsManager.hpp>

namespace Colonization
{
class ColoniesActionsProcessor : public Urho3D::Component
{
URHO3D_OBJECT (ColoniesActionsProcessor, Component)
protected:
    void ProcessColonyActions (District *colony, float timeStep);
    virtual void OnSceneSet (Urho3D::Scene* scene);

    /// Return true if action completed.
    bool ProcessAction (District *colony, Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> &action, float timeStep);
    bool ProcessBuildFleetAction (GameConfiguration *configuration, Map *map, UnitsManager *unitsManager,
                                  District *colony, Urho3D::VariantMap &actionData, float timeStep);

public:
    ColoniesActionsProcessor (Urho3D::Context *context);
    virtual ~ColoniesActionsProcessor ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
};
}
