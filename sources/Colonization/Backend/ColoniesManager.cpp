#include <Colonization/BuildConfiguration.hpp>
#include "ColoniesManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <cmath>
#include <Colonization/Backend/NetworkUpdateCounter.hpp>
#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
float ColoniesManager::GetTotalColonyEvolution(District *colony)
{
    return colony->GetFarmsEvolutionPoints () + colony->GetMinesEvolutionPoints () +
            colony->GetIndustryEvolutionPoints () + colony->GetLogisticsEvolutionPoints () +
            colony->GetDefenseEvolutionPoints ();
}

void ColoniesManager::ProcessColony (GameConfiguration *configuration, District *colony, float timeStep)
{
    // TODO: Think about balance.
    // TODO: Maybe delete ability to cut forests? And delete forests reproductivity too.
    float updatePoints = 0.0f;
    updatePoints += ProcessColonyPopulation (configuration, colony, timeStep);
    updatePoints += ProcessColonyForests (configuration, colony, timeStep);
    updatePoints += ProcessColonyFarmsEvolution (configuration, colony, timeStep);
    updatePoints += ProcessColonyMinesEvolution (configuration, colony, timeStep);
    updatePoints += ProcessColonyIndustryEvolution (configuration, colony, timeStep);
    updatePoints += ProcessColonyLogisticsEvolution (configuration, colony, timeStep);
    updatePoints += ProcessColonyDefenseEvolution (configuration, colony, timeStep);
    // TODO: Implement average level of life calculation.
    colony->SetAverageLevelOfLifePoints (1.0f);

    NetworkUpdateCounter *counter = colony->GetNode ()->GetComponent <NetworkUpdateCounter> ();
    if (!counter)
    {
        counter = CreateNetworkUpdateCounterForComponent (colony);
    }
    counter->AddUpdatePoints (updatePoints);
}

float ColoniesManager::ProcessColonyPopulation(GameConfiguration *configuration, District *colony, float timeStep)
{
    // TODO: If colony population is very big, stop grow. Maybe automatically send colonists from it.
    float sexRatio = colony->GetMenCount () / colony->GetWomenCount ();
    float increaseModifer = 1.0f - Urho3D::Abs (1.0f - sexRatio);
    float populationIncrease = (colony->GetMenCount () * colony->GetWomenCount ()) *
            configuration->GetColoniesBasicPopulationIncrease () * increaseModifer * timeStep;

    float newPopulationSexRatio = Urho3D::Random (0.4f, 0.6f);
    float newMen = populationIncrease * newPopulationSexRatio * timeStep;
    float newWomen = populationIncrease * (1.0f - newPopulationSexRatio) * timeStep;

    float oldMenCount = colony->GetMenCount ();
    float oldWomenCount = colony->GetWomenCount ();
    colony->SetMenCount (oldMenCount + newMen);
    colony->SetWomenCount (oldWomenCount + newWomen);

    if (floor (oldMenCount) == floor (oldMenCount + newMen) &&
            floor (oldWomenCount) == floor (oldWomenCount + newWomen))
    {
        return 0.0f;
    }
    else
    {
        float points = 0.0f;
        points += newMen * 20.0f / (oldMenCount + newMen);
        points += newWomen * 20.0f / (oldWomenCount + newWomen);
        return points;
    }
}

float ColoniesManager::ProcessColonyForests (GameConfiguration *configuration, District *colony, float timeStep)
{
    if (colony->GetForestsSquare () < colony->GetFarmingSquare ())
    {
        float oldForestsSquare = colony->GetForestsSquare ();
        float forestsSquareChange = colony->GetForestsReproductivity () * timeStep;
        colony->SetForestsSquare (oldForestsSquare + forestsSquareChange);
        colony->SetFarmingSquare (oldForestsSquare - forestsSquareChange);
        return (forestsSquareChange * 10.0f / (oldForestsSquare + forestsSquareChange));
    }
    else
    {
        return 0.0f;
    }
}

