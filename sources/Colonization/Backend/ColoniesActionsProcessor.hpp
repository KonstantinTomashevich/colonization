#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

namespace Colonization
{
class ColoniesActionsProcessor : public Urho3D::Component
{
URHO3D_OBJECT (ColoniesActionsProcessor, Component)
protected:
    void ProcessColonyActions (GameConfiguration *configuration, District *colony, float timeStep);
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    ColoniesActionsProcessor (Urho3D::Context *context);
    virtual ~ColoniesActionsProcessor ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
};
}
