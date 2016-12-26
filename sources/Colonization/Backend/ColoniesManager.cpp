#include <Colonization/BuildConfiguration.hpp>
#include "ColoniesManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <cmath>

namespace Colonization
{
void ColoniesManager::ProcessColony (District *colony, float timeStep)
{
    // TODO: Think about balance.
    ProcessColonyPopulation (colony, timeStep);
    ProcessColonyForests (colony, timeStep);
    ProcessColonyFarmsEvolution (colony, timeStep);
    ProcessColonyMinesEvolution (colony, timeStep);
    ProcessColonyIndustryEvolution (colony, timeStep);
    ProcessColonyLogisticsEvolution (colony, timeStep);
    ProcessColonyDefenseEvolution (colony, timeStep);
    // TODO: Implement average level of life calculation.
    colony->averageLevelOfLifePoints_ = 1.0f;
    colony->needDataUpdate_ = true;
}

void ColoniesManager::ProcessColonyPopulation (District *colony, float timeStep)
{
    float sexRatio = colony->menCount_ / colony->womenCount_;
    float increaseModifer = 1.0f - Urho3D::Abs (1.0f - sexRatio);
    float populationIncrease = (colony->menCount_ * colony->womenCount_) *
            coloniesBasicPopulationIncrease_ * increaseModifer * timeStep;

    float newPopulationSexRatio = Urho3D::Random (0.4f, 0.6f);
    colony->menCount_ += populationIncrease * newPopulationSexRatio * timeStep;
    colony->womenCount_ += populationIncrease * (1.0f - newPopulationSexRatio) * timeStep;
}

void ColoniesManager::ProcessColonyForests (District *colony, float timeStep)
{
    if (colony->forestsSquare_ < colony->farmingSquare_)
    {
        colony->forestsSquare_ += colony->forestsReproductivity_ * timeStep;
        colony->farmingSquare_ -= colony->forestsReproductivity_ * timeStep;
    }
}

void ColoniesManager::ProcessColonyFarmsEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = colony->farmsEvolutionPoints_ + colony->minesEvolutionPoints_ +
            colony->industryEvolutionPoints_ + colony->logisticsEvolutionPoints_ +
            colony->defenseEvolutionPoints_;

    float colonyFarmsEvolution = colony->farmsEvolutionPoints_;

    float farmsEvolutionInColonyEvolution = colonyFarmsEvolution / totalColonyEvolution;

    float canBePlanted = (colony->menCount_ + colony->womenCount_) * farmsEvolutionInColonyEvolution *
            canBePlantedByOneColonist_ * sqrt (colonyFarmsEvolution);

    if (colony->forestsSquare_ > (colony->forestsSquare_ + colony->farmingSquare_) * 0.15f)
    {
        colony->forestsSquare_ -= colony->forestsReproductivity_ * 0.25f * forestCanBeCuttedByOneColonist_ * timeStep;
        colony->farmingSquare_ +=  colony->forestsReproductivity_ * 0.25f * forestCanBeCuttedByOneColonist_ * timeStep;
    }

    float climateModifer = 1.0f;
    if (colony->climate_ == CLIMATE_TEMPERATE)
        climateModifer = 1.0f;
    else if (colony->climate_ == CLIMATE_TEMPERATE_CONTINENTAL)
        climateModifer = 0.8f;
    else if (colony->climate_ == CLIMATE_TROPICAL)
        climateModifer = 1.5f;
    else if (colony->climate_ == CLIMATE_HOT)
        climateModifer = 1.25f;
    else if (colony->climate_ == CLIMATE_COLD)
        climateModifer = 0.5f;
    else if (colony->climate_ == CLIMATE_DESERT)
        climateModifer = 0.25f;

    float investitions = investitions_ [colony->GetHash ()] ["farms"];
    float investitionsModifer = 1.0f;
    if (investitions > 0.0f)
    {
        investitionsModifer = sqrt (investitionsEfficiency_);
        investitions_ [colony->GetHash ()] ["farms"] = investitions - investitionsConsumption_ * timeStep;
    }

    float evolutionModifer = (colony->farmingSquare_ - canBePlanted) / colony->farmingSquare_;
    if (evolutionModifer > 0.0f)
    {
        evolutionModifer *= colony->landAverageFertility_;
        evolutionModifer *= climateModifer;
        evolutionModifer *= investitionsModifer;
    }
    else
    {
        evolutionModifer /= colony->landAverageFertility_;
        evolutionModifer /= climateModifer;
        evolutionModifer /= investitionsModifer;
    }

