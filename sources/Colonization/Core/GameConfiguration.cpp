#include <Colonization/BuildConfiguration.hpp>
#include "GameConfiguration.hpp"
#include <Urho3D/Core/Context.h>
#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
const char *wayToEuropeStructureElementsNames [] =
{
    "Way To Europe Districts Count",
    "   District Hash",
    0
};

GameConfiguration::GameConfiguration (Urho3D::Context *context) : Urho3D::Component (context),
    wayToEuropeDistricts_ (),
    sailSpeed_ (0.40f),
    marchSpeed_ (0.15f),
    embarkationSpeed_ (0.11f),
    disembarkationSpeed_ (0.11f),

    oneColonistFarmsProductionConsumption_ (0.01f),
    farmsProductionMinesConsumption_ (0.005f),
    farmsProductionIndustryConsumption_ (0.3f),

    oneColonistMinesProductionConsumption_ (0.002f),
    minesProductionFarmsConsumption_ (0.2f),
    minesProductionIndustryConsumption_ (0.8f),

    oneColonistIndustryProductionConsumption_ (0.003f),
    industryProductionFarmsConsumption_ (0.15f),
    industryProductionMinesConsumption_ (0.25f),

    farmsProductionInternalCost_ (5.0f),
    minesProductionInternalCost_ (10.0f),
    industryProductionInternalCost_ (25.0f),

    farmsProductionExternalCost_ (3.0f),
    minesProductionExternalCost_ (7.0f),
    industryProductionExternalCost_ (18.0f),

    internalTaxes_ (0.25f),
    externalTaxes_ (0.25f),

    coloniesBasicPopulationIncrease_ (0.0005f),
    coloniesBasicEvolution_ (0.005f),
    canBePlantedByOneColonist_ (0.5f),
    forestCanBeCutDownByOneColonist_ (0.025f),
    investitionsConsumption_ (3.5f),
    investitionsEfficiency_ (9.0f),

    maximumGameDuration_ (1200.0f),
    colonyPointsModifer_ (0.15f),
    unitFleetPointsModifer_ (0.5f),
    unitTradersPointsModifer_ (0.02f),
    unitColonizatorsPointsModifer_ (0.02f),
    unitArmyPointsModifer_ (0.02f),
    victoryTypesProcessorScriptPath_ ("AngelScript/Utils/DefaultVictoryTypesProcessor.as")
{

}

GameConfiguration::~GameConfiguration ()
{

}

