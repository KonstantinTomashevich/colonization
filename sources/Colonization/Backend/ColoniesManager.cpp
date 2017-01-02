#include <Colonization/BuildConfiguration.hpp>
#include "ColoniesManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Utils/Categories.hpp>
#include <cmath>

namespace Colonization
{
float ColoniesManager::GetTotalColonyEvolution(District *colony)
{
    return colony->GetFarmsEvolutionPoints () + colony->GetMinesEvolutionPoints () +
            colony->GetIndustryEvolutionPoints () + colony->GetLogisticsEvolutionPoints () +
            colony->GetDefenseEvolutionPoints ();
}

void ColoniesManager::ProcessColony (District *colony, float timeStep)
{
    // TODO: Think about balance.
    // TODO: Maybe delete ability to cut forests? And delete forests reproductivity too.
    ProcessColonyPopulation (colony, timeStep);
    ProcessColonyForests (colony, timeStep);
    ProcessColonyFarmsEvolution (colony, timeStep);
    ProcessColonyMinesEvolution (colony, timeStep);
    ProcessColonyIndustryEvolution (colony, timeStep);
    ProcessColonyLogisticsEvolution (colony, timeStep);
    ProcessColonyDefenseEvolution (colony, timeStep);
    // TODO: Implement average level of life calculation.
    colony->SetAverageLevelOfLifePoints (1.0f);
    colony->MarkNetworkUpdate ();
}

void ColoniesManager::ProcessColonyPopulation (District *colony, float timeStep)
{
    float sexRatio = colony->GetMenCount () / colony->GetWomenCount ();
    float increaseModifer = 1.0f - Urho3D::Abs (1.0f - sexRatio);
    float populationIncrease = (colony->GetMenCount () * colony->GetWomenCount ()) *
            coloniesBasicPopulationIncrease_ * increaseModifer * timeStep;

    float newPopulationSexRatio = Urho3D::Random (0.4f, 0.6f);
    colony->SetMenCount (colony->GetMenCount () + populationIncrease * newPopulationSexRatio * timeStep);
    colony->SetWomenCount (colony->GetWomenCount () + populationIncrease * (1.0f - newPopulationSexRatio) * timeStep);
}

void ColoniesManager::ProcessColonyForests (District *colony, float timeStep)
{
    if (colony->GetForestsSquare () < colony->GetFarmingSquare ())
    {
        colony->SetForestsSquare (colony->GetForestsSquare () + colony->GetForestsReproductivity () * timeStep);
        colony->SetFarmingSquare (colony->GetFarmingSquare () - colony->GetForestsReproductivity () * timeStep);
    }
}

void ColoniesManager::ProcessColonyFarmsEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyFarmsEvolution = colony->GetFarmsEvolutionPoints ();
    float farmsEvolutionInColonyEvolution = colonyFarmsEvolution / totalColonyEvolution;

    float canBePlanted = (colony->GetMenCount () + colony->GetWomenCount ()) * farmsEvolutionInColonyEvolution *
            canBePlantedByOneColonist_ * sqrt (colonyFarmsEvolution);

    if (colony->GetForestsSquare () > (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.15f)
    {
        colony->SetForestsSquare (colony->GetForestsSquare () - 0.25f * forestCanBeCuttedByOneColonist_ * timeStep);
        colony->SetFarmingSquare (colony->GetFarmingSquare () + 0.25f * forestCanBeCuttedByOneColonist_ * timeStep);
    }

    float climateModifer = 1.0f;
    if (colony->GetClimate () == CLIMATE_TEMPERATE)
        climateModifer = 1.0f;
    else if (colony->GetClimate () == CLIMATE_TEMPERATE_CONTINENTAL)
        climateModifer = 0.8f;
    else if (colony->GetClimate () == CLIMATE_TROPICAL)
        climateModifer = 1.5f;
    else if (colony->GetClimate () == CLIMATE_HOT)
        climateModifer = 1.25f;
    else if (colony->GetClimate () == CLIMATE_COLD)
        climateModifer = 0.5f;
    else if (colony->GetClimate () == CLIMATE_DESERT)
        climateModifer = 0.25f;

    float investitions = investitions_ [colony->GetHash ()] ["farms"];
    float investitionsModifer = 1.0f;
    if (investitions > 0.0f)
    {
        investitionsModifer = sqrt (investitionsEfficiency_);
        investitions_ [colony->GetHash ()] ["farms"] = investitions - investitionsConsumption_ * timeStep;
    }

    float evolutionModifer = (colony->GetFarmingSquare () - canBePlanted) / colony->GetFarmingSquare ();
    if (evolutionModifer > 0.0f)
    {
        evolutionModifer *= colony->GetLandAverageFertility ();
        evolutionModifer *= climateModifer;
        evolutionModifer *= investitionsModifer;
    }
    else
        evolutionModifer = 0.0f;

    colony->SetFarmsEvolutionPoints (colony->GetFarmsEvolutionPoints () + coloniesBasicEvolution_ * evolutionModifer * timeStep);
}

void ColoniesManager::ProcessColonyMinesEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyMinesEvolution = colony->GetMinesEvolutionPoints ();
    float minesEvolutionInColonyEvolution = colonyMinesEvolution / totalColonyEvolution;