float ColoniesManager::ProcessColonyFarmsEvolution (GameConfiguration *configuration, District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyFarmsEvolution = colony->GetFarmsEvolutionPoints ();
    float farmsEvolutionInColonyEvolution = colonyFarmsEvolution / totalColonyEvolution;

    float canBePlanted = (colony->GetMenCount () + colony->GetWomenCount ()) * farmsEvolutionInColonyEvolution *
            configuration->GetCanBePlantedByOneColonist () * sqrt (colonyFarmsEvolution);

    if (colony->GetForestsSquare () > (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.15f)
    {
        colony->SetForestsSquare (colony->GetForestsSquare () - 0.25f * configuration->GetForestCanBeCutDownByOneColonist () * timeStep);
        colony->SetFarmingSquare (colony->GetFarmingSquare () + 0.25f * configuration->GetForestCanBeCutDownByOneColonist () * timeStep);
    }

    float climateModifer = 1.0f;
    if (colony->GetClimate () == CLIMATE_TEMPERATE)
    {
        climateModifer = 1.0f;
    }
    else if (colony->GetClimate () == CLIMATE_TEMPERATE_CONTINENTAL)
    {
        climateModifer = 0.8f;
    }
    else if (colony->GetClimate () == CLIMATE_TROPICAL)
    {
        climateModifer = 1.5f;
    }
    else if (colony->GetClimate () == CLIMATE_HOT)
    {
        climateModifer = 1.25f;
    }
    else if (colony->GetClimate () == CLIMATE_COLD)
    {
        climateModifer = 0.5f;
    }
    else if (colony->GetClimate () == CLIMATE_DESERT)
    {
        climateModifer = 0.25f;
    }

    float investitions = investitions_ [colony->GetHash ()] ["farms"];
    float investitionsModifer = 1.0f;
    if (investitions > 0.0f)
    {
        investitionsModifer = sqrt (configuration->GetInvestitionsEfficiency ());
        investitions_ [colony->GetHash ()] ["farms"] = investitions - configuration->GetInvestitionsConsumption () * timeStep;
    }

    float evolutionModifer = (colony->GetFarmingSquare () - canBePlanted) / colony->GetFarmingSquare ();
    if (evolutionModifer > 0.0f)
    {
        evolutionModifer *= colony->GetLandAverageFertility ();
        evolutionModifer *= climateModifer;
        evolutionModifer *= investitionsModifer;
    }
    else
    {
        evolutionModifer = 0.0f;
    }

    float oldFarmsEvolution = colony->GetFarmsEvolutionPoints ();
    float evolutionAddition = configuration->GetColoniesBasicEvolution () * evolutionModifer * timeStep;
    colony->SetFarmsEvolutionPoints (oldFarmsEvolution + evolutionAddition);
    return (evolutionAddition * 1500.0f);
}

float ColoniesManager::ProcessColonyMinesEvolution (GameConfiguration *configuration, District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyMinesEvolution = colony->GetMinesEvolutionPoints ();
    float minesEvolutionInColonyEvolution = colonyMinesEvolution / totalColonyEvolution;

    float perspective = 1.0f;

    if (colony->GetForestsSquare () < (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.15f)
    {
        perspective -= 1.0;
    }
    else if (colony->GetForestsSquare () < (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.25f)
    {
        perspective -= 0.75f;
    }
    else if (colony->GetForestsSquare () > (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.5f)
    {
        perspective += 0.5f;
    }
    else if (colony->GetForestsSquare () > colony->GetForestsSquare () < (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.75f)
    {
        perspective += 1.0f;
    }

    if (colony->HasCoalDeposits ())
    {
        perspective += 0.5f;
    }
    if (colony->HasIronDeposits ())
    {
        perspective += 0.75f;
    }
    if (colony->HasSilverDeposits ())
    {
        perspective += 1.25f;
    }
    if (colony->HasGoldDeposits ())
    {
        perspective += 1.5f;
    }

    if (colony->GetForestsSquare () > (colony->GetForestsSquare () + colony->GetFarmingSquare ()) * 0.15f)
    {
        colony->SetForestsSquare (colony->GetForestsSquare () - configuration->GetForestCanBeCutDownByOneColonist () * timeStep);
        colony->SetFarmingSquare (colony->GetFarmingSquare () + configuration->GetForestCanBeCutDownByOneColonist () * timeStep);
    }

    float investitions = investitions_ [colony->GetHash ()] ["mines"];
    if (investitions > 0.0f)
    {
        perspective *= configuration->GetInvestitionsEfficiency ();
        investitions_ [colony->GetHash ()] ["mines"] = investitions - configuration->GetInvestitionsConsumption () * timeStep;
    }

    perspective += sqrt (colony->GetIndustryEvolutionPoints ());
    float modifer = sqrt (perspective);

    if (minesEvolutionInColonyEvolution > 0.25f)
    {
        modifer *= 0.25f;
    }
    else if (minesEvolutionInColonyEvolution > 0.5f)
    {
        modifer *= 0.15f;
    }
    else if (colonyMinesEvolution > 1.5f * colony->GetFarmsEvolutionPoints ())
    {
        modifer *= 0.1f;
    }
    else
    {
        modifer *= 0.5f;
    }

    float oldMinesEvolution = colony->GetMinesEvolutionPoints ();
    float evolutionAddition = configuration->GetColoniesBasicEvolution () * modifer * timeStep;
    colony->SetMinesEvolutionPoints (oldMinesEvolution + evolutionAddition);
    return (evolutionAddition * 1500.0f);
}

float ColoniesManager::ProcessColonyIndustryEvolution (GameConfiguration *configuration, District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyIndustryEvolution = colony->GetIndustryEvolutionPoints ();
    float industryEvolutionInColonyEvolution = colonyIndustryEvolution / totalColonyEvolution;

    float perspective = 0.0f;
    perspective += sqrt (colony->GetMinesEvolutionPoints ()) * 0.75f;
    perspective += sqrt (colony->GetLogisticsEvolutionPoints ()) * 0.75f;

    if (colony->HasCoalDeposits () && colony->HasIronDeposits ())
    {
        perspective += 2.0f;
    }
    else if (colony->HasCoalDeposits ())
    {
        perspective += 0.5f;
    }
    else if (colony->HasIronDeposits ())
    {
        perspective += 1.0f;
    }

    float investitions = investitions_ [colony->GetHash ()] ["industry"];
    if (investitions > 0.0f)
    {
        perspective *= configuration->GetInvestitionsEfficiency ();
        investitions_ [colony->GetHash ()] ["industry"] = investitions - configuration->GetInvestitionsConsumption () * timeStep;
    }

    float modifer = sqrt (perspective);

    if (industryEvolutionInColonyEvolution > 0.5f)
    {
        modifer *= 0.3f;
    }
    else if (industryEvolutionInColonyEvolution > 0.75f)
    {
        modifer *= 0.15f;
    }
    else if (colonyIndustryEvolution > 5.0f * colony->GetFarmsEvolutionPoints ())
    {
        modifer *= 0.1f;
    }
    else
    {
        modifer *= 0.5f;
    }

    float oldIndustryEvolution = colony->GetIndustryEvolutionPoints ();
    float evolutionAddition = configuration->GetColoniesBasicEvolution () * modifer * timeStep;
    colony->SetIndustryEvolutionPoints (oldIndustryEvolution + evolutionAddition);
    return (evolutionAddition * 1500.0f);
}

float ColoniesManager::ProcessColonyLogisticsEvolution (GameConfiguration *configuration, District *colony, float timeStep)
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
        perspective *= configuration->GetInvestitionsEfficiency ();
        investitions_ [colony->GetHash ()] ["logistics"] = investitions - configuration->GetInvestitionsConsumption () * timeStep;
    }

    float modifer = sqrt (perspective);

    if (logisticsEvolutionInColonyEvolution > 0.1f)
    {
        modifer *= 0.35f;
    }
    else if (logisticsEvolutionInColonyEvolution > 0.15f)
    {
        modifer *= 0.2f;
    }
    else if (logisticsEvolutionInColonyEvolution > 0.2f)
    {
        modifer *= 0.1f;
    }
    else if (logisticsEvolutionInColonyEvolution > 0.25f)
    {
        modifer *= 0.05f;
    }
    else if (logisticsEvolutionInColonyEvolution > 0.30f)
    {
        modifer *= 0.0f;
    }
    else
    {
        modifer *= 0.5f;
    }

    float oldLogisticsEvolution = colony->GetLogisticsEvolutionPoints ();
    float evolutionAddition = configuration->GetColoniesBasicEvolution () * modifer * timeStep;
    colony->SetLogisticsEvolutionPoints (oldLogisticsEvolution + evolutionAddition);
    return (evolutionAddition * 1500.0f);
}

float ColoniesManager::ProcessColonyDefenseEvolution (GameConfiguration *configuration, District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyDefenseEvolution = colony->GetDefenseEvolutionPoints ();
    float defenseEvolutionInColonyEvolution = colonyDefenseEvolution / totalColonyEvolution;
    float modifer = 1.0f;

    if (defenseEvolutionInColonyEvolution > 0.1f)
    {
        modifer *= 0.4f;
    }
    else if (defenseEvolutionInColonyEvolution > 0.2f)
    {
        modifer *= 0.1f;
    }
    else if (defenseEvolutionInColonyEvolution > 0.5f)
    {
        modifer *= 0.0f;
    }
    else
    {
        modifer *= 0.5f;
    }

    float investitions = investitions_ [colony->GetHash ()] ["defense"];
    if (investitions > 0.0f)
    {
        modifer *= sqrt (configuration->GetInvestitionsEfficiency ());
        investitions_ [colony->GetHash ()] ["defense"] = investitions - configuration->GetInvestitionsConsumption () * timeStep;
    }

    float oldDefenseEvolution = colony->GetDefenseEvolutionPoints ();
    float evolutionAddition = configuration->GetColoniesBasicEvolution () * modifer * timeStep;
    colony->SetDefenseEvolutionPoints (oldDefenseEvolution + evolutionAddition);
    return (evolutionAddition * 1500.0f);
}

void ColoniesManager::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (ColoniesManager, Update));
}

ColoniesManager::ColoniesManager (Urho3D::Context *context) : Urho3D::Component (context),
    investitions_ ()
{

}

ColoniesManager::~ColoniesManager ()
{

}

void ColoniesManager::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <ColoniesManager> (COLONIZATION_SERVER_ONLY_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void ColoniesManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();

        assert (map);
        assert (configuration);
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();

        for (int index = 0; index < map->GetDistrictsCount (); index++)
        {
            District *district = map->GetDistrictByIndex (index);
            if (district->HasColony ())
            {
                ProcessColony (configuration, district, timeStep);
            }
        }
    }
}

void ColoniesManager::Invest (District *district, Urho3D::StringHash investitionType, float money)
{
    investitions_ [district->GetHash ()] [investitionType] = investitions_ [district->GetHash ()] [investitionType] + money;
}
}
