#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/District/DistrictUtils.hpp>
#include <Colonization/Backend/UnitsManager.hpp>

namespace Colonization
{
class GameConfiguration;
class ColoniesProcessor : public Urho3D::Component
{
URHO3D_OBJECT (ColoniesProcessor, Component)
public:
    explicit ColoniesProcessor (Urho3D::Context *context);
    virtual ~ColoniesProcessor ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandlePlayerWillBeDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

protected:
    virtual void OnSceneSet (Urho3D::Scene* scene);

private:
    float ProcessColonyEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyPopulation (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyEvolutionBranch (GameConfiguration *configuration, District *colony,
                                        DistrictEvolutionBranch branch, float timeStep);

    float ProcessColonyActions (District *colony, float timeStep);
    /// Return true if action completed.
    bool ProcessAction (District *colony, Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> &action, float timeStep);
    bool ProcessBuildWarShipAction (GameConfiguration *configuration, Map *map, UnitsManager *unitsManager,
                                    District *colony, Urho3D::VariantMap &actionData, float timeStep);
    bool ProcessFormArmyAction (GameConfiguration *configuration, Map *map, UnitsManager *unitsManager,
                                 District *colony, Urho3D::VariantMap &actionData, float timeStep);
};
}