    colony->farmsEvolutionPoints_ += coloniesBasicEvolution_ * evolutionModifer * timeStep;
}

void ColoniesManager::ProcessColonyMinesEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = colony->farmsEvolutionPoints_ + colony->minesEvolutionPoints_ +
            colony->industryEvolutionPoints_ + colony->logisticsEvolutionPoints_ +
            colony->defenseEvolutionPoints_;

    float colonyMinesEvolution = colony->minesEvolutionPoints_;

    float minesEvolutionInColonyEvolution = colonyMinesEvolution / totalColonyEvolution;

    float perspective = 1.0f;

    if (colony->forestsSquare_ < (colony->forestsSquare_ + colony->farmingSquare_) * 0.15f)
        perspective -= 1.0;
    else if (colony->forestsSquare_ < (colony->forestsSquare_ + colony->farmingSquare_) * 0.25f)
        perspective -= 0.75f;
    else if (colony->forestsSquare_ > (colony->forestsSquare_ + colony->farmingSquare_) * 0.5f)
        perspective += 0.5f;
    else if (colony->forestsSquare_ > colony->forestsSquare_ < (colony->forestsSquare_ + colony->farmingSquare_) * 0.75f)
        perspective += 1.0f;

    if (colony->hasCoalDeposits_)
        perspective += 0.5f;

    if (colony->hasIronDeposits_)
        perspective += 0.75f;

    if (colony->hasSilverDeposits_)
        perspective += 1.25f;

    if (colony->hasGoldDeposits_)
        perspective += 1.5f;

    if (colony->forestsSquare_ > (colony->forestsSquare_ + colony->farmingSquare_) * 0.15f)
    {
        colony->forestsSquare_ -= colony->forestsReproductivity_ * forestCanBeCuttedByOneColonist_ * timeStep;
        colony->farmingSquare_ +=  colony->forestsReproductivity_ * forestCanBeCuttedByOneColonist_ * timeStep;
    }

    float investitions = investitions_ [colony->GetHash ()] ["mines"];
    if (investitions > 0.0f)
    {
        perspective *= investitionsEfficiency_;
        investitions_ [colony->GetHash ()] ["mines"] = investitions - investitionsConsumption_ * timeStep;
    }

    perspective += sqrt (colony->industryEvolutionPoints_);
    float modifer = sqrt (perspective);

    if (minesEvolutionInColonyEvolution > 0.25f)
        modifer *= 0.25f;

    else if (minesEvolutionInColonyEvolution > 0.5f)
        modifer *= 0.15f;

    else if (colonyMinesEvolution > 1.5f * colony->farmsEvolutionPoints_)
        modifer *= 0.1f;

    else
        modifer *= 0.5f;

    colony->minesEvolutionPoints_ += coloniesBasicEvolution_ * modifer * timeStep;
}

void ColoniesManager::ProcessColonyIndustryEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = colony->farmsEvolutionPoints_ + colony->minesEvolutionPoints_ +
            colony->industryEvolutionPoints_ + colony->logisticsEvolutionPoints_ +
            colony->defenseEvolutionPoints_;

    float colonyIndustryEvolution = colony->industryEvolutionPoints_;

    float industryEvolutionInColonyEvolution = colonyIndustryEvolution / totalColonyEvolution;

    float perspective = 0.0f;
    perspective += sqrt (colony->minesEvolutionPoints_) * 0.75f;
    perspective += sqrt (colony->logisticsEvolutionPoints_) * 0.75f;

    if (colony->hasCoalDeposits_ && colony->hasIronDeposits_)
        perspective += 2.0f;
    else if (colony->hasCoalDeposits_)
        perspective += 0.5f;
    else if (colony->hasIronDeposits_)
        perspective += 1.0f;

    float investitions = investitions_ [colony->GetHash ()] ["industry"];
    if (investitions > 0.0f)
    {
        perspective *= investitionsEfficiency_;
        investitions_ [colony->GetHash ()] ["industry"] = investitions - investitionsConsumption_ * timeStep;
    }

    float modifer = sqrt (perspective);

    if (industryEvolutionInColonyEvolution > 0.5f)
        modifer *= 0.3f;

    else if (industryEvolutionInColonyEvolution > 0.75f)
        modifer *= 0.15f;

    else if (colonyIndustryEvolution > 5.0f * colony->farmsEvolutionPoints_)
        modifer *= 0.1f;

    else
        modifer *= 0.5f;

    colony->industryEvolutionPoints_ += coloniesBasicEvolution_ * modifer * timeStep;
}

