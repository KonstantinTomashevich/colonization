#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
class GameConfiguration : public Urho3D::Component
{
URHO3D_OBJECT (GameConfiguration, Component)
protected:
    float sailSpeed_;
    float marchSpeed_;
    float embarkationSpeed_;
    float disembarkationSpeed_;

    float oneColonistFarmsProductionConsumption_;
    float farmsProductionMinesConsumption_;
    float farmsProductionIndustryConsumption_;

    float oneColonistMinesProductionConsumption_;
    float minesProductionFarmsConsumption_;
    float minesProductionIndustryConsumption_;

    float oneColonistIndustryProductionConsumption_;
    float industryProductionFarmsConsumption_;
    float industryProductionMinesConsumption_;

    float farmsProductionInternalCost_;
    float minesProductionInternalCost_;
    float industryProductionInternalCost_;

    float farmsProductionExternalCost_;
    float minesProductionExternalCost_;
    float industryProductionExternalCost_;

    float internalTaxes_;
    float externalTaxes_;

    float coloniesBasicPopulationIncrease_;
    float coloniesBasicEvolution_;
    float canBePlantedByOneColonist_;
    float forestCanBeCutDownByOneColonist_;
    float investitionsConsumption_;
    float investitionsEfficiency_;

public:
    GameConfiguration (Urho3D::Context *context);
    virtual ~GameConfiguration ();
    static void RegisterObject (Urho3D::Context *context);

    float GetSailSpeed () const;
    void SetSailSpeed (float sailSpeed);

    float GetMarchSpeed () const;
    void SetMarchSpeed (float marchSpeed);

    float GetEmbarkationSpeed () const;
    void SetEmbarkationSpeed (float embarkationSpeed);

    float GetDisembarkationSpeed () const;
    void SetDisembarkationSpeed (float disembarkationSpeed);

    float GetOneColonistFarmsProductionConsumption () const;
    void SetOneColonistFarmsProductionConsumption (float oneColonistFarmsProductionConsumption);

    float GetFarmsProductionMinesConsumption () const;
    void SetFarmsProductionMinesConsumption (float farmsProductionMinesConsumption);

    float GetFarmsProductionIndustryConsumption () const;
    void SetFarmsProductionIndustryConsumption (float farmsProductionIndustryConsumption);

    float GetOneColonistMinesProductionConsumption () const;
    void SetOneColonistMinesProductionConsumption (float oneColonistMinesProductionConsumption);

    float GetMinesProductionFarmsConsumption () const;
    void SetMinesProductionFarmsConsumption (float minesProductionFarmsConsumption);

    float GetMinesProductionIndustryConsumption () const;
    void SetMinesProductionIndustryConsumption (float minesProductionIndustryConsumption);

    float GetOneColonistIndustryProductionConsumption () const;
    void SetOneColonistIndustryProductionConsumption (float oneColonistIndustryProductionConsumption);

    float GetIndustryProductionFarmsConsumption () const;
    void SetIndustryProductionFarmsConsumption (float industryProductionFarmsConsumption);

    float GetIndustryProductionMinesConsumption () const;
    void SetIndustryProductionMinesConsumption (float industryProductionMinesConsumption);

    float GetFarmsProductionInternalCost () const;
    void SetFarmsProductionInternalCost (float farmsProductionInternalCost);

    float GetMinesProductionInternalCost () const;
    void SetMinesProductionInternalCost (float minesProductionInternalCost);

    float GetIndustryProductionInternalCost () const;
    void SetIndustryProductionInternalCost (float industryProductionInternalCost);

    float GetFarmsProductionExternalCost () const;
    void SetFarmsProductionExternalCost (float farmsProductionExternalCost);

    float GetMinesProductionExternalCost () const;
    void SetMinesProductionExternalCost (float minesProductionExternalCost);

    float GetIndustryProductionExternalCost () const;
    void SetIndustryProductionExternalCost (float industryProductionExternalCost);

    float GetInternalTaxes () const;
    void SetInternalTaxes (float internalTaxes);

    float GetExternalTaxes () const;
    void SetExternalTaxes (float externalTaxes);

    float GetColoniesBasicPopulationIncrease () const;
    void SetColoniesBasicPopulationIncrease (float coloniesBasicPopulationIncrease);

    float GetColoniesBasicEvolution () const;
    void SetColoniesBasicEvolution (float coloniesBasicEvolution);

    float GetCanBePlantedByOneColonist () const;
    void SetCanBePlantedByOneColonist (float canBePlantedByOneColonist);

    float GetForestCanBeCutDownByOneColonist () const;
    void SetForestCanBeCutDownByOneColonist (float forestCanBeCuttedByOneColonist);

    float GetInvestitionsConsumption () const;
    void SetInvestitionsConsumption (float investitionsConsumption);

    float GetInvestitionsEfficiency () const;
    void SetInvestitionsEfficiency (float investitionsEfficiency);
};
}
