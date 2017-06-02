#include "TestBasicColonyBalance.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea/InternalTradeArea.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestBasicColonyBalanceApplication)
namespace Tests
{
TestBasicColonyBalanceApplication::TestBasicColonyBalanceApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestBasicColonyBalanceApplication::~TestBasicColonyBalanceApplication ()
{

}

void TestBasicColonyBalanceApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "TestBasicColonyBalance.log";
    engineParameters_ ["WindowTitle"] = "TestBasicColonyBalance";
    engineParameters_ ["ResourcePrefixPaths"] = "..;.";
}

void TestBasicColonyBalanceApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Set logging level
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetLevel (Urho3D::LOG_DEBUG);

    Colonization::RegisterAllObjects (context_);
    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    scene->CreateComponent <Colonization::GameConfiguration> ()->ResetToDefault ();
    scene->CreateChild ("units");
    Colonization::Map *map = scene->CreateChild ("map")->CreateComponent <Colonization::Map> ();
    const int mapWidth = 2;
    const int mapHeight = 2;

    for (int x = 0; x < mapWidth; x++)
    {
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

            district->SetUnitPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f));
            if (x == 1 && y == 1)
            {
                district->SetColonyPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f));
                district->SetIsSea (false);

                district->SetUnitPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f));
                district->SetColonyPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f));
                district->SetHasColony (true);
                district->SetColonyOwnerName ("PlayerX");
                district->SetMenCount (50.0f);
                district->SetWomenCount (50.0f);

                district->SetFarmsEvolutionPoints (1.0f);
                district->SetMinesEvolutionPoints (1.0f);
                district->SetIndustryEvolutionPoints (1.0f);
                district->SetLogisticsEvolutionPoints (1.0f);
                district->SetDefenseEvolutionPoints (1.0f);
            }
            else
            {
                district->SetIsSea (true);
            }
        }
    }
    map->RecalculateDistrictsNeighbors ();
    // array (X * HEIGHT + Y) = (X, Y)

    Colonization::InternalTradeArea *internalTradeArea = scene->CreateComponent <Colonization::InternalTradeArea> ();
    internalTradeArea->AddDistrictHash (map->GetDistrictByIndex (1 * mapHeight + 1)->GetHash ());
    Urho3D::SharedPtr <Colonization::TradeDistrictProcessingInfo> result = internalTradeArea->ProcessTrade (map, 10.0f, true);

    float soldGoodsCost = result->GetSoldTradeGoodsCost ();
    float unsoldGoodsCost = result->GetUnsoldTradeGoodsCost ();
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Sold goods cost: " + Urho3D::String (soldGoodsCost));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Unsold goods cost: " + Urho3D::String (unsoldGoodsCost));

    Colonization::District *colony = map->GetDistrictByIndex (1 * mapHeight + 1);
    float farmsBalance = colony->GetFarmsBalance ();
    float minesBalance = colony->GetMinesBalance ();
    float industryBalance = colony->GetIndustryBalance ();
    float logisticsBalance = colony->GetLogisticsBalance ();
    float defenseBalance = colony->GetDefenseBalance ();

    Urho3D::Log::Write (Urho3D::LOG_INFO, "Farms balance after trade: " + Urho3D::String (farmsBalance));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Mines balance after trade: " + Urho3D::String (minesBalance));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Industry balance after trade: " + Urho3D::String (industryBalance));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Logistics balance after trade: " + Urho3D::String (logisticsBalance));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Defense balance after trade: " + Urho3D::String (defenseBalance));

    if (unsoldGoodsCost / (soldGoodsCost + unsoldGoodsCost) < 0.35f ||
            unsoldGoodsCost / (soldGoodsCost + unsoldGoodsCost) > 0.75f)
    {
        ErrorExit ("Expected than unsold trade goods cost is in 35%-75% of total trade, but it is " +
                   Urho3D::String (unsoldGoodsCost * 100.0f / (soldGoodsCost + unsoldGoodsCost)) + "%.");
    }
    else if (farmsBalance / minesBalance < 0.75f || farmsBalance / minesBalance > 1.25f ||
             farmsBalance / industryBalance < 0.75f || farmsBalance / industryBalance > 1.25f ||
             farmsBalance / logisticsBalance < 0.75f || farmsBalance / logisticsBalance > 1.25f ||
             farmsBalance / defenseBalance < 0.75f || farmsBalance / defenseBalance > 1.25f)
    {
        ErrorExit ("Evolution brances balances are so inbalanced!");
    }
    else
    {
        engine_->Exit ();
    }
}

void TestBasicColonyBalanceApplication::Stop ()
{

}
}