void GameConfiguration::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <GameConfiguration> (COLONIZATION_CORE_CATEGORY);

    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Way To Europe Districts", GetWayToEuropeDistrictsAttribute, SetWayToEuropeDistrictsAttribute,
                                                              Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector,
                                                              wayToEuropeStructureElementsNames, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Sail Speed", GetSailSpeed, SetSailSpeed, float, 0.40f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("March Speed", GetMarchSpeed, SetMarchSpeed, float, 0.15f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Embarkation Speed", GetEmbarkationSpeed,
                               SetEmbarkationSpeed, float, 0.11f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Disembarkation Speed", GetDisembarkationSpeed,
                               SetDisembarkationSpeed, float, 0.11f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("One Colonist Farms Production Consumption", GetOneColonistFarmsProductionConsumption,
                               SetOneColonistFarmsProductionConsumption, float, 0.01f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Mines Consumption", GetFarmsProductionMinesConsumption,
                               SetFarmsProductionMinesConsumption, float, 0.005f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Industry Consumption", GetFarmsProductionIndustryConsumption,
                               SetFarmsProductionIndustryConsumption, float, 0.3f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("One Colonist Mines Production Consumption", GetOneColonistMinesProductionConsumption,
                               SetOneColonistMinesProductionConsumption, float, 0.002f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Production Farms Consumption", GetMinesProductionFarmsConsumption,
                               SetMinesProductionFarmsConsumption, float, 0.2f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Production Industry Consumption", GetMinesProductionIndustryConsumption,
                               SetMinesProductionIndustryConsumption, float, 0.8f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("One Colonist Industry Production Consumption", GetOneColonistIndustryProductionConsumption,
                               SetOneColonistIndustryProductionConsumption, float, 0.003f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Production Farms Consumption", GetIndustryProductionFarmsConsumption,
                               SetIndustryProductionFarmsConsumption, float, 0.15f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Production Industry Consumption", GetIndustryProductionMinesConsumption,
                               SetIndustryProductionMinesConsumption, float, 0.25f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Internal Cost", GetFarmsProductionInternalCost,
                               SetFarmsProductionInternalCost, float, 5.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Production Internal Cost", GetMinesProductionInternalCost,
                               SetMinesProductionInternalCost, float, 10.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Production Internal Cost", GetIndustryProductionInternalCost,
                               SetIndustryProductionInternalCost, float, 25.0f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production External Cost", GetFarmsProductionExternalCost,
                               SetFarmsProductionExternalCost, float, 3.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Production External Cost", GetMinesProductionExternalCost,
                               SetMinesProductionExternalCost, float, 7.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Production External Cost", GetIndustryProductionExternalCost,
                               SetIndustryProductionExternalCost, float, 18.0f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Internal Taxes", GetInternalTaxes, SetInternalTaxes, float, 0.25f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("External Taxes", GetExternalTaxes, SetExternalTaxes, float, 0.25f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Colonies Basic Population Increase", GetColoniesBasicPopulationIncrease,
                               SetColoniesBasicPopulationIncrease, float, 0.0005f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Colonies Basic Evolution", GetColoniesBasicEvolution,
                               SetColoniesBasicEvolution, float, 0.005f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Can Be Planted By One Colonist", GetCanBePlantedByOneColonist,
                               SetCanBePlantedByOneColonist, float, 0.5f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Forest Can Be Cut Down By One Colonist", GetForestCanBeCutDownByOneColonist,
                               SetForestCanBeCutDownByOneColonist, float, 0.025f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Investitions Consumption", GetInvestitionsConsumption,
                               SetInvestitionsConsumption, float, 3.5f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Investitions Efficiency", GetInvestitionsEfficiency,
                               SetInvestitionsEfficiency, float, 9.0f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Maximum Game Duration", GetMaximumGameDuration,
                               SetMaximumGameDuration, float, 600.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Colony Points Modifer", GetColonyPointsModifer,
                               SetColonyPointsModifer, float, 0.15f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Unit Fleet Points Modifer", GetUnitFleetPointsModifer,
                               SetUnitFleetPointsModifer, float, 0.5f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Unit Traders Points Modifer", GetUnitTradersPointsModifer,
                               SetUnitTradersPointsModifer, float, 0.02f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Unit Colonizators Points Modifer", GetUnitColonizatorsPointsModifer,
                               SetUnitColonizatorsPointsModifer, float, 0.02f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Unit Army Points Modifer", GetUnitArmyPointsModifer,
                               SetUnitArmyPointsModifer, float, 0.02f, Urho3D::AM_DEFAULT);

    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Victory Types Processor Script Path", GetVictoryTypesProcessorScriptPath,
                                     SetVictoryTypesProcessorScriptPath, Urho3D::String,
                                     Urho3D::String ("AngelScript/Utils/DefaultVictoryTypesProcessor.as"), Urho3D::AM_DEFAULT);
}

Urho3D::PODVector <Urho3D::StringHash> GameConfiguration::GetWayToEuropeDistricts () const
{
    return wayToEuropeDistricts_;
}

void GameConfiguration::SetWayToEuropeDistricts (Urho3D::PODVector <Urho3D::StringHash> wayToEuropeDistricts)
{
    wayToEuropeDistricts_ = wayToEuropeDistricts;
}

Urho3D::VariantVector GameConfiguration::GetWayToEuropeDistrictsAttribute () const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (Urho3D::Variant (wayToEuropeDistricts_.Size ()));
    for (int index = 0; index < wayToEuropeDistricts_.Size (); index++)
    {
        variantVector.Push (Urho3D::Variant (wayToEuropeDistricts_.At (index)));
    }
    return variantVector;
}

void GameConfiguration::SetWayToEuropeDistrictsAttribute (const Urho3D::VariantVector &wayToEuropeDistricts)
{
    wayToEuropeDistricts_.Clear ();
    if (!wayToEuropeDistricts.Empty ())
    {
        int requestedSize = wayToEuropeDistricts.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < wayToEuropeDistricts.Size ())
                {
                    wayToEuropeDistricts_.Push (wayToEuropeDistricts.At (index + 1).GetStringHash ());
                }
                else
                {
                    wayToEuropeDistricts_.Push (Urho3D::StringHash ());
                }
            }
        }
    }
}

float GameConfiguration::GetSailSpeed () const
{
    return sailSpeed_;
}

void GameConfiguration::SetSailSpeed (float sailSpeed)
{
    sailSpeed_ = sailSpeed;
}

float GameConfiguration::GetMarchSpeed () const
{
    return marchSpeed_;
}

void GameConfiguration::SetMarchSpeed (float marchSpeed)
{
    marchSpeed_ = marchSpeed;
}

float GameConfiguration::GetEmbarkationSpeed () const
{
    return embarkationSpeed_;
}

void GameConfiguration::SetEmbarkationSpeed (float embarkationSpeed)
{
    embarkationSpeed_ = embarkationSpeed;
}

