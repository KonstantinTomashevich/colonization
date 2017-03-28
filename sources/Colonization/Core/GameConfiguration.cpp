#include <Colonization/BuildConfiguration.hpp>
#include "GameConfiguration.hpp"
#include <Urho3D/Core/Context.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/District.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

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

    oneColonistFarmsProductionConsumption_ (0.02f),
    oneSoldierFarmsProductionConsumption_ (0.0225f),
    farmsProductionMinesConsumption_ (0.025f),
    farmsProductionIndustryConsumption_ (0.2f),

    oneColonistMinesProductionConsumption_ (0.001f),
    oneSoldierMinesProductionConsumption_ (0.001f),
    minesProductionFarmsConsumption_ (0.1f),
    minesProductionIndustryConsumption_ (0.55f),

    oneColonistIndustryProductionConsumption_ (0.0015f),
    oneSoldierIndustryProductionConsumption_ (0.0035f),
    industryProductionFarmsConsumption_ (0.025f),
    industryProductionMinesConsumption_ (0.05f),

    farmsProductionPerColonist_ (0.05f),
    minesProductionPerColonist_ (0.025f),
    industryProductionPerColonist_ (0.0125f),

    farmsProductionInternalCost_ (1.0f),
    minesProductionInternalCost_ (2.0f),
    industryProductionInternalCost_ (5.0f),

    farmsProductionExternalCost_ (0.85f),
    minesProductionExternalCost_ (1.7f),
    industryProductionExternalCost_ (4.25f),

    tradeAreaInternalProfitToBalance_ (0.85f),
    tradeAreaExternalProfitToBalance_ (0.35f),

    tradeAreaFarmsLogisticsExpenses_ (0.05f),
    tradeAreaMinesLogisticsExpenses_ (0.5f),
    tradeAreaIndustryLogisticsExpenses_ (0.15f),

    tradeAreaFarmsDefenseExpenses_ (0.1f),
    tradeAreaMinesDefenseExpenses_ (0.1f),
    tradeAreaIndustryDefenseExpenses_ (0.15f),

    evolutionCostPerLevel_ (1000.0f),
    degradationCostPerLevel_ (1000.0f),
    basicEvolutionSpeed_ (0.01f),
    basicDegradationSpeed_ (0.01f),

    internalTaxes_ (0.25f),
    externalTaxes_ (0.25f),

    coloniesBasicPopulationIncrease_ (0.0005f),
    canBePlantedByOneColonist_ (0.5f),
    minimumLogisticsToUniteDistrictsToTradeArea_ (4.0f),
    minimumLogisticsToUniteDistrictsToTradeAreaBySea_ (7.0f),

    farmsProductionTropicalClimateModifer_ (1.15f),
    farmsProductionHotClimateModifer_ (1.1f),
    farmsProductionTemperateClimateModifer_ (1.0f),
    farmsProductionTemperateContinentalClimateModifer_ (0.95f),
    farmsProductionDesertClimateModifer_ (0.65f),
    farmsProductionColdClimateModifer_ (0.75f),

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
                               SetOneColonistFarmsProductionConsumption, float, 0.02f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("One Soldier Farms Production Consumption", GetOneSoldierFarmsProductionConsumption,
                               SetOneSoldierFarmsProductionConsumption, float, 0.0225f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Mines Consumption", GetFarmsProductionMinesConsumption,
                               SetFarmsProductionMinesConsumption, float, 0.025f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Industry Consumption", GetFarmsProductionIndustryConsumption,
                               SetFarmsProductionIndustryConsumption, float, 0.2f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("One Colonist Mines Production Consumption", GetOneColonistMinesProductionConsumption,
                               SetOneColonistMinesProductionConsumption, float, 0.001f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("One Soldier Mines Production Consumption", GetOneSoldierMinesProductionConsumption,
                               SetOneSoldierMinesProductionConsumption, float, 0.001f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Production Farms Consumption", GetMinesProductionFarmsConsumption,
                               SetMinesProductionFarmsConsumption, float, 0.1f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Production Industry Consumption", GetMinesProductionIndustryConsumption,
                               SetMinesProductionIndustryConsumption, float, 0.55f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("One Colonist Industry Production Consumption", GetOneColonistIndustryProductionConsumption,
                               SetOneColonistIndustryProductionConsumption, float, 0.0015f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("One Soldier Industry Production Consumption", GetOneSoldierIndustryProductionConsumption,
                               SetOneSoldierIndustryProductionConsumption, float, 0.0035f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Production Farms Consumption", GetIndustryProductionFarmsConsumption,
                               SetIndustryProductionFarmsConsumption, float, 0.025f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Production Mines Consumption", GetIndustryProductionMinesConsumption,
                               SetIndustryProductionMinesConsumption, float, 0.05f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Per Colonist", GetFarmsProductionPerColonist,
                               SetFarmsProductionPerColonist, float, 0.05f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Production Per Colonist", GetMinesProductionPerColonist,
                               SetMinesProductionPerColonist, float, 0.025f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Production Per Colonist", GetIndustryProductionPerColonist,
                               SetIndustryProductionPerColonist, float, 0.0125f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Internal Cost", GetFarmsProductionInternalCost,
                               SetFarmsProductionInternalCost, float, 1.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Production Internal Cost", GetMinesProductionInternalCost,
                               SetMinesProductionInternalCost, float, 2.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Production Internal Cost", GetIndustryProductionInternalCost,
                               SetIndustryProductionInternalCost, float, 5.0f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production External Cost", GetFarmsProductionExternalCost,
                               SetFarmsProductionExternalCost, float, 0.85f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Production External Cost", GetMinesProductionExternalCost,
                               SetMinesProductionExternalCost, float, 1.7f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Production External Cost", GetIndustryProductionExternalCost,
                               SetIndustryProductionExternalCost, float, 4.25f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Area Internal Profit To Balance", GetTradeAreaInternalProfitToBalance,
                               SetTradeAreaInternalProfitToBalance, float, 0.85f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Area External Profit To Balance", GetTradeAreaExternalProfitToBalance,
                               SetTradeAreaExternalProfitToBalance, float, 0.35f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Area Farms Logistics Expenses", GetTradeAreaFarmsLogisticsExpenses,
                               SetTradeAreaFarmsLogisticsExpenses, float, 0.05f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Area Mines Logistics Expenses", GetTradeAreaMinesLogisticsExpenses,
                               SetTradeAreaMinesLogisticsExpenses, float, 0.1f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Area Industry Logistics Expenses", GetTradeAreaIndustryLogisticsExpenses,
                               SetTradeAreaIndustryLogisticsExpenses, float, 0.15f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Area Farms Defense Expenses", GetTradeAreaFarmsDefenseExpenses,
                               SetTradeAreaFarmsDefenseExpenses, float, 0.1f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Area Mines Defense Expenses", GetTradeAreaMinesDefenseExpenses,
                               SetTradeAreaMinesDefenseExpenses, float, 0.1f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Area Industry Defense Expenses", GetTradeAreaIndustryDefenseExpenses,
                               SetTradeAreaIndustryDefenseExpenses, float, 0.15f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Evolution Cost Per Level", GetEvolutionCostPerLevel,
                               SetEvolutionCostPerLevel, float, 1000.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Degradation Cost Per Level", GetDegradationCostPerLevel,
                               SetDegradationCostPerLevel, float, 1000.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Basic Evolution Speed", GetBasicEvolutionSpeed,
                               SetBasicEvolutionSpeed, float, 0.01f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Basic Degradation Speed", GetBasicDegradationSpeed,
                               SetBasicDegradationSpeed, float, 0.01f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Internal Taxes", GetInternalTaxes, SetInternalTaxes, float, 0.25f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("External Taxes", GetExternalTaxes, SetExternalTaxes, float, 0.25f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Colonies Basic Population Increase", GetColoniesBasicPopulationIncrease,
                               SetColoniesBasicPopulationIncrease, float, 0.0005f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Can Be Planted By One Colonist", GetCanBePlantedByOneColonist,
                               SetCanBePlantedByOneColonist, float, 0.5f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Minimum Logistics To Unite Districts To Trade Area", GetMinimumLogisticsToUniteDistrictsToTradeArea,
                               SetMinimumLogisticsToUniteDistrictsToTradeArea, float, 4.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Minimum Logistics To Unite Districts To Trade Area By Sea", GetMinimumLogisticsToUniteDistrictsToTradeAreaBySea,
                               SetMinimumLogisticsToUniteDistrictsToTradeAreaBySea, float, 7.0f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Tropical Climate Modifer", GetFarmsProductionTropicalClimateModifer,
                               SetFarmsProductionTropicalClimateModifer, float, 1.15f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Hot Climate Modifer", GetFarmsProductionHotClimateModifer,
                               SetFarmsProductionHotClimateModifer, float, 1.1f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Temperate Climate Modifer", GetFarmsProductionTemperateClimateModifer,
                               SetFarmsProductionTemperateClimateModifer, float, 1.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Temperate Continental Climate Modifer", GetFarmsProductionTemperateContinentalClimateModifer,
                               SetFarmsProductionTemperateContinentalClimateModifer, float, 0.95f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Desert Climate Modifer", GetFarmsProductionDesertClimateModifer,
                               SetFarmsProductionDesertClimateModifer, float, 0.65f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Production Cold Climate Modifer", GetFarmsProductionColdClimateModifer,
                               SetFarmsProductionColdClimateModifer, float, 0.75f, Urho3D::AM_DEFAULT);

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

Urho3D::StringHash GameConfiguration::GetHeuristicNearestWayToEuropeDistrict (Map *map, District *wayStartDistrict) const
{
    if (wayToEuropeDistricts_.Empty ())
    {
        return Urho3D::StringHash::ZERO;
    }

    if (wayToEuropeDistricts_.Size () == 1)
    {
        return wayToEuropeDistricts_.At (0);
    }

    Urho3D::StringHash heuristicNearest;
    int index = 0;
    float minHeuristicDistance = -1.0f;

    do
    {
        Urho3D::StringHash wayToEuropeDistrictHash = wayToEuropeDistricts_.At (index);
        District *wayToEuropeDistrict = map->GetDistrictByHash (wayToEuropeDistrictHash);
        assert (wayToEuropeDistrict);
        float heuristicDistance = HeuristicDistanceForPathFinding (wayToEuropeDistrict, wayStartDistrict);

        if (heuristicDistance < minHeuristicDistance || minHeuristicDistance <= 0.0f)
        {
            minHeuristicDistance = heuristicDistance;
            heuristicNearest = wayToEuropeDistrictHash;
        }
        index++;
    }
    while (index < wayToEuropeDistricts_.Size ());
    return heuristicNearest;
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

float GameConfiguration::GetOneSoldierFarmsProductionConsumption () const
{
    return oneSoldierFarmsProductionConsumption_;
}

void GameConfiguration::SetOneSoldierFarmsProductionConsumption (float oneSoldierFarmsProductionConsumption)
{
    oneSoldierFarmsProductionConsumption_ = oneSoldierFarmsProductionConsumption;
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

float GameConfiguration::GetOneSoldierMinesProductionConsumption () const
{
    return oneSoldierMinesProductionConsumption_;
}

void GameConfiguration::SetOneSoldierMinesProductionConsumption (float oneSoldierMinesProductionConsumption)
{
    oneSoldierMinesProductionConsumption_ = oneSoldierMinesProductionConsumption;
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

float GameConfiguration::GetOneSoldierIndustryProductionConsumption () const
{
    return oneSoldierIndustryProductionConsumption_;
}

void GameConfiguration::SetOneSoldierIndustryProductionConsumption (float oneSoldierIndustryProductionConsumption)
{
    oneSoldierIndustryProductionConsumption_ = oneSoldierIndustryProductionConsumption;
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

float GameConfiguration::GetFarmsProductionPerColonist () const
{
    return farmsProductionPerColonist_;
}

void GameConfiguration::SetFarmsProductionPerColonist (float farmsProductionPerColonist)
{
    farmsProductionPerColonist_ = farmsProductionPerColonist;
}

float GameConfiguration::GetMinesProductionPerColonist () const
{
    return minesProductionPerColonist_;
}

void GameConfiguration::SetMinesProductionPerColonist (float minesProductionPerColonist)
{
    minesProductionPerColonist_ = minesProductionPerColonist;
}

float GameConfiguration::GetIndustryProductionPerColonist () const
{
    return industryProductionPerColonist_;
}

void GameConfiguration::SetIndustryProductionPerColonist (float industryProductionPerColonist)
{
    industryProductionPerColonist_ = industryProductionPerColonist;
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

float GameConfiguration::GetTradeAreaInternalProfitToBalance () const
{
    return tradeAreaInternalProfitToBalance_;
}

void GameConfiguration::SetTradeAreaInternalProfitToBalance (float tradeAreaInternalProfitToBalance)
{
    tradeAreaInternalProfitToBalance_ = tradeAreaInternalProfitToBalance;
}

float GameConfiguration::GetTradeAreaExternalProfitToBalance () const
{
    return tradeAreaExternalProfitToBalance_;
}

void GameConfiguration::SetTradeAreaExternalProfitToBalance (float tradeAreaExternalProfitToBalance)
{
    tradeAreaExternalProfitToBalance_ = tradeAreaExternalProfitToBalance;
}

float GameConfiguration::GetTradeAreaFarmsLogisticsExpenses () const
{
    return tradeAreaFarmsLogisticsExpenses_;
}

void GameConfiguration::SetTradeAreaFarmsLogisticsExpenses (float tradeAreaFarmsLogisticsExpenses)
{
    tradeAreaFarmsLogisticsExpenses_ = tradeAreaFarmsLogisticsExpenses;
}

float GameConfiguration::GetTradeAreaMinesLogisticsExpenses () const
{
    return tradeAreaMinesLogisticsExpenses_;
}

void GameConfiguration::SetTradeAreaMinesLogisticsExpenses (float tradeAreaMinesLogisticsExpenses)
{
    tradeAreaMinesLogisticsExpenses_ = tradeAreaMinesLogisticsExpenses;
}

float GameConfiguration::GetTradeAreaIndustryLogisticsExpenses () const
{
    return tradeAreaIndustryLogisticsExpenses_;
}

void GameConfiguration::SetTradeAreaIndustryLogisticsExpenses (float tradeAreaIndustryLogisticsExpenses)
{
    tradeAreaIndustryLogisticsExpenses_ = tradeAreaIndustryLogisticsExpenses;
}

float GameConfiguration::GetTradeAreaFarmsDefenseExpenses () const
{
    return tradeAreaFarmsDefenseExpenses_;
}

void GameConfiguration::SetTradeAreaFarmsDefenseExpenses (float tradeAreaFarmsDefenseExpenses)
{
    tradeAreaFarmsDefenseExpenses_ = tradeAreaFarmsDefenseExpenses;
}

float GameConfiguration::GetTradeAreaMinesDefenseExpenses () const
{
    return tradeAreaMinesDefenseExpenses_;
}

void GameConfiguration::SetTradeAreaMinesDefenseExpenses (float tradeAreaMinesDefenseExpenses)
{
    tradeAreaMinesDefenseExpenses_ = tradeAreaMinesDefenseExpenses;
}

float GameConfiguration::GetTradeAreaIndustryDefenseExpenses () const
{
    return tradeAreaIndustryDefenseExpenses_;
}

void GameConfiguration::SetTradeAreaIndustryDefenseExpenses (float tradeAreaIndustryDefenseExpenses)
{
    tradeAreaIndustryDefenseExpenses_ = tradeAreaIndustryDefenseExpenses;
}

float GameConfiguration::GetEvolutionCostPerLevel () const
{
    return evolutionCostPerLevel_;
}

void GameConfiguration::SetEvolutionCostPerLevel (float evolutionCostPerLevel)
{
    evolutionCostPerLevel_ = evolutionCostPerLevel;
}

float GameConfiguration::GetDegradationCostPerLevel () const
{
    return degradationCostPerLevel_;
}

void GameConfiguration::SetDegradationCostPerLevel (float degradationCostPerLevel)
{
    degradationCostPerLevel_ = degradationCostPerLevel;
}

float GameConfiguration::GetBasicEvolutionSpeed () const
{
    return basicEvolutionSpeed_;
}

void GameConfiguration::SetBasicEvolutionSpeed (float basicEvolutionSpeed)
{
    basicEvolutionSpeed_ = basicEvolutionSpeed;
}

float GameConfiguration::GetBasicDegradationSpeed () const
{
    return basicDegradationSpeed_;
}

void GameConfiguration::SetBasicDegradationSpeed (float basicDegradationSpeed)
{
    basicDegradationSpeed_ = basicDegradationSpeed;
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

float GameConfiguration::GetCanBePlantedByOneColonist () const
{
    return canBePlantedByOneColonist_;
}

void GameConfiguration::SetCanBePlantedByOneColonist (float canBePlantedByOneColonist)
{
    canBePlantedByOneColonist_ = canBePlantedByOneColonist;
}

float GameConfiguration::GetMinimumLogisticsToUniteDistrictsToTradeArea () const
{
    return minimumLogisticsToUniteDistrictsToTradeArea_;
}

void GameConfiguration::SetMinimumLogisticsToUniteDistrictsToTradeArea (float minimumLogisticsToUniteDistrictsToTradeArea)
{
    minimumLogisticsToUniteDistrictsToTradeArea_ = minimumLogisticsToUniteDistrictsToTradeArea;
}

float GameConfiguration::GetMinimumLogisticsToUniteDistrictsToTradeAreaBySea () const
{
    return minimumLogisticsToUniteDistrictsToTradeAreaBySea_;
}

void GameConfiguration::SetMinimumLogisticsToUniteDistrictsToTradeAreaBySea (float minimumLogisticsToUniteDistrictsToTradeAreaBySea)
{
    minimumLogisticsToUniteDistrictsToTradeAreaBySea_ = minimumLogisticsToUniteDistrictsToTradeAreaBySea;
}

float GameConfiguration::GetFarmsProductionTropicalClimateModifer () const
{
    return farmsProductionTropicalClimateModifer_;
}

void GameConfiguration::SetFarmsProductionTropicalClimateModifer (float farmsProductionTropicalClimateModifer)
{
    farmsProductionTropicalClimateModifer_ = farmsProductionTropicalClimateModifer;
}

float GameConfiguration::GetFarmsProductionHotClimateModifer () const
{
    return farmsProductionHotClimateModifer_;
}

void GameConfiguration::SetFarmsProductionHotClimateModifer (float farmsProductionHotClimateModifer)
{
    farmsProductionHotClimateModifer_ = farmsProductionHotClimateModifer;
}

float GameConfiguration::GetFarmsProductionTemperateClimateModifer () const
{
    return farmsProductionTemperateClimateModifer_;
}

void GameConfiguration::SetFarmsProductionTemperateClimateModifer (float farmsProductionTemperateClimateModifer)
{
    farmsProductionTemperateClimateModifer_ = farmsProductionTemperateClimateModifer;
}

float GameConfiguration::GetFarmsProductionTemperateContinentalClimateModifer () const
{
    return farmsProductionTemperateContinentalClimateModifer_;
}

void GameConfiguration::SetFarmsProductionTemperateContinentalClimateModifer (float farmsProductionTemperateContinentalClimateModifer)
{
    farmsProductionTemperateContinentalClimateModifer_ = farmsProductionTemperateContinentalClimateModifer;
}

float GameConfiguration::GetFarmsProductionDesertClimateModifer () const
{
    return farmsProductionDesertClimateModifer_;
}

void GameConfiguration::SetFarmsProductionDesertClimateModifer (float farmsProductionDesertClimateModifer)
{
    farmsProductionDesertClimateModifer_ = farmsProductionDesertClimateModifer;
}

float GameConfiguration::GetFarmsProductionColdClimateModifer () const
{
    return farmsProductionColdClimateModifer_;
}

void GameConfiguration::SetFarmsProductionColdClimateModifer (float farmsProductionColdClimateModifer)
{
    farmsProductionColdClimateModifer_ = farmsProductionColdClimateModifer;
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

