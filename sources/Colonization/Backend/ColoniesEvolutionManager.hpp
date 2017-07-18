#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/Map.hpp>

namespace Colonization
{
class GameConfiguration;
class ColoniesEvolutionManager : public Urho3D::Component
{
URHO3D_OBJECT (ColoniesEvolutionManager, Component)
public:
    explicit ColoniesEvolutionManager (Urho3D::Context *context);
    virtual ~ColoniesEvolutionManager ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandlePlayerWillBeDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

protected:
    virtual void OnSceneSet (Urho3D::Scene* scene);

private:
    float GetTotalColonyEvolution (District *colony);
    void ProcessColony (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyPopulation (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyFarmsEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyMinesEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyIndustryEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyLogisticsEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyDefenseEvolution (GameConfiguration *configuration, District *colony, float timeStep);
};
}
