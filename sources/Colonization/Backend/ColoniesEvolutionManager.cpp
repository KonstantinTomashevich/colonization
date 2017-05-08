#include <Colonization/BuildConfiguration.hpp>
#include "ColoniesEvolutionManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <cmath>
#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
float ColoniesEvolutionManager::GetTotalColonyEvolution(District *colony)
{
    return colony->GetFarmsEvolutionPoints () + colony->GetMinesEvolutionPoints () +
            colony->GetIndustryEvolutionPoints () + colony->GetLogisticsEvolutionPoints () +
            colony->GetDefenseEvolutionPoints ();
}

void ColoniesEvolutionManager::ProcessColony (GameConfiguration *configuration, District *colony, float timeStep)
{
    float updatePoints = 0.0f;
    updatePoints += ProcessColonyPopulation (configuration, colony, timeStep);
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

float ColoniesEvolutionManager::ProcessColonyPopulation (GameConfiguration *configuration, District *colony, float timeStep)
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

float ColoniesEvolutionManager::ProcessColonyFarmsEvolution (GameConfiguration *configuration, District *colony, float timeStep)
{
    // TODO:
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyFarmsEvolution = colony->GetFarmsEvolutionPoints ();
    float farmsEvolutionInColonyEvolution = colonyFarmsEvolution / totalColonyEvolution;

    float canBePlanted = (colony->GetMenCount () + colony->GetWomenCount ()) * farmsEvolutionInColonyEvolution *
            configuration->GetCanBePlantedByOneColonist () * sqrt (colonyFarmsEvolution);

    // TODO: Remove all magic numbers there!
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

    // TODO: Farms evolution is inbalanced. Rewrite it.
    float evolutionModifer = 2.0f * (colony->GetFarmingSquare () - canBePlanted) / colony->GetFarmingSquare ();
    evolutionModifer *= colony->GetLandAverageFertility ();
    evolutionModifer *= climateModifer;

    float balance = colony->GetFarmsBalance ();
    if (balance > 0.0f)
    {
        evolutionModifer *= configuration->GetBasicEvolutionSpeed ();
    }
    else
    {
        evolutionModifer = -configuration->GetBasicDegradationSpeed () / evolutionModifer;
    }

    float oldFarmsEvolution = colony->GetFarmsEvolutionPoints ();
    float evolutionAddition = evolutionModifer * timeStep;

    if (evolutionAddition > 0.0f)
    {
        colony->SetFarmsEvolutionPoints (oldFarmsEvolution + evolutionAddition);
        float evolutionCost = configuration->GetEvolutionCostPerLevel () * evolutionAddition;
        colony->SetFarmsBalance (balance - evolutionCost);
    }
    else
    {
        float balanceAddition = -configuration->GetDegradationCostPerLevel () * evolutionAddition;
        colony->SetFarmsBalance (balance + balanceAddition);
        if (oldFarmsEvolution > 1.0f)
        {
            colony->SetFarmsEvolutionPoints (oldFarmsEvolution + evolutionAddition);
        }
    }
    return (evolutionAddition * 1500.0f);
}

float ColoniesEvolutionManager::ProcessColonyMinesEvolution (GameConfiguration *configuration, District *colony, float timeStep)
{
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

    perspective = sqrt (perspective);
    float balance = colony->GetMinesBalance ();
    if (balance > 0.0f)
    {
        perspective *= configuration->GetBasicEvolutionSpeed ();
    }
    else
    {
        perspective = -configuration->GetBasicDegradationSpeed () / perspective;
    }
    float oldMinesEvolution = colony->GetMinesEvolutionPoints ();
    float evolutionAddition = perspective * timeStep;

    if (evolutionAddition > 0.0f)
    {
        colony->SetMinesEvolutionPoints (oldMinesEvolution + evolutionAddition);
        float evolutionCost = configuration->GetEvolutionCostPerLevel () * evolutionAddition;
        colony->SetMinesBalance (balance - evolutionCost);
    }
    else
    {
        float balanceAddition = -configuration->GetDegradationCostPerLevel () * evolutionAddition;
        colony->SetMinesBalance (balance + balanceAddition);
        if (oldMinesEvolution > 1.0f)
        {
            colony->SetMinesEvolutionPoints (oldMinesEvolution + evolutionAddition);
        }
    }
    return (evolutionAddition * 1500.0f);
}

float ColoniesEvolutionManager::ProcessColonyIndustryEvolution (GameConfiguration *configuration, District *colony, float timeStep)
{
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

    perspective = sqrt (perspective);
    float balance = colony->GetIndustryBalance ();
    if (balance > 0.0f)
    {
        perspective *= configuration->GetBasicEvolutionSpeed ();
    }
    else
    {
        perspective = -configuration->GetBasicDegradationSpeed () / perspective;
    }

    float oldIndustryEvolution = colony->GetIndustryEvolutionPoints ();
    float evolutionAddition = perspective * timeStep;

    if (evolutionAddition > 0.0f)
    {
        colony->SetIndustryEvolutionPoints (oldIndustryEvolution + evolutionAddition);
        float evolutionCost = configuration->GetEvolutionCostPerLevel () * evolutionAddition;
        colony->SetIndustryBalance (balance - evolutionCost);
    }
    else
    {
        float balanceAddition = -configuration->GetDegradationCostPerLevel () * evolutionAddition;
        colony->SetIndustryBalance (balance + balanceAddition);
        if (oldIndustryEvolution > 1.0f)
        {
            colony->SetIndustryEvolutionPoints (oldIndustryEvolution + evolutionAddition);
        }
    }
    return (evolutionAddition * 1500.0f);
}

float ColoniesEvolutionManager::ProcessColonyLogisticsEvolution (GameConfiguration *configuration, District *colony, float timeStep)
{
    float totalColonyEvolution = GetTotalColonyEvolution (colony);
    float colonyLogisticsEvolution = colony->GetLogisticsEvolutionPoints ();
    float logisticsEvolutionInColonyEvolution = colonyLogisticsEvolution / totalColonyEvolution;

    float perspective = 0.0f;
    perspective += sqrt (colony->GetFarmsEvolutionPoints ()) * 0.4f;
    perspective += sqrt (colony->GetMinesEvolutionPoints ()) * 0.4f;
    perspective += sqrt (colony->GetIndustryEvolutionPoints ()) * 0.75f;
    perspective += sqrt (colony->GetDefenseEvolutionPoints ());

    perspective = sqrt (perspective);
    float balance = colony->GetLogisticsBalance ();
    if (balance > 0.0f)
    {
        perspective *= configuration->GetBasicEvolutionSpeed ();
    }
    else
    {
        perspective = -configuration->GetBasicDegradationSpeed () / perspective;
    }

    float oldLogisticsEvolution = colony->GetLogisticsEvolutionPoints ();
    float evolutionAddition = perspective * timeStep;

    if (evolutionAddition > 0.0f)
    {
        colony->SetLogisticsEvolutionPoints (oldLogisticsEvolution + evolutionAddition);
        float evolutionCost = configuration->GetEvolutionCostPerLevel () * evolutionAddition;
        colony->SetLogisticsBalance (balance - evolutionCost);
    }
    else
    {
        float balanceAddition = -configuration->GetDegradationCostPerLevel () * evolutionAddition;
        colony->SetLogisticsBalance (balance + balanceAddition);
        if (oldLogisticsEvolution > 1.0f)
        {
            colony->SetLogisticsEvolutionPoints (oldLogisticsEvolution + evolutionAddition);
        }
    }
    return (evolutionAddition * 1500.0f);
}

float ColoniesEvolutionManager::ProcessColonyDefenseEvolution (GameConfiguration *configuration, District *colony, float timeStep)
{
    float perspective = 1.0f;
    float balance = colony->GetDefenseBalance ();
    if (balance > 0.0f)
    {
        perspective *= configuration->GetBasicEvolutionSpeed ();
    }
    else
    {
        perspective = -configuration->GetBasicDegradationSpeed () / perspective;
    }

    float oldDefenseEvolution = colony->GetDefenseEvolutionPoints ();
    float evolutionAddition = perspective * timeStep;

    if (evolutionAddition > 0.0f)
    {
        colony->SetDefenseEvolutionPoints (oldDefenseEvolution + evolutionAddition);
        float evolutionCost = configuration->GetEvolutionCostPerLevel () * evolutionAddition;
        colony->SetDefenseBalance (balance - evolutionCost);
    }
    else
    {
        float balanceAddition = -configuration->GetDegradationCostPerLevel () * evolutionAddition;
        colony->SetDefenseBalance (balance + balanceAddition);
        if (oldDefenseEvolution > 1.0f)
        {
            colony->SetDefenseEvolutionPoints (oldDefenseEvolution + evolutionAddition);
        }
    }
    return (evolutionAddition * 1500.0f);
}

void ColoniesEvolutionManager::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (ColoniesEvolutionManager, Update));
}

ColoniesEvolutionManager::ColoniesEvolutionManager (Urho3D::Context *context) : Urho3D::Component (context)
{

}

ColoniesEvolutionManager::~ColoniesEvolutionManager ()
{

}

void ColoniesEvolutionManager::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <ColoniesEvolutionManager> (COLONIZATION_SERVER_ONLY_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void ColoniesEvolutionManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
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
}
