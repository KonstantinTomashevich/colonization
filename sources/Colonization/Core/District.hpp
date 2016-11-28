#pragma once
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Container/Str.h>

namespace Colonization
{
enum ClimateType
{
    CLIMATE_TROPICAL = 0,
    CLIMATE_HOT = 1,
    CLIMATE_NORMAL = 2,
    CLIMATE_NORMAL_CONTINENTAL = 3,
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
public:
    District (Urho3D::Context *context);
    virtual ~District ();

    void UpdateDataNode (Urho3D::Node *dataNode, bool rewritePolygonPoints = false);
    void ReadDataFromNode (Urho3D::Node *dataNode);
    void CalculateNeighbors (Urho3D::Vector <District *> &allDistricts);

    bool needDataUpdate_;
    Urho3D::PODVector <Urho3D::Vector3> polygonPoints_;
    Urho3D::Vector <District *> neighbors_;

    float farmingSquare_;
    float forestsSquare_;
    float landAverageFertility_;
    ClimateType climate_;

    float forestsReproductivity_;
    bool hasCoalDeposits_;
    bool hasIronDeposits_;
    bool hasSilverDeposits_;
    bool hasGoldDeposits_;

    int nativesCount_;
    int nativesFightingTechnologyLevel_;
    float nativesAggressiveness_;
    NativesCharacter nativesCharacter_;

    bool hasColony_;
    Urho3D::String colonyOwnerName_;
    int mansCount_;
    int womenCount_;
    int localArmyCount_;
    float farmsEvolutionPoints_;
    float minesEvolutionPoints_;
    float industryEvolutionPoints_;
    float logisticsEvolutionPoints_;
    float defenseEvolutionPoints_;
    float averageStandartOfLifePoints_;
};
}
