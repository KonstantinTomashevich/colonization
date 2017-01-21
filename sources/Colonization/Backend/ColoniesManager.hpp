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
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::HashMap <Urho3D::StringHash, float> > investitions_;

    float GetTotalColonyEvolution (District *colony);
    void ProcessColony (GameConfiguration *configuration, District *colony, float timeStep);
    void ProcessColonyPopulation (GameConfiguration *configuration, District *colony, float timeStep);
    void ProcessColonyForests (GameConfiguration *configuration, District *colony, float timeStep);
    void ProcessColonyFarmsEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    void ProcessColonyMinesEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    void ProcessColonyIndustryEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    void ProcessColonyLogisticsEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    void ProcessColonyDefenseEvolution (GameConfiguration *configuration, District *colony, float timeStep);
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    ColoniesManager (Urho3D::Context *context);
    virtual ~ColoniesManager ();
    static void RegisterObject (Urho3D::Context *context);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void Invest (District *district, Urho3D::StringHash investitionType, float money);
};
}
