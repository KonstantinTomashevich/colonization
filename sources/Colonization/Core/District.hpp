#pragma once
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Container/Str.h>

namespace Colonization
{
class Map;
enum ClimateType
{
    CLIMATE_TROPICAL = 0,
    CLIMATE_HOT = 1,
    CLIMATE_TEMPERATE = 2,
    CLIMATE_TEMPERATE_CONTINENTAL = 3,
    CLIMATE_DESERT = 4,
    CLIMATE_COLD = 5,
};

enum NativesCharacter
{
    NATIVES_CHARACTER_FRIENDLY = 0,
    NATIVES_CHARACTER_MEDIUM = 1,
    NATIVES_CHARACTER_COLD = 2,
    NATIVES_CHARACTER_ISOLATIONIST = 3,
    NATIVES_CHARATER_AGRESSIVE = 4
};

class District : public Urho3D::Object
{
URHO3D_OBJECT (District, Object)
protected:
    Urho3D::StringHash hash_;
    bool isSea_;
    bool isImpassable_;
    Urho3D::String name_;
    Urho3D::PODVector <Urho3D::Vector3> polygonPoints_;
    Urho3D::Vector3 unitPosition_;
    Urho3D::Vector3 colonyPosition_;
    Urho3D::PODVector <District *> neighbors_;

    float farmingSquare_;
    float forestsSquare_;
    float landAverageFertility_;
    ClimateType climate_;

    float forestsReproductivity_;
    bool hasCoalDeposits_;
    bool hasIronDeposits_;
    bool hasSilverDeposits_;
    bool hasGoldDeposits_;

    float nativesCount_;
    float nativesFightingTechnologyLevel_;
    float nativesAggressiveness_;
    NativesCharacter nativesCharacter_;

    bool hasColony_;
    Urho3D::String colonyOwnerName_;
    float menCount_;
    float womenCount_;
    float localArmySize_;
    float farmsEvolutionPoints_;
    float minesEvolutionPoints_;
    float industryEvolutionPoints_;
    float logisticsEvolutionPoints_;
    float defenseEvolutionPoints_;
    float averageLevelOfLifePoints_;
public:
    District (Urho3D::Context *context);
    virtual ~District ();

    void UpdateDataNode (Urho3D::Node *dataNode, bool rewritePolygonPoints);
    void ReadDataFromNode (Urho3D::Node *dataNode);
    void CalculateNeighbors (Urho3D::PODVector <District *> &allDistricts);

    void UpdateHash (Map *owner);
    Urho3D::StringHash GetHash ();
    void SetHash (Urho3D::StringHash hash);

    bool IsSea ();
    bool SetIsSea (bool isSea);

    bool IsImpassable ();
    void SetIsImpassable (bool isImpassable);

    Urho3D::String GetName ();
    void SetName (Urho3D::String name);

    Urho3D::PODVector<Urho3D::Vector3> GetPolygonPoints();
    void SetPolygonPoints (Urho3D::PODVector <Urho3D::Vector3> polygonPoints);
    Urho3D::VariantVector &GetPolygonPointsAttribute ();
    void SetPolygonPointsAttribute (Urho3D::VariantVector polygonPoints);

    Urho3D::Vector3 GetUnitPosition ();
    void SetUnitPosition (Urho3D::Vector3 unitPosition);

    Urho3D::Vector3 GetColonyPosition ();
    void SetColonyPosition (Urho3D::Vector3 colonyPosition);

    Urho3D::PODVector <Urho3D::StringHash> GetNeighborsHashes ();
    void SetNeighborsHashes (Urho3D::PODVector <Urho3D::StringHash> neighbors);
    Urho3D::VariantVector GetNeighborsHashesAttribute ();
    void SetNeighborsHashesAttribute (Urho3D::VariantVector neighbors);

    float GetFarmingSquare ();
    void SetFarmingSquare (float farmingSquare);

    float GetForestsSquare ();
    void SetForestsSquare (float forestsSquare);

    float GetLandAverageFertility ();
    void SetLandAverageFertility (float landAverageFertility);

    ClimateType GetClimate ();
    void SetClimate (ClimateType climate);

    float GetForestsReproductivity ();
    void SetForestsReproductivity (float forestsReproductivity);

    bool HasCoalDeposits ();
    void SetCoalDepositits (bool hasCoalDeposits);

    bool HasIronDeposits ();
    void SetIronDeposits (bool hasIronDeposits);

    bool HasSilverDeposits ();
    void SetSilverDeposits (bool hasSilverDeposits);

    bool HasGoldDeposits ();
    void SetGoldDepositions (bool hasGoldDeposits);

    float GetNativesCount ();
    void SetNativesCount (float nativesCount);

    float GetNativesFightingTechnologyLevel ();
    void SetNativesFightingTechnologyLevel (float nativesFightingTechnologyLevel);

    float GetNativesAggressiveness ();
    void SetNativesAggressivness (float nativesAggressiveness);

    NativesCharacter GetNativesCharacter ();
    void SetNativesCharacter (NativesCharacter nativesCharacter);

    bool HasColony ();
    void SetColony (bool hasColony);

    Urho3D::String GetColonyOwnerName ();
    void SetColonyOwnerName (Urho3D::String colonyOwnerName);

    float GetMenCount ();
    void SetMenCount (float menCount);

    float GetWomenCount ();
    void SetWomenCount (float womenCount);

    float GetLocalArmySize ();
    void SetLocalArmySize (float localArmySize);

    float GetFarmsEvolutionPoints ();
    void SetFarmsEvolutionPoints (float farmsEvolutionPoints);

    float GetMinesEvolutionPoints ();
    void SetMinesEvolutionPoints (float minesEvolutionPoints);

    float GetIndustryEvolutionPoints ();
    void SetIndustryEvolutionPoints (float industryEvolutionPoints);

    float GetLogisticsEvolutionPoints ();
    void SetLogisticsEvolutionPoints (float logisticsEvolutionPoints);

    float GetDefenseEvolutionPoints ();
    void SetDefenseEvolutionPoints (float defenseEvolutionPoints);

    float GetAverageLevelOfLifePoints ();
    void SetAverageLevelOfLifePoint (float averageLevelOfLifePoints);
};
}
