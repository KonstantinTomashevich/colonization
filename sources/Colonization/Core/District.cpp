#include <Colonization/BuildConfiguration.hpp>
#include "District.hpp"

namespace Colonization
{
District::District (Urho3D::Context *context) : Urho3D::Object (context),
    needDataUpdate_ (false),
    polygonPoints_ (),
    neighbors_ (),

    farmingSquare_ (0.0f),
    forestsSquare_ (0.0f),
    landAverageFertility_ (0.0f),
    climate_ (CLIMATE_NORMAL),

    forestsReproductivity_ (0.0f),
    hasCoalDeposits_ (false),
    hasIronDeposits_ (false),
    hasSilverDeposits_ (false),
    hasGoldDeposits_ (false),

    nativesCount_ (0),
    nativesFightingTechnologyLevel_ (0),
    nativesAggressiveness_ (0.0f),
    nativesCharacter_ (NATIVES_CHARACTER_MEDIUM),

    hasColony_ (false),
    colonyOwnerName_ (),
    mansCount_ (0),
    womenCount_ (0),
    localArmyCount_ (0),
    farmsEvolutionPoints_ (0.0f),
    minesEvolutionPoints_ (0.0f),
    industryEvolutionPoints_ (0.0f),
    logisticsEvolutionPoints_ (0.0f),
    defenseEvolutionPoints_ (0.0f),
    averageStandartOfLifePoints_ (0.0f)
{

}

District::~District ()
{

}

void District::UpdateDataNode (Urho3D::Node *dataNode, bool rewritePolygonPoints)
{
    assert (dataNode);
    if (rewritePolygonPoints)
    {
        Urho3D::VariantVector polygonPointsVariants;
        polygonPoints_.Clear ();
        for (int index = 0; index < polygonPoints_.Size (); index++)
            polygonPointsVariants.Push (polygonPoints_.At (index));
        dataNode->SetVar ("polygonPoints", polygonPointsVariants);
    }

    dataNode->SetVar ("farmingSquare", farmingSquare_);
    dataNode->SetVar ("forestsSquare", forestsSquare_);
    dataNode->SetVar ("landAverageFertility", landAverageFertility_);
    dataNode->SetVar ("climate", static_cast <int> (climate_));

    dataNode->SetVar ("forestsReproductivity", forestsReproductivity_);
    dataNode->SetVar ("hasCoalDeposits", hasCoalDeposits_);
    dataNode->SetVar ("hasIronDeposits", hasIronDeposits_);
    dataNode->SetVar ("hasSilverDeposits", hasSilverDeposits_);
    dataNode->SetVar ("hasGoldDeposits", hasGoldDeposits_);

    dataNode->SetVar ("nativesCount", nativesCount_);
    dataNode->SetVar ("nativesFightingTechnologyLevel", nativesFightingTechnologyLevel_);
    dataNode->SetVar ("nativesAggressiveness", nativesAggressiveness_);
    dataNode->SetVar ("nativesCharacter", static_cast <int> (nativesCharacter_));

    dataNode->SetVar ("hasColony", hasColony_);
    dataNode->SetVar ("colonyOwnerName", colonyOwnerName_);
    dataNode->SetVar ("mansCount", mansCount_);
    dataNode->SetVar ("womenCount", womenCount_);
    dataNode->SetVar ("localArmyCount", localArmyCount_);
    dataNode->SetVar ("farmsEvolutionPoints", farmsEvolutionPoints_);
    dataNode->SetVar ("minesEvolutionPoints", minesEvolutionPoints_);
    dataNode->SetVar ("industryEvolutionPoints", industryEvolutionPoints_);
    dataNode->SetVar ("logisticsEvolutionPoints", logisticsEvolutionPoints_);
    dataNode->SetVar ("defenseEvolutionPoints", defenseEvolutionPoints_);
    dataNode->SetVar ("averageStandartOfLifePoints", averageStandartOfLifePoints_);
}

void District::ReadDataFromNode (Urho3D::Node *dataNode)
{
    assert (dataNode);
    Urho3D::VariantVector polygonPointsVariants = dataNode->GetVar ("polygonPoints").GetVariantVector ();
    polygonPoints_.Clear ();
    for (int index = 0; index < polygonPointsVariants.Size (); index++)
        polygonPoints_.Push (polygonPointsVariants.At (index).GetVector3 ());

    farmingSquare_ = dataNode->GetVar ("farmingSquare").GetFloat ();
    forestsSquare_ = dataNode->GetVar ("forestsSquare").GetFloat ();
    landAverageFertility_ = dataNode->GetVar ("landAverageFertility").GetFloat ();
    climate_ = static_cast <ClimateType> (dataNode->GetVar ("climate").GetInt ());

    forestsReproductivity_ = dataNode->GetVar ("forestsReproductivity").GetFloat ();
    hasCoalDeposits_ = dataNode->GetVar ("hasCoalDeposits").GetBool ();
    hasIronDeposits_ = dataNode->GetVar ("hasIronDeposits").GetBool ();
    hasSilverDeposits_ = dataNode->GetVar ("hasSilverDeposits").GetBool ();
    hasGoldDeposits_ = dataNode->GetVar ("hasGoldDeposits").GetBool ();

    nativesCount_ = dataNode->GetVar ("nativesCount").GetInt ();
    nativesFightingTechnologyLevel_ = dataNode->GetVar ("nativesFightingTechnologyLevel").GetInt ();
    nativesAggressiveness_ = dataNode->GetVar ("nativesAggressiveness").GetFloat ();
    nativesCharacter_ = static_cast <NativesCharacter> (dataNode->GetVar ("nativesCharacter").GetInt ());

    hasColony_ = dataNode->GetVar ("hasColony").GetBool ();
    colonyOwnerName_ = dataNode->GetVar ("colonyOwnerName").GetString ();
    mansCount_ = dataNode->GetVar ("mansCount").GetInt ();
    womenCount_ = dataNode->GetVar ("womenCount").GetInt ();
    localArmyCount_ = dataNode->GetVar ("localArmyCount").GetInt ();
    farmsEvolutionPoints_ = dataNode->GetVar ("farmsEvolutionPoints").GetFloat ();
    minesEvolutionPoints_ = dataNode->GetVar ("minesEvolutionPoints").GetFloat ();
    industryEvolutionPoints_ = dataNode->GetVar ("industryEvolutionPoints").GetFloat ();
    logisticsEvolutionPoints_ = dataNode->GetVar ("logisticsEvolutionPoints").GetFloat ();
    defenseEvolutionPoints_ = dataNode->GetVar ("defenseEvolutionPoints").GetFloat ();
    averageStandartOfLifePoints_ = dataNode->GetVar ("averageStandartOfLifePoints").GetFloat ();
}

void District::CalculateNeighbors (Urho3D::Vector <District *> &allDistricts)
{
    assert (!allDistricts.Empty ());
    neighbors_.Clear ();
    for (int index = 0; index < allDistricts.Size (); index++)
    {
        District *another = allDistricts.At (index);
        assert (another);

        if (!another->polygonPoints_.Empty () && !polygonPoints_.Empty ())
        {
            int contactsCount = 0;
            for (int anotherPolygonPointIndex = 0;
                 anotherPolygonPointIndex < another->polygonPoints_.Size (); anotherPolygonPointIndex++)

                for (int thisPolygonPointIndex = 0;
                     thisPolygonPointIndex < polygonPoints_.Size (); thisPolygonPointIndex++)

                    if (another->polygonPoints_.At (anotherPolygonPointIndex) ==
                            polygonPoints_.At (thisPolygonPointIndex))
                        contactsCount += 1;

            if (contactsCount >= 2)
                neighbors_.Push (another);
        }
    }
}
}