void ColoniesManager::ProcessColonyLogisticsEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = colony->farmsEvolutionPoints_ + colony->minesEvolutionPoints_ +
            colony->industryEvolutionPoints_ + colony->logisticsEvolutionPoints_ +
            colony->defenseEvolutionPoints_;

    float colonyLogisticsEvolution = colony->logisticsEvolutionPoints_;

    float logisticsEvolutionInColonyEvolution = colonyLogisticsEvolution / totalColonyEvolution;

    float perspective = 0.0f;
    perspective += sqrt (colony->farmsEvolutionPoints_) * 0.4f;
    perspective += sqrt (colony->minesEvolutionPoints_) * 0.4f;
    perspective += sqrt (colony->industryEvolutionPoints_) * 0.75f;
    perspective += sqrt (colony->defenseEvolutionPoints_);

    float investitions = investitions_ [colony->GetHash ()] ["logistics"];
    if (investitions > 0.0f)
    {
        perspective *= investitionsEfficiency_;
        investitions_ [colony->GetHash ()] ["logistics"] = investitions - investitionsConsumption_ * timeStep;
    }

    float modifer = sqrt (perspective);

    if (logisticsEvolutionInColonyEvolution > 0.1f)
        modifer *= 0.35f;

    else if (logisticsEvolutionInColonyEvolution > 0.15f)
        modifer *= 0.2f;

    else if (logisticsEvolutionInColonyEvolution > 0.2f)
        modifer *= 0.1f;

    else if (logisticsEvolutionInColonyEvolution > 0.25f)
        modifer *= 0.05f;

    else if (logisticsEvolutionInColonyEvolution > 0.30f)
        modifer *= 0.0f;

    else
        modifer *= 0.5f;

    colony->logisticsEvolutionPoints_ += coloniesBasicEvolution_ * modifer * timeStep;
}

void ColoniesManager::ProcessColonyDefenseEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = colony->farmsEvolutionPoints_ + colony->minesEvolutionPoints_ +
            colony->industryEvolutionPoints_ + colony->logisticsEvolutionPoints_ +
            colony->defenseEvolutionPoints_;

    float colonyDefenseEvolution = colony->defenseEvolutionPoints_;

    float defenseEvolutionInColonyEvolution = colonyDefenseEvolution / totalColonyEvolution;
    float modifer = 1.0f;

    if (defenseEvolutionInColonyEvolution > 0.1f)
        modifer *= 0.4f;

    else if (defenseEvolutionInColonyEvolution > 0.2f)
        modifer *= 0.1f;

    else if (defenseEvolutionInColonyEvolution > 0.5f)
        modifer *= 0.0f;

    else
        modifer *= 0.5f;

    float investitions = investitions_ [colony->GetHash ()] ["defense"];
    if (investitions > 0.0f)
    {
        modifer *= sqrt (investitionsEfficiency_);
        investitions_ [colony->GetHash ()] ["defense"] = investitions - investitionsConsumption_ * timeStep;
    }

    colony->defenseEvolutionPoints_ += coloniesBasicEvolution_ * modifer * timeStep;
}

ColoniesManager::ColoniesManager (Urho3D::Context *context) : Urho3D::Object (context),
    coloniesBasicPopulationIncrease_ (0.005f),
    coloniesBasicEvolution_ (0.002f),
    canBePlantedByOneColonist_ (1.0f),
    investitionsConsumption_ (2.5f),
    investitionsEfficiency_ (5.0f),
    investitions_ ()
{
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (ColoniesManager, Update));
}

ColoniesManager::~ColoniesManager ()
{

}

void ColoniesManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Map *map = (Map *) context_->GetGlobalVar ("Map").GetPtr ();
    assert (map);
    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();

    for (int index = 0; index < map->GetDistrictsCount (); index++)
    {
        District *district = map->GetDistrictByIndex (index);
        if (district->hasColony_)
            ProcessColony (district, timeStep);
    }
}

void ColoniesManager::Invest (District *district, Urho3D::StringHash investitionType, float money)
{
    investitions_ [district->GetHash ()] [investitionType] = investitions_ [district->GetHash ()] [investitionType] + money;
}
}
