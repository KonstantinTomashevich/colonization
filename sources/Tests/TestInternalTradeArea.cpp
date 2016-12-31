#include "TestInternalTradeArea.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea.hpp>
#include <Colonization/Utils/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestInternalTradeAreaApplication)
namespace Tests
{
TestInternalTradeAreaApplication::TestInternalTradeAreaApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestInternalTradeAreaApplication::~TestInternalTradeAreaApplication ()
{

}

void TestInternalTradeAreaApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "TestInternalTradeArea.log";
    engineParameters_ ["WindowTitle"] = "TestInternalTradeArea";
}

void TestInternalTradeAreaApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Set logging level
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetLevel (Urho3D::LOG_DEBUG);

    context_->SetGlobalVar ("oneColonistFarmsProductionConsumption", 0.01f);
    context_->SetGlobalVar ("farmsProductionMinesConsumption", 0.005f);
    context_->SetGlobalVar ("farmsProductionIndustryConsumption", 0.3f);

    context_->SetGlobalVar ("oneColonistMinesProductionConsumption", 0.002f);
    context_->SetGlobalVar ("minesProductionFarmsConsumption", 0.2f);
    context_->SetGlobalVar ("minesProductionIndustryConsumption", 0.9f);

    context_->SetGlobalVar ("oneColonistIndustryProductionConsumption", 0.003f);
    context_->SetGlobalVar ("industryProductionFarmsConsumption", 0.15f);
    context_->SetGlobalVar ("industryProductionMinesConsumption", 0.25f);

    context_->SetGlobalVar ("farmsProductionInternalCost", 1.0f);
    context_->SetGlobalVar ("minesProductionInternalCost", 1.5f);
    context_->SetGlobalVar ("industryProductionInternalCost", 5.0f);

    context_->SetGlobalVar ("farmsProductionExternalCost", 0.5f);
    context_->SetGlobalVar ("minesProductionExternalCost", 1.0f);
    context_->SetGlobalVar ("industryProductionExternalCost", 3.0f);

    Colonization::RegisterAllObjects (context_);
    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    Colonization::Map *map = scene->CreateChild ("map")->CreateComponent <Colonization::Map> ();
    const int mapWidth = 2;
    const int mapHeight = 2;

    for (int x = 0; x < mapWidth; x++)
        for (int y = 0; y < mapHeight; y++)
        {
            Colonization::District *district = map->CreateDistrict (
                        "(" + Urho3D::String (x) + "; " + Urho3D::String (y) + ")");

            Urho3D::PODVector <Urho3D::Vector3> polygonPoints;
            polygonPoints.Push (Urho3D::Vector3 (x, 0.0f, y ));
            polygonPoints.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y));
            polygonPoints.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y + 1.0f));
            polygonPoints.Push (Urho3D::Vector3 (x, 0.0f, y + 1.0f));
            district->SetPolygonPoints (polygonPoints);

            district->SetClimate (Colonization::CLIMATE_TEMPERATE);
            district->SetFarmingSquare (250.0f);
            district->SetLandAverageFertility (1.0f);
            district->SetForestsSquare (150.0f);
            district->SetForestsReproductivity (0.75f);

            district->SetUnitPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f));
            district->SetColonyPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f));
            district->SetColonyOwnerName ("PlayerX");
            district->UpdateHash (map);
        }
    map->RecalculateDistrictsNeighbors ();

    map->GetDistrictByIndex (0)->SetFarmsEvolutionPoints (9.0f);
    map->GetDistrictByIndex (0)->SetMinesEvolutionPoints (1.0f);
    map->GetDistrictByIndex (0)->SetIndustryEvolutionPoints (1.0f);
    map->GetDistrictByIndex (0)->SetLogisticsEvolutionPoints (4.0f);
    map->GetDistrictByIndex (0)->SetDefenseEvolutionPoints (1.0f);

    map->GetDistrictByIndex (1)->SetFarmsEvolutionPoints (2.0f);
    map->GetDistrictByIndex (1)->SetMinesEvolutionPoints (2.0f);
    map->GetDistrictByIndex (1)->SetIndustryEvolutionPoints (1.5f);
    map->GetDistrictByIndex (1)->SetLogisticsEvolutionPoints (4.0f);
    map->GetDistrictByIndex (1)->SetDefenseEvolutionPoints (1.0f);

    map->GetDistrictByIndex (2)->SetFarmsEvolutionPoints (2.0f);
    map->GetDistrictByIndex (2)->SetMinesEvolutionPoints (8.0f);
    map->GetDistrictByIndex (2)->SetIndustryEvolutionPoints (2.0f);
    map->GetDistrictByIndex (2)->SetLogisticsEvolutionPoints (4.0f);
    map->GetDistrictByIndex (2)->SetDefenseEvolutionPoints (1.0f);

    map->GetDistrictByIndex (3)->SetFarmsEvolutionPoints (2.0f);
    map->GetDistrictByIndex (3)->SetMinesEvolutionPoints (2.0f);
    map->GetDistrictByIndex (3)->SetIndustryEvolutionPoints (7.0f);
    map->GetDistrictByIndex (3)->SetLogisticsEvolutionPoints (4.0f);
    map->GetDistrictByIndex (3)->SetDefenseEvolutionPoints (1.0f);

    Urho3D::SharedPtr <Colonization::InternalTradeArea> internalTradeArea (new Colonization::InternalTradeArea (context_));
    internalTradeArea->AddDistrictHash (map->GetDistrictByIndex (0)->GetHash ());
    internalTradeArea->AddDistrictHash (map->GetDistrictByIndex (1)->GetHash ());
    internalTradeArea->AddDistrictHash (map->GetDistrictByIndex (2)->GetHash ());
    internalTradeArea->AddDistrictHash (map->GetDistrictByIndex (3)->GetHash ());
    Urho3D::SharedPtr <Colonization::TradeDistrictProcessingInfo> result = internalTradeArea->ProcessTrade (map);

    Urho3D::Log::Write (Urho3D::LOG_INFO, "Unused farms production: " + Urho3D::String (result->GetUnusedEvolutionPointsOf ("farms")));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Unused mines production: " + Urho3D::String (result->GetUnusedEvolutionPointsOf ("mines")));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Unused industry production: " + Urho3D::String (result->GetUnusedEvolutionPointsOf ("industry")));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Sold trade goods cost: " + Urho3D::String (result->GetSoldTradeGoodsCost ()));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Unsold trade goods cost: " + Urho3D::String (result->GetUnsoldTradeGoodsCost ()));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Logistics bonus: " + Urho3D::String (result->GetLogisticsBonus ()));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Defense bonus: " + Urho3D::String (result->GetDefenseBonus ()));

    if (result->GetUnusedEvolutionPointsOf ("farms") <= 0.0f)
        ErrorExit ("Expected unused farms production > 0.0.");
    else if (result->GetUnusedEvolutionPointsOf ("mines") >= 0.0f)
        ErrorExit ("Expected unused mines production < 0.0.");
    else if (result->GetUnusedEvolutionPointsOf ("industry") <= 0.0f)
        ErrorExit ("Expected unused industry production > 0.0.");
    else if (result->GetUnsoldTradeGoodsCost () / (result->GetUnsoldTradeGoodsCost () + result->GetSoldTradeGoodsCost ()) < 0.1f ||
             result->GetUnsoldTradeGoodsCost () / (result->GetUnsoldTradeGoodsCost () + result->GetSoldTradeGoodsCost ()) > 0.5f)
        ErrorExit ("Expected unsold / (unsold + sold) production in (0.1; 0.5).");
    else if (result->GetLogisticsBonus () >= 1.0f)
        ErrorExit ("Expected logistics bonus < 1.0.");
    else if (result->GetDefenseBonus () >= 1.0f)
        ErrorExit ("Expected logistics bonus < 1.0.");
    else
        engine_->Exit ();
}

void TestInternalTradeAreaApplication::Stop ()
{

}
}