float GameConfiguration::GetDisembarkationSpeed () const
{
    return disembarkationSpeed_;
}

void GameConfiguration::SetDisembarkationSpeed (float disembarkationSpeed)
{
    disembarkationSpeed_ = disembarkationSpeed;
}

float GameConfiguration::GetOneColonistFarmsProductionConsumption () const
{
    return oneColonistFarmsProductionConsumption_;
}

void GameConfiguration::SetOneColonistFarmsProductionConsumption (float oneColonistFarmsProductionConsumption)
{
    oneColonistFarmsProductionConsumption_ = oneColonistFarmsProductionConsumption;
}

float GameConfiguration::GetFarmsProductionMinesConsumption () const
{
    return farmsProductionMinesConsumption_;
}

void GameConfiguration::SetFarmsProductionMinesConsumption (float farmsProductionMinesConsumption)
{
    farmsProductionMinesConsumption_ = farmsProductionMinesConsumption;
}

float GameConfiguration::GetFarmsProductionIndustryConsumption () const
{
    return farmsProductionIndustryConsumption_;
}

void GameConfiguration::SetFarmsProductionIndustryConsumption (float farmsProductionIndustryConsumption)
{
    farmsProductionIndustryConsumption_ = farmsProductionIndustryConsumption;
}

float GameConfiguration::GetOneColonistMinesProductionConsumption () const
{
    return oneColonistMinesProductionConsumption_;
}

void GameConfiguration::SetOneColonistMinesProductionConsumption (float oneColonistMinesProductionConsumption)
{
    oneColonistMinesProductionConsumption_ = oneColonistMinesProductionConsumption;
}

float GameConfiguration::GetMinesProductionFarmsConsumption () const
{
    return minesProductionFarmsConsumption_;
}

void GameConfiguration::SetMinesProductionFarmsConsumption (float minesProductionFarmsConsumption)
{
    minesProductionFarmsConsumption_ = minesProductionFarmsConsumption;
}

float GameConfiguration::GetMinesProductionIndustryConsumption () const
{
    return minesProductionIndustryConsumption_;
}

void GameConfiguration::SetMinesProductionIndustryConsumption (float minesProductionIndustryConsumption)
{
    minesProductionIndustryConsumption_ = minesProductionIndustryConsumption;
}

float GameConfiguration::GetOneColonistIndustryProductionConsumption () const
{
    return oneColonistIndustryProductionConsumption_;
}

void GameConfiguration::SetOneColonistIndustryProductionConsumption (float oneColonistIndustryProductionConsumption)
{
    oneColonistIndustryProductionConsumption_ = oneColonistIndustryProductionConsumption;
}

float GameConfiguration::GetIndustryProductionFarmsConsumption () const
{
    return industryProductionFarmsConsumption_;
}

void GameConfiguration::SetIndustryProductionFarmsConsumption (float industryProductionFarmsConsumption)
{
    industryProductionFarmsConsumption_ = industryProductionFarmsConsumption;
}

float GameConfiguration::GetIndustryProductionMinesConsumption () const
{
    return industryProductionMinesConsumption_;
}

void GameConfiguration::SetIndustryProductionMinesConsumption (float industryProductionMinesConsumption)
{
    industryProductionMinesConsumption_ = industryProductionMinesConsumption;
}

float GameConfiguration::GetFarmsProductionInternalCost () const
{
    return farmsProductionInternalCost_;
}

void GameConfiguration::SetFarmsProductionInternalCost (float farmsProductionInternalCost)
{
    farmsProductionInternalCost_ = farmsProductionInternalCost;
}

float GameConfiguration::GetMinesProductionInternalCost () const
{
    return minesProductionInternalCost_;
}

void GameConfiguration::SetMinesProductionInternalCost (float minesProductionInternalCost)
{
    minesProductionInternalCost_ = minesProductionInternalCost;
}

float GameConfiguration::GetIndustryProductionInternalCost () const
{
    return industryProductionInternalCost_;
}

void GameConfiguration::SetIndustryProductionInternalCost (float industryProductionInternalCost)
{
    industryProductionInternalCost_ = industryProductionInternalCost;
}

float GameConfiguration::GetFarmsProductionExternalCost () const
{
    return farmsProductionExternalCost_;
}

void GameConfiguration::SetFarmsProductionExternalCost (float farmsProductionExternalCost)
{
    farmsProductionExternalCost_ = farmsProductionExternalCost;
}

float GameConfiguration::GetMinesProductionExternalCost () const
{
    return minesProductionExternalCost_;
}

void GameConfiguration::SetMinesProductionExternalCost (float minesProductionExternalCost)
{
    minesProductionExternalCost_ = minesProductionExternalCost;
}

