#pragma once
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Scene/LogicComponent.h>

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

enum NativesCharacterType
{
    NATIVES_CHARACTER_FRIENDLY = 0,
    NATIVES_CHARACTER_MEDIUM = 1,
    NATIVES_CHARACTER_COLD = 2,
    NATIVES_CHARACTER_ISOLATIONIST = 3,
    NATIVES_CHARATER_AGGRESSIVE = 4
};

class District : public Urho3D::Component
{
URHO3D_OBJECT (District, Component)
protected:
    Urho3D::StringHash hash_;
    bool isSea_;
    bool isImpassable_;
    Urho3D::String name_;
    Urho3D::PODVector <Urho3D::Vector3> polygonPoints_;
    Urho3D::Vector3 unitPosition_;
    Urho3D::Vector3 colonyPosition_;
    Urho3D::PODVector <Urho3D::StringHash> neighbors_;

    float farmingSquare_;
    float forestsSquare_;
    float landAverageFertility_;
    ClimateType climate_;

    bool hasCoalDeposits_;
    bool hasIronDeposits_;
    bool hasSilverDeposits_;
    bool hasGoldDeposits_;

    float nativesCount_;
    float nativesFightingTechnologyLevel_;
    float nativesAggressiveness_;
    NativesCharacterType nativesCharacter_;

    bool hasColony_;
    Urho3D::String colonyOwnerName_;
    float menCount_;
    float womenCount_;
    float farmsEvolutionPoints_;
    float minesEvolutionPoints_;
    float industryEvolutionPoints_;
    float logisticsEvolutionPoints_;
    float defenseEvolutionPoints_;

    float farmsBalance_;
    float minesBalance_;
    float industryBalance_;
    float logisticsBalance_;
    float defenseBalance_;
    float averageLevelOfLifePoints_;
public:
    District (Urho3D::Context *context);
    virtual ~District ();

    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);
    static void RegisterObject (Urho3D::Context *context);
    void CalculateNeighbors (Urho3D::Vector <Urho3D::SharedPtr <District> > &allDistricts);

    void UpdateHash (Map *owner);
    Urho3D::StringHash GetHash () const;
    void SetHash (const Urho3D::StringHash &hash);

    bool IsSea () const;
    void SetIsSea (bool isSea);

    bool IsImpassable () const;
    void SetIsImpassable (bool isImpassable);

    Urho3D::String GetName () const;
    void SetName (const Urho3D::String &name);

    Urho3D::PODVector <Urho3D::Vector3> GetPolygonPoints () const;
    void SetPolygonPoints (const Urho3D::PODVector <Urho3D::Vector3> &polygonPoints);
    Urho3D::VariantVector GetPolygonPointsAttribute () const;
    void SetPolygonPointsAttribute (const Urho3D::VariantVector &polygonPoints);

    Urho3D::Vector3 GetUnitPosition () const;
    void SetUnitPosition (const Urho3D::Vector3 &unitPosition);

    Urho3D::Vector3 GetColonyPosition () const;
    void SetColonyPosition (const Urho3D::Vector3 &colonyPosition);

    bool IsNeighborsWith (Urho3D::StringHash otherDistrict) const;
    Urho3D::PODVector <Urho3D::StringHash> GetNeighborsHashes () const;
    void SetNeighborsHashes (const Urho3D::PODVector <Urho3D::StringHash> &neighbors);
    Urho3D::VariantVector GetNeighborsHashesAttribute () const;
    void SetNeighborsHashesAttribute (const Urho3D::VariantVector &neighbors);

    float GetFarmingSquare () const;
    void SetFarmingSquare (float farmingSquare);

    float GetForestsSquare () const;
    void SetForestsSquare (float forestsSquare);

    float GetLandAverageFertility () const;
    void SetLandAverageFertility (float landAverageFertility);

    ClimateType GetClimate () const;
    void SetClimate (ClimateType climate);

    bool HasCoalDeposits () const;
    void SetCoalDeposits (bool hasCoalDeposits);

    bool HasIronDeposits () const;
    void SetIronDeposits (bool hasIronDeposits);

    bool HasSilverDeposits () const;
    void SetSilverDeposits (bool hasSilverDeposits);

    bool HasGoldDeposits () const;
    void SetGoldDeposits (bool hasGoldDeposits);

    float GetNativesCount () const;
    void SetNativesCount (float nativesCount);

    float GetNativesFightingTechnologyLevel () const;
    void SetNativesFightingTechnologyLevel (float nativesFightingTechnologyLevel);

    float GetNativesAggressiveness () const;
    void SetNativesAggressiveness (float nativesAggressiveness);

    NativesCharacterType GetNativesCharacter () const;
    void SetNativesCharacter (NativesCharacterType nativesCharacter);

    bool HasColony () const;
    void SetColony (bool hasColony);

    Urho3D::String GetColonyOwnerName () const;
    void SetColonyOwnerName (const Urho3D::String &colonyOwnerName);

    float GetMenCount () const;
    void SetMenCount (float menCount);

    float GetWomenCount () const;
    void SetWomenCount (float womenCount);

    float GetFarmsEvolutionPoints () const;
    void SetFarmsEvolutionPoints (float farmsEvolutionPoints);

    float GetMinesEvolutionPoints () const;
    void SetMinesEvolutionPoints (float minesEvolutionPoints);

    float GetIndustryEvolutionPoints () const;
    void SetIndustryEvolutionPoints (float industryEvolutionPoints);

    float GetLogisticsEvolutionPoints () const;
    void SetLogisticsEvolutionPoints (float logisticsEvolutionPoints);

    float GetDefenseEvolutionPoints () const;
    void SetDefenseEvolutionPoints (float defenseEvolutionPoints);

    float GetFarmsBalance () const;
    void SetFarmsBalance (float farmsBalance);

    float GetMinesBalance () const;
    void SetMinesBalance (float minesBalance);

    float GetIndustryBalance () const;
    void SetIndustryBalance (float industryBalance);

    float GetLogisticsBalance () const;
    void SetLogisticsBalance (float logisticsBalance);

    float GetDefenseBalance () const;
    void SetDefenseBalance (float defenseBalance);

    float GetAverageLevelOfLifePoints () const;
    void SetAverageLevelOfLifePoints (float averageLevelOfLifePoints);
};
}