    float perspective = 1.0f;

    if (colony->GetForestsSquare () < (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.15f)
        perspective -= 1.0;
    else if (colony->GetForestsSquare () < (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.25f)
        perspective -= 0.75f;
    else if (colony->GetForestsSquare () > (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.5f)
        perspective += 0.5f;
    else if (colony->GetForestsSquare () > colony->GetForestsSquare () < (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.75f)
        perspective += 1.0f;

    if (colony->HasCoalDeposits ())
        perspective += 0.5f;

    if (colony->HasIronDeposits ())
        perspective += 0.75f;

    if (colony->HasSilverDeposits ())
        perspective += 1.25f;

    if (colony->HasGoldDeposits ())
        perspective += 1.5f;

    if (colony->GetForestsSquare () > (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.15f)
    {
        colony->SetForestsSquare (colony->GetForestsSquare () - forestCanBeCuttedByOneColonist_ * timeStep);
        colony->SetFarmingSquare (colony->GetFarmingSquare () + forestCanBeCuttedByOneColonist_ * timeStep);
    }

    float investitions = investitions_ [colony->GetHash ()] ["mines"];
    if (investitions > 0.0f)
    {
        perspective *= investitionsEfficiency_;
        investitions_ [colony->GetHash ()] ["mines"] = investitions - investitionsConsumption_ * timeStep;
    }

    perspective += sqrt (colony->GetIndustryEvolutionPoints ());
    float modifer = sqrt (perspective);

    if (minesEvolutionInColonyEvolution > 0.25f)
        modifer *= 0.25f;

    else if (minesEvolutionInColonyEvolution > 0.5f)
        modifer *= 0.15f;

    else if (colonyMinesEvolution > 1.5f * colony->GetFarmsEvolutionPoints ())
        modifer *= 0.1f;

    else
        modifer *= 0.5f;

    colony->SetMinesEvolutionPoints (colony->GetMinesEvolutionPoints () + coloniesBasicEvolution_ * modifer * timeStep);
}

void ColoniesManager::ProcessColonyIndustryEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyIndustryEvolution = colony->GetIndustryEvolutionPoints ();
    float industryEvolutionInColonyEvolution = colonyIndustryEvolution / totalColonyEvolution;

    float perspective = 0.0f;
    perspective += sqrt (colony->GetMinesEvolutionPoints ()) * 0.75f;
    perspective += sqrt (colony->GetLogisticsEvolutionPoints ()) * 0.75f;

    if (colony->HasCoalDeposits () && colony->HasIronDeposits ())
        perspective += 2.0f;
    else if (colony->HasCoalDeposits ())
        perspective += 0.5f;
    else if (colony->HasIronDeposits ())
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

    else if (colonyIndustryEvolution > 5.0f * colony->GetFarmsEvolutionPoints ())
        modifer *= 0.1f;

    else
        modifer *= 0.5f;

    colony->SetIndustryEvolutionPoints (colony->GetIndustryEvolutionPoints () + coloniesBasicEvolution_ * modifer * timeStep);
}

void ColoniesManager::ProcessColonyLogisticsEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyLogisticsEvolution = colony->GetLogisticsEvolutionPoints ();
    float logisticsEvolutionInColonyEvolution = colonyLogisticsEvolution / totalColonyEvolution;

    float perspective = 0.0f;
    perspective += sqrt (colony->GetFarmsEvolutionPoints ()) * 0.4f;
    perspective += sqrt (colony->GetMinesEvolutionPoints ()) * 0.4f;
    perspective += sqrt (colony->GetIndustryEvolutionPoints ()) * 0.75f;
    perspective += sqrt (colony->GetDefenseEvolutionPoints ());

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

    colony->SetLogisticsEvolutionPoints (colony->GetLogisticsEvolutionPoints () + coloniesBasicEvolution_ * modifer * timeStep);
}

void ColoniesManager::ProcessColonyDefenseEvolution (District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyDefenseEvolution = colony->GetDefenseEvolutionPoints ();
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

    colony->SetDefenseEvolutionPoints (colony->GetDefenseEvolutionPoints () + coloniesBasicEvolution_ * modifer * timeStep);
}

ColoniesManager::ColoniesManager (Urho3D::Context *context) : Urho3D::Component (context),
    coloniesBasicPopulationIncrease_ (0.005f),
    coloniesBasicEvolution_ (0.002f),
    canBePlantedByOneColonist_ (0.5f),
    forestCanBeCuttedByOneColonist_ (0.025f),
    investitionsConsumption_ (2.5f),
    investitionsEfficiency_ (5.0f),
    investitions_ ()
{
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (ColoniesManager, Update));
}

ColoniesManager::~ColoniesManager ()
{

}

void ColoniesManager::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <ColoniesManager> (COLONIZATION_SERVER_ONLY_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Colonies Basic Population Increase", GetColoniesBasicPopulationIncrease,
                               SetColoniesBasicPopulationIncrease, float, 0.005f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Colonies Basic Evolution", GetColoniesBasicEvolution,
                               SetColoniesBasicEvolution, float, 0.002f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Can Be Planted By One Colonist", GetCanBePlantedByOneColonist,
                               SetCanBePlantedByOneColonist, float, 0.5f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Forest Can Be Cutted By One Colonist", GetForestCanBeCuttedByOneColonist,
                               SetForestCanBeCuttedByOneColonist, float, 0.025f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Investitions Consumption", GetInvestitionsConsumption,
                               SetInvestitionsConsumption, float, 2.5f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Investitions Efficiency", GetInvestitionsEfficiency,
                               SetInvestitionsEfficiency, float, 5.0f, Urho3D::AM_DEFAULT);
}

void ColoniesManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        assert (map);
        float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();

