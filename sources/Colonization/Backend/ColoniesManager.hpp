#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/Map.hpp>

namespace Colonization
{
class ColoniesManager : public Urho3D::Object
{
URHO3D_OBJECT (ColoniesManager, Object)
protected:
    void ProcessColony (District *colony, float timeStep);
    void ProcessColonyPopulation (District *colony, float timeStep);
    void ProcessColonyForests (District *colony, float timeStep);
    void ProcessColonyFarmsEvolution (District *colony, float timeStep);
    void ProcessColonyMinesEvolution (District *colony, float timeStep);
    void ProcessColonyIndustryEvolution (District *colony, float timeStep);
    void ProcessColonyLogisticsEvolution (District *colony, float timeStep);
    void ProcessColonyDefenseEvolution (District *colony, float timeStep);

public:
    ColoniesManager (Urho3D::Context *context);
    virtual ~ColoniesManager ();

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    float coloniesBasicPopulationIncrease_;
    float coloniesBasicEvolution_;
    float canBePlantedByOneColonist_;
    float forestCanBeCuttedByOneColonist_;
};
}
