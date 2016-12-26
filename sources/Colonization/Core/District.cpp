#include <Colonization/BuildConfiguration.hpp>
#include "District.hpp"
#include <Urho3D/IO/Log.h>
#include <Colonization/Core/Map.hpp>

namespace Colonization
{
float District::forestsReproductivity() const
{
    return forestsReproductivity_;
}

void District::setForestsReproductivity(float forestsReproductivity)
{
    forestsReproductivity_ = forestsReproductivity;
}

District::District (Urho3D::Context *context) : Urho3D::LogicComponent (context),
    hash_ ("nothing"),
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
    climate_ (CLIMATE_TEMPERATE),

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
    menCount_ (0),
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

    if (dataNode->GetVar ("hash").GetStringHash () != hash_)
        dataNode->SetVar ("hash", hash_);

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

    if (dataNode->GetVar ("mansCount").GetInt () != menCount_)
        dataNode->SetVar ("mansCount", menCount_);

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
    hash_ = dataNode->GetVar ("hash").GetStringHash ();
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
    menCount_ = dataNode->GetVar ("mansCount").GetInt ();
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

void District::UpdateHash (Map *owner)
{
    do
        hash_ = Urho3D::StringHash (name_ + Urho3D::String (Urho3D::Random (0, 1000)));
    while (owner->GetDistrictByHash (hash_) != this);
}

Urho3D::StringHash District::GetHash ()
{
    return hash_;
}

void District::SetHash(Urho3D::StringHash hash)
{
    hash_ = hash;
}

bool District::IsSea ()
{
    return isSea_;
}

bool District::SetIsSea (bool isSea)
{
    isSea_ = isSea;
}

bool District::IsImpassable ()
{
    return isImpassable_;
}

void District::SetIsImpassable (bool isImpassable)
{
    isImpassable_ = isImpassable;
}

Urho3D::String District::GetName ()
{
    return name_;
}

void District::SetName (Urho3D::String name)
{
    name_ = name;
}

Urho3D::PODVector <Urho3D::Vector3> District::GetPolygonPoints ()
{
    return polygonPoints_;
}

void District::SetPolygonPoints (Urho3D::PODVector <Urho3D::Vector3> polygonPoints)
{
    polygonPoints_ = polygonPoints;
}

Urho3D::VariantVector &District::GetPolygonPointsAttribute ()
{
    Urho3D::VariantVector variantVector;
    for (int index = 0; index < polygonPoints_.Size (); index++)
        variantVector.Push (polygonPoints_.At (index));
    return variantVector;
}

void District::SetPolygonPointsAttribute (Urho3D::VariantVector polygonPoints)
{
    polygonPoints_.Clear ();
    for (int index = 0; index < polygonPoints.Size (); index++)
        polygonPoints_.Push (polygonPoints.At (index).GetVector3 ());
}

Urho3D::Vector3 District::GetUnitPosition ()
{
    return unitPosition_;
}

void District::SetUnitPosition (Urho3D::Vector3 unitPosition)
{
    unitPosition_ = unitPosition;
}

Urho3D::Vector3 District::GetColonyPosition ()
{
    return colonyPosition_;
}

void District::SetColonyPosition (Urho3D::Vector3 colonyPosition)
{
    colonyPosition_ = colonyPosition;
}

Urho3D::PODVector <Urho3D::StringHash> District::GetNeighborsHashes ()
{
    return neighbors_;
}

void District::SetNeighborsHashes (Urho3D::PODVector <Urho3D::StringHash> neighbors)
{
    neighbors_ = neighbors;
}

Urho3D::VariantVector District::GetNeighborsHashesAttribute ()
{
    Urho3D::VariantVector variantVector;
    for (int index = 0; index < neighbors_.Size (); index++)
        variantVector.Push (neighbors_.At (index));
    return variantVector;
}

void District::SetNeighborsHashesAttribute (Urho3D::VariantVector neighbors)
{
    neighbors_.Clear ();
    for (int index = 0; index < neighbors.Size (); index++)
        neighbors_.Push (neighbors.At (index).GetStringHash ());
}

float District::GetFarmingSquare ()
{
    return farmingSquare_;
}

void District::SetFarmingSquare (float farmingSquare)
{
    farmingSquare_ = farmingSquare;
}

float District::GetForestsSquare ()
{
    return forestsSquare_;
}

void District::SetForestsSquare (float forestsSquare)
{
    forestsSquare_ = forestsSquare;
}

float District::GetLandAverageFertility ()
{
    return landAverageFertility_;
}

void District::SetLandAverageFertility (float landAverageFertility)
{
    landAverageFertility_ = landAverageFertility;
}

ClimateType District::GetClimate ()
{
    return climate_;
}

void District::SetClimate (ClimateType climate)
{
    climate_ = climate;
}

float District::GetForestsReproductivity ()
{
    return forestsReproductivity_;
}

void District::SetForestsReproductivity (float forestsReproductivity)
{
    forestsReproductivity_ = forestsReproductivity;
}

bool District::HasCoalDeposits ()
{
    return hasCoalDeposits_;
}

void District::SetCoalDepositits (bool hasCoalDeposits)
{
    hasCoalDeposits_ = hasCoalDeposits;
}

bool District::HasIronDeposits ()
{
    return hasIronDeposits_;
}

void District::SetIronDeposits (bool hasIronDeposits)
{
    hasIronDeposits_ = hasIronDeposits;
}

bool District::HasSilverDeposits ()
{
    return hasSilverDeposits_;
}

void District::SetSilverDeposits (bool hasSilverDeposits)
{
    hasSilverDeposits_ = hasSilverDeposits;
}

bool District::HasGoldDeposits ()
{
    return hasGoldDeposits_;
}

void District::SetGoldDepositions (bool hasGoldDeposits)
{
    hasGoldDeposits_ = hasGoldDeposits;
}

float District::GetNativesCount ()
{
    return nativesCount_;
}

void District::SetNativesCount (float nativesCount)
{
    nativesCount_ = nativesCount;
}

float District::GetNativesFightingTechnologyLevel ()
{
    return nativesFightingTechnologyLevel_;
}

void District::SetNativesFightingTechnologyLevel (float nativesFightingTechnologyLevel)
{
    nativesFightingTechnologyLevel_ = nativesFightingTechnologyLevel;
}

float District::GetNativesAggressiveness ()
{
    return nativesAggressiveness_;
}

void District::SetNativesAggressivness (float nativesAggressiveness)
{
    nativesAggressiveness_ = nativesAggressiveness;
}

NativesCharacter District::GetNativesCharacter ()
{
    return nativesCharacter_;
}

void District::SetNativesCharacter (NativesCharacter nativesCharacter)
{
    nativesCharacter_ = nativesCharacter;
}

bool District::HasColony ()
{
    return hasColony_;
}

void District::SetColony (bool hasColony)
{
    hasColony_ = hasColony;
}

Urho3D::String District::GetColonyOwnerName ()
{
    return colonyOwnerName_;
}

void District::SetColonyOwnerName (Urho3D::String colonyOwnerName)
{
    colonyOwnerName_ = colonyOwnerName;
}

float District::GetMenCount ()
{
    return menCount_;
}

void District::SetMenCount (float menCount)
{
    menCount_ = menCount;
}

float District::GetWomenCount ()
{
    return womenCount_;
}

void District::SetWomenCount (float womenCount)
{
    womenCount_ = womenCount;
}

float District::GetLocalArmySize ()
{
    return localArmySize_;
}

void District::SetLocalArmySize (float localArmySize)
{
    localArmySize_ = localArmySize;
}

float District::GetFarmsEvolutionPoints ()
{
    return farmsEvolutionPoints_;
}

void District::SetFarmsEvolutionPoints (float farmsEvolutionPoints)
{
    farmsEvolutionPoints_ = farmsEvolutionPoints;
}

float District::GetMinesEvolutionPoints ()
{
    return minesEvolutionPoints_;
}

void District::SetMinesEvolutionPoints (float minesEvolutionPoints)
{
    minesEvolutionPoints_ = minesEvolutionPoints;
}

float District::GetIndustryEvolutionPoints ()
{
    return industryEvolutionPoints_;
}

void District::SetIndustryEvolutionPoints (float industryEvolutionPoints)
{
    industryEvolutionPoints_ = industryEvolutionPoints;
}

float District::GetLogisticsEvolutionPoints ()
{
    return logisticsEvolutionPoints_;
}

void District::SetLogisticsEvolutionPoints (float logisticsEvolutionPoints)
{
    logisticsEvolutionPoints_ = logisticsEvolutionPoints;
}

float District::GetDefenseEvolutionPoints ()
{
    return defenseEvolutionPoints_;
}

void District::SetDefenseEvolutionPoints (float defenseEvolutionPoints)
{
    defenseEvolutionPoints_ = defenseEvolutionPoints;
}

float District::GetAverageLevelOfLifePoints ()
{
    return averageLevelOfLifePoints_;
}

void District::SetAverageLevelOfLifePoint (float averageLevelOfLifePoints)
{
    averageLevelOfLifePoints_ = averageLevelOfLifePoints;
}
}