        for (int index = 0; index < map->GetDistrictsCount (); index++)
        {
            District *district = map->GetDistrictByIndex (index);
            if (district->HasColony ())
                ProcessColony (district, timeStep);
        }
    }
}

void ColoniesManager::Invest (District *district, Urho3D::StringHash investitionType, float money)
{
    investitions_ [district->GetHash ()] [investitionType] = investitions_ [district->GetHash ()] [investitionType] + money;
}

float ColoniesManager::GetColoniesBasicPopulationIncrease () const
{
    return coloniesBasicPopulationIncrease_;
}

void ColoniesManager::SetColoniesBasicPopulationIncrease (float coloniesBasicPopulationIncrease)
{
    coloniesBasicPopulationIncrease_ = coloniesBasicPopulationIncrease;
}

float ColoniesManager::GetColoniesBasicEvolution () const
{
    return coloniesBasicEvolution_;
}

void ColoniesManager::SetColoniesBasicEvolution (float coloniesBasicEvolution)
{
    coloniesBasicEvolution_ = coloniesBasicEvolution;
}

float ColoniesManager::GetCanBePlantedByOneColonist () const
{
    return canBePlantedByOneColonist_;
}

void ColoniesManager::SetCanBePlantedByOneColonist (float canBePlantedByOneColonist)
{
    canBePlantedByOneColonist_ = canBePlantedByOneColonist;
}

float ColoniesManager::GetForestCanBeCuttedByOneColonist () const
{
    return forestCanBeCuttedByOneColonist_;
}

void ColoniesManager::SetForestCanBeCuttedByOneColonist (float forestCanBeCuttedByOneColonist)
{
    forestCanBeCuttedByOneColonist_ = forestCanBeCuttedByOneColonist;
}

float ColoniesManager::GetInvestitionsConsumption () const
{
    return investitionsConsumption_;
}

void ColoniesManager::SetInvestitionsConsumption (float investitionsConsumption)
{
    investitionsConsumption_ = investitionsConsumption;
}

float ColoniesManager::GetInvestitionsEfficiency () const
{
    return investitionsEfficiency_;
}

void ColoniesManager::SetInvestitionsEfficiency (float investitionsEfficiency)
{
    investitionsEfficiency_ = investitionsEfficiency;
}
}
