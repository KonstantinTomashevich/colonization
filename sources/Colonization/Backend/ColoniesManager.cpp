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
    float sexRatio = colony->mansCount_ / colony->womenCount_;
    float increaseModifer = 1.0f - Urho3D::Abs (1.0f - sexRatio);
    float populationIncrease = (colony->mansCount_ * colony->womenCount_) *
            coloniesBasicPopulationIncrease_ * increaseModifer * timeStep;

    float newPopulationSexRatio = Urho3D::Random (0.4f, 0.6f);
    colony->mansCount_ += populationIncrease * newPopulationSexRatio * timeStep;
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

    float canBePlanted = (colony->mansCount_ + colony->womenCount_) * farmsEvolutionInColonyEvolution *
            canBePlantedByOneColonist_ * sqrt (colonyFarmsEvolution);

    if (canBePlanted > colony->farmingSquare_ && colony->forestsSquare_ > colony->forestsReproductivity_ * 120.0f)
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

    float evolutionModifer = (colony->farmingSquare_ - canBePlanted) / colony->farmingSquare_;
    if (evolutionModifer > 0.0f)
    {
        evolutionModifer *= colony->landAverageFertility_;
        evolutionModifer *= climateModifer;
    }
    else
    {
        evolutionModifer /= colony->landAverageFertility_;
        evolutionModifer /= climateModifer;
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

    if (colony->forestsSquare_ < colony->forestsReproductivity_ * 120.0f)
        perspective -= 1.0;
    else if (colony->forestsSquare_ < colony->forestsReproductivity_ * 180.0f)
        perspective -= 0.75f;
    else if (colony->forestsSquare_ > colony->forestsReproductivity_ * 360.0f)
        perspective += 0.5f;
    else if (colony->forestsSquare_ > colony->forestsReproductivity_ * 600.0f)
        perspective += 1.0f;

    if (colony->hasCoalDeposits_)
        perspective += 0.5f;

    if (colony->hasIronDeposits_)
        perspective += 0.75f;

    if (colony->hasSilverDeposits_)
        perspective += 1.25f;

    if (colony->hasGoldDeposits_)
        perspective += 1.5f;

    if (colony->forestsSquare_ > colony->forestsReproductivity_ * 120.0f)
    {
        colony->forestsSquare_ -= colony->forestsReproductivity_ * forestCanBeCuttedByOneColonist_ * timeStep;
        colony->farmingSquare_ +=  colony->forestsReproductivity_ * forestCanBeCuttedByOneColonist_ * timeStep;
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

    float perspective = 1.0f;
    perspective += sqrt (colony->minesEvolutionPoints_);
    perspective += sqrt (colony->logisticsEvolutionPoints_);

    if (colony->hasCoalDeposits_ && colony->hasIronDeposits_)
        perspective += 2.0f;

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

    float perspective = 1.0f;
    perspective += sqrt (colony->farmsEvolutionPoints_);
    perspective += sqrt (colony->minesEvolutionPoints_);
    perspective += sqrt (colony->industryEvolutionPoints_);
    perspective += sqrt (colony->defenseEvolutionPoints_) * 2.0f;

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

    colony->defenseEvolutionPoints_ += coloniesBasicEvolution_ * modifer * timeStep;
}

ColoniesManager::ColoniesManager (Urho3D::Context *context) : Urho3D::Object (context),
    coloniesBasicPopulationIncrease_ (0.005f),
    coloniesBasicEvolution_ (0.001f),
    canBePlantedByOneColonist_ (1.0f)
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
}
