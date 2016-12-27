#include "TestInternalTradeArea.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>

#include <Urho3D/IO/Log.h>
#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea.hpp>

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

    Urho3D::SharedPtr <Colonization::Map> map (new Colonization::Map (context_));
    const int mapWidth = 2;
    const int mapHeight = 2;

    for (int x = 0; x < mapWidth; x++)
        for (int y = 0; y < mapHeight; y++)
        {
            Colonization::District *district = new Colonization::District (context_);
            district->name_ = "(" + Urho3D::String (x) + "; " + Urho3D::String (y) + ")";

            district->polygonPoints_.Push (Urho3D::Vector3 (x - 0.5f, 0.0f, y - 0.5f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x + 0.5f, 0.0f, y - 0.5f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.5f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x - 0.5f, 0.0f, y + 0.5f));

            district->unitPosition_ = Urho3D::Vector3 (x, 0.0f, y);
            district->colonyPosition_ = Urho3D::Vector3 (x, 0.0f, y);

            district->isSea_ = false;
            district->hasColony_ = true;
            district->colonyOwnerName_ = "PlayerX";
            district->menCount_ = 100;
            district->womenCount_ = 100;
            map->AddDistrict (district);
            district->UpdateHash (map);
        }
    map->RecalculateDistrictsNeighbors ();

    map->GetDistrictByIndex (0)->farmsEvolutionPoints_ = 9.0f;
    map->GetDistrictByIndex (0)->minesEvolutionPoints_ = 1.0f;
    map->GetDistrictByIndex (0)->industryEvolutionPoints_ = 1.0f;
    map->GetDistrictByIndex (0)->logisticsEvolutionPoints_ = 4.0f;
    map->GetDistrictByIndex (0)->defenseEvolutionPoints_ = 1.0f;

    map->GetDistrictByIndex (1)->farmsEvolutionPoints_ = 2.0f;
    map->GetDistrictByIndex (1)->minesEvolutionPoints_ = 2.0f;
    map->GetDistrictByIndex (1)->industryEvolutionPoints_ = 1.5f;
    map->GetDistrictByIndex (1)->logisticsEvolutionPoints_ = 4.0f;
    map->GetDistrictByIndex (1)->defenseEvolutionPoints_ = 1.0f;

    map->GetDistrictByIndex (2)->farmsEvolutionPoints_ = 2.0f;
    map->GetDistrictByIndex (2)->minesEvolutionPoints_ = 8.0f;
    map->GetDistrictByIndex (2)->industryEvolutionPoints_ = 2.0f;
    map->GetDistrictByIndex (2)->logisticsEvolutionPoints_ = 4.0f;
    map->GetDistrictByIndex (2)->defenseEvolutionPoints_ = 1.0f;

    map->GetDistrictByIndex (3)->farmsEvolutionPoints_ = 2.0f;
    map->GetDistrictByIndex (3)->minesEvolutionPoints_ = 2.0f;
    map->GetDistrictByIndex (3)->industryEvolutionPoints_ = 7.0f;
    map->GetDistrictByIndex (3)->logisticsEvolutionPoints_ = 4.0f;
    map->GetDistrictByIndex (3)->defenseEvolutionPoints_ = 1.0f;

    Urho3D::SharedPtr <Colonization::InternalTradeArea> internalTradeArea (new Colonization::InternalTradeArea (context_));
    internalTradeArea->AddDistrictHash (map->GetDistrictByIndex (0)->GetHash ());
    internalTradeArea->AddDistrictHash (map->GetDistrictByIndex (1)->GetHash ());
    internalTradeArea->AddDistrictHash (map->GetDistrictByIndex (2)->GetHash ());
    internalTradeArea->AddDistrictHash (map->GetDistrictByIndex (3)->GetHash ());
    Colonization::TradeDistrictProcessingInfo result = internalTradeArea->ProcessTrade (map);

    Urho3D::Log::Write (Urho3D::LOG_INFO, "Unused farms production: " + Urho3D::String (result.unusedEvolutionPoints_ ["farms"]));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Unused mines production: " + Urho3D::String (result.unusedEvolutionPoints_ ["mines"]));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Unused industry production: " + Urho3D::String (result.unusedEvolutionPoints_ ["industry"]));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Sold trade goods cost: " + Urho3D::String (result.soldTradeGoodsCost_));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Unsold trade goods cost: " + Urho3D::String (result.unsoldTradeGoodsCost_));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Logistics bonus: " + Urho3D::String (result.logisticsBonus_));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Defense bonus: " + Urho3D::String (result.defenseBonus_));

    if (result.unusedEvolutionPoints_ ["farms"] <= 0.0f)
        ErrorExit ("Expected unused farms production > 0.0.");
    else if (result.unusedEvolutionPoints_ ["mines"] >= 0.0f)
        ErrorExit ("Expected unused mines production < 0.0.");
    else if (result.unusedEvolutionPoints_ ["industry"] <= 0.0f)
        ErrorExit ("Expected unused industry production > 0.0.");
    else if (result.unsoldTradeGoodsCost_ / (result.unsoldTradeGoodsCost_ + result.soldTradeGoodsCost_) < 0.1f ||
             result.unsoldTradeGoodsCost_ / (result.unsoldTradeGoodsCost_ + result.soldTradeGoodsCost_) > 0.5f)
        ErrorExit ("Expected unsold / (unsold + sold) production in (0.1; 0.5).");
    else if (result.logisticsBonus_ >= 1.0f)
        ErrorExit ("Expected logistics bonus < 1.0.");
    else if (result.defenseBonus_ >= 1.0f)
        ErrorExit ("Expected logistics bonus < 1.0.");
    else
        engine_->Exit ();
}

void TestInternalTradeAreaApplication::Stop ()
{

}
}
