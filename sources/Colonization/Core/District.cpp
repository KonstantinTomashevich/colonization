#include <Colonization/BuildConfiguration.hpp>
#include "District.hpp"

namespace Colonization
{
District::District (Urho3D::Context *context) : Urho3D::Object (context),
    needDataUpdate_ (true),
    isSea_ (true),
    isImpassable_ (false),
    name_ ("Unknown"),
    polygonPoints_ (),
    unitPosition_ (),
    colonyPosition_ (),
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
    localArmySize_ (0),
    farmsEvolutionPoints_ (0.0f),
    minesEvolutionPoints_ (0.0f),
    industryEvolutionPoints_ (0.0f),
    logisticsEvolutionPoints_ (0.0f),
    defenseEvolutionPoints_ (0.0f),
    averageLevelOfLifePoints_ (0.0f)
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
        for (int index = 0; index < polygonPoints_.Size (); index++)
            polygonPointsVariants.Push (polygonPoints_.At (index));
        dataNode->SetVar ("polygonPoints", polygonPointsVariants);
    }

    if (dataNode->GetVar ("name").GetString () != name_)
        dataNode->SetVar ("name", name_);

    if (dataNode->GetVar ("isSea").GetBool () != isSea_)
        dataNode->SetVar ("isSea", isSea_);

    if (dataNode->GetVar ("isImpassable").GetBool () != isImpassable_)
        dataNode->SetVar ("isImpassable", isImpassable_);

    if (dataNode->GetVar ("unitPosition").GetVector3 () != unitPosition_)
        dataNode->SetVar ("unitPosition", unitPosition_);

    if (dataNode->GetVar ("colonyPosition").GetVector3 () != colonyPosition_)
        dataNode->SetVar ("colonyPosition", colonyPosition_);

    if (dataNode->GetVar ("farmingSquare").GetFloat () != farmingSquare_)
        dataNode->SetVar ("farmingSquare", farmingSquare_);

    if (dataNode->GetVar ("forestsSquare").GetFloat () != forestsSquare_)
        dataNode->SetVar ("forestsSquare", forestsSquare_);

    if (dataNode->GetVar ("landAverageFertility").GetFloat () != landAverageFertility_)
        dataNode->SetVar ("landAverageFertility", landAverageFertility_);

    if (static_cast <ClimateType> (dataNode->GetVar ("climate").GetInt ()) != climate_)
        dataNode->SetVar ("climate", static_cast <int> (climate_));

    if (dataNode->GetVar ("forestsReproductivity").GetFloat () != forestsReproductivity_)
        dataNode->SetVar ("forestsReproductivity", forestsReproductivity_);

    if (dataNode->GetVar ("hasCoalDeposits").GetBool () != hasCoalDeposits_)
        dataNode->SetVar ("hasCoalDeposits", hasCoalDeposits_);

    if (dataNode->GetVar ("hasIronDeposits").GetBool () != hasIronDeposits_)
        dataNode->SetVar ("hasIronDeposits", hasIronDeposits_);

    if (dataNode->GetVar ("hasSilverDeposits").GetBool () != hasSilverDeposits_)
        dataNode->SetVar ("hasSilverDeposits", hasSilverDeposits_);

    if (dataNode->GetVar ("hasGoldDeposits").GetBool () != hasGoldDeposits_)
        dataNode->SetVar ("hasGoldDeposits", hasGoldDeposits_);

    if (dataNode->GetVar ("nativesCount").GetInt () != nativesCount_)
        dataNode->SetVar ("nativesCount", nativesCount_);

    if (dataNode->GetVar ("nativesFightingTechnologyLevel").GetInt () != nativesFightingTechnologyLevel_)
        dataNode->SetVar ("nativesFightingTechnologyLevel", nativesFightingTechnologyLevel_);

    if (dataNode->GetVar ("nativesAggressiveness").GetFloat () != nativesAggressiveness_)
        dataNode->SetVar ("nativesAggressiveness", nativesAggressiveness_);

    if (static_cast <NativesCharacter> (dataNode->GetVar ("nativesCharacter").GetInt ()) != nativesCharacter_)
        dataNode->SetVar ("nativesCharacter", static_cast <int> (nativesCharacter_));

    if (dataNode->GetVar ("hasColony").GetBool () != hasColony_)
        dataNode->SetVar ("hasColony", hasColony_);

    if (dataNode->GetVar ("colonyOwnerName").GetString () != colonyOwnerName_)
        dataNode->SetVar ("colonyOwnerName", colonyOwnerName_);

    if (dataNode->GetVar ("mansCount").GetInt () != mansCount_)
        dataNode->SetVar ("mansCount", mansCount_);

    if (dataNode->GetVar ("womenCount").GetInt () != womenCount_)
        dataNode->SetVar ("womenCount", womenCount_);

    if (dataNode->GetVar ("localArmySize").GetInt () != localArmySize_)
        dataNode->SetVar ("localArmySize", localArmySize_);

    if (dataNode->GetVar ("farmsEvolutionPoints").GetFloat () != farmsEvolutionPoints_)
        dataNode->SetVar ("farmsEvolutionPoints", farmsEvolutionPoints_);

    if (dataNode->GetVar ("minesEvolutionPoints").GetFloat () != minesEvolutionPoints_)
        dataNode->SetVar ("minesEvolutionPoints", minesEvolutionPoints_);

    if (dataNode->GetVar ("industryEvolutionPoints").GetFloat () != industryEvolutionPoints_)
        dataNode->SetVar ("industryEvolutionPoints", industryEvolutionPoints_);

    if (dataNode->GetVar ("logisticsEvolutionPoints").GetFloat () != logisticsEvolutionPoints_)
        dataNode->SetVar ("logisticsEvolutionPoints", logisticsEvolutionPoints_);

    if (dataNode->GetVar ("defenseEvolutionPoints").GetFloat () != defenseEvolutionPoints_)
        dataNode->SetVar ("defenseEvolutionPoints", defenseEvolutionPoints_);

    if (dataNode->GetVar ("averageLevelOfLifePoints").GetFloat () != averageLevelOfLifePoints_)
        dataNode->SetVar ("averageLevelOfLifePoints", averageLevelOfLifePoints_);
}

