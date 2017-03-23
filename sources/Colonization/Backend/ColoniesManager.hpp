#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

namespace Colonization
{
class ColoniesManager : public Urho3D::Component
{
URHO3D_OBJECT (ColoniesManager, Component)
protected:
    float GetTotalColonyEvolution (District *colony);
    void ProcessColony (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyPopulation (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyFarmsEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyMinesEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyIndustryEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyLogisticsEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    float ProcessColonyDefenseEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    ColoniesManager (Urho3D::Context *context);
    virtual ~ColoniesManager ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
};
}
