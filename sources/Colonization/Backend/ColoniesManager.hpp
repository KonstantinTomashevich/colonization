#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Core/Map.hpp>

namespace Colonization
{
class ColoniesManager : public Urho3D::Component
{
URHO3D_OBJECT (ColoniesManager, Component)
protected:
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::HashMap <Urho3D::StringHash, float> > investitions_;
    float coloniesBasicPopulationIncrease_;
    float coloniesBasicEvolution_;
    float canBePlantedByOneColonist_;
    float forestCanBeCuttedByOneColonist_;
    float investitionsConsumption_;
    float investitionsEfficiency_;

    float GetTotalColonyEvolution (District *colony);
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
    static void RegisterObject (Urho3D::Context *context);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void Invest (District *district, Urho3D::StringHash investitionType, float money);

    float GetColoniesBasicPopulationIncrease () const;
    void SetColoniesBasicPopulationIncrease (float coloniesBasicPopulationIncrease);

    float GetColoniesBasicEvolution () const;
    void SetColoniesBasicEvolution (float coloniesBasicEvolution);

    float GetCanBePlantedByOneColonist () const;
    void SetCanBePlantedByOneColonist (float canBePlantedByOneColonist);

    float GetForestCanBeCuttedByOneColonist () const;
    void SetForestCanBeCuttedByOneColonist (float forestCanBeCuttedByOneColonist);

    float GetInvestitionsConsumption () const;
    void SetInvestitionsConsumption (float investitionsConsumption);

    float GetInvestitionsEfficiency () const;
    void SetInvestitionsEfficiency (float investitionsEfficiency);
};
}