void District::ReadDataFromNode (Urho3D::Node *dataNode)
{
    assert (dataNode);
    name_ = dataNode->GetVar ("name").GetString ();
    Urho3D::VariantVector polygonPointsVariants = dataNode->GetVar ("polygonPoints").GetVariantVector ();
    polygonPoints_.Clear ();
    for (int index = 0; index < polygonPointsVariants.Size (); index++)
        polygonPoints_.Push (polygonPointsVariants.At (index).GetVector3 ());

    unitPosition_ = dataNode->GetVar ("unitPosition").GetVector3 ();
    colonyPosition_ = dataNode->GetVar ("colonyPosition").GetVector3 ();
    isSea_ = dataNode->GetVar ("isSea").GetBool ();
    isImpassable_ = dataNode->GetVar ("isImpassable").GetBool ();

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
    localArmySize_ = dataNode->GetVar ("localArmySize").GetInt ();
    farmsEvolutionPoints_ = dataNode->GetVar ("farmsEvolutionPoints").GetFloat ();
    minesEvolutionPoints_ = dataNode->GetVar ("minesEvolutionPoints").GetFloat ();
    industryEvolutionPoints_ = dataNode->GetVar ("industryEvolutionPoints").GetFloat ();
    logisticsEvolutionPoints_ = dataNode->GetVar ("logisticsEvolutionPoints").GetFloat ();
    defenseEvolutionPoints_ = dataNode->GetVar ("defenseEvolutionPoints").GetFloat ();
    averageLevelOfLifePoints_ = dataNode->GetVar ("averageLevelOfLifePoints").GetFloat ();
}

void District::CalculateNeighbors (Urho3D::PODVector <District *> &allDistricts)
{
    assert (!allDistricts.Empty ());
    neighbors_.Clear ();
    for (int index = 0; index < allDistricts.Size (); index++)
    {
        District *another = allDistricts.At (index);
        assert (another);

        if (another != this && !another->polygonPoints_.Empty () && !polygonPoints_.Empty ())
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