float GameConfiguration::GetIndustryProductionExternalCost () const
{
    return industryProductionExternalCost_;
}

void GameConfiguration::SetIndustryProductionExternalCost (float industryProductionExternalCost)
{
    industryProductionExternalCost_ = industryProductionExternalCost;
}

float GameConfiguration::GetInternalTaxes () const
{
    return internalTaxes_;
}

void GameConfiguration::SetInternalTaxes (float internalTaxes)
{
    internalTaxes_ = internalTaxes;
}

float GameConfiguration::GetExternalTaxes () const
{
    return externalTaxes_;
}

void GameConfiguration::SetExternalTaxes (float externalTaxes)
{
    externalTaxes_ = externalTaxes;
}

float GameConfiguration::GetColoniesBasicPopulationIncrease () const
{
    return coloniesBasicPopulationIncrease_;
}

void GameConfiguration::SetColoniesBasicPopulationIncrease (float coloniesBasicPopulationIncrease)
{
    coloniesBasicPopulationIncrease_ = coloniesBasicPopulationIncrease;
}

float GameConfiguration::GetColoniesBasicEvolution () const
{
    return coloniesBasicEvolution_;
}

void GameConfiguration::SetColoniesBasicEvolution (float coloniesBasicEvolution)
{
    coloniesBasicEvolution_ = coloniesBasicEvolution;
}

float GameConfiguration::GetCanBePlantedByOneColonist () const
{
    return canBePlantedByOneColonist_;
}

void GameConfiguration::SetCanBePlantedByOneColonist (float canBePlantedByOneColonist)
{
    canBePlantedByOneColonist_ = canBePlantedByOneColonist;
}

float GameConfiguration::GetForestCanBeCutDownByOneColonist () const
{
    return forestCanBeCutDownByOneColonist_;
}

void GameConfiguration::SetForestCanBeCutDownByOneColonist (float forestCanBeCuttedByOneColonist)
{
    forestCanBeCutDownByOneColonist_ = forestCanBeCuttedByOneColonist;
}

float GameConfiguration::GetInvestitionsConsumption () const
{
    return investitionsConsumption_;
}

void GameConfiguration::SetInvestitionsConsumption (float investitionsConsumption)
{
    investitionsConsumption_ = investitionsConsumption;
}

float GameConfiguration::GetInvestitionsEfficiency () const
{
    return investitionsEfficiency_;
}

void GameConfiguration::SetInvestitionsEfficiency (float investitionsEfficiency)
{
    investitionsEfficiency_ = investitionsEfficiency;
}

float GameConfiguration::GetMaximumGameDuration () const
{
    return maximumGameDuration_;
}

void GameConfiguration::SetMaximumGameDuration (float maximumGameDuration)
{
    maximumGameDuration_ = maximumGameDuration;
}

float GameConfiguration::GetColonyPointsModifer () const
{
    return colonyPointsModifer_;
}

void GameConfiguration::SetColonyPointsModifer (float colonyPointsModifer)
{
    colonyPointsModifer_ = colonyPointsModifer;
}

float GameConfiguration::GetUnitFleetPointsModifer () const
{
    return unitFleetPointsModifer_;
}

void GameConfiguration::SetUnitFleetPointsModifer (float unitFleetPointsModifer)
{
    unitFleetPointsModifer_ = unitFleetPointsModifer;
}

float GameConfiguration::GetUnitTradersPointsModifer () const
{
    return unitTradersPointsModifer_;
}

void GameConfiguration::SetUnitTradersPointsModifer (float unitTradersPointsModifer)
{
    unitTradersPointsModifer_ = unitTradersPointsModifer;
}

float GameConfiguration::GetUnitColonizatorsPointsModifer () const
{
    return unitColonizatorsPointsModifer_;
}

void GameConfiguration::SetUnitColonizatorsPointsModifer (float unitColonizatorsPointsModifer)
{
    unitColonizatorsPointsModifer_ = unitColonizatorsPointsModifer;
}

float GameConfiguration::GetUnitArmyPointsModifer () const
{
    return unitArmyPointsModifer_;
}

void GameConfiguration::SetUnitArmyPointsModifer (float unitArmyPointsModifer)
{
    unitArmyPointsModifer_ = unitArmyPointsModifer;
}

Urho3D::String GameConfiguration::GetVictoryTypesProcessorScriptPath () const
{
    return victoryTypesProcessorScriptPath_;
}

void GameConfiguration::SetVictoryTypesProcessorScriptPath (const Urho3D::String &victoryTypesProcessorScriptPath)
{
    victoryTypesProcessorScriptPath_ = victoryTypesProcessorScriptPath;
}
}

