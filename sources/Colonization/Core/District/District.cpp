#include <Colonization/BuildConfiguration.hpp>
#include "District.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>

#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/District/ColonyActions.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
const char *polygonPointsStructureElementsNames [] =
{
    "Polygon Points Count",
    "   Point",
    0
};

const char *neighborsHashesStructureElementsNames [] =
{
    "Neighbors Count",
    "   District Hash",
    0
};

const char *colonyActionsStructureElementsNames [] =
{
    "Colony Actions Count",
    "   Type",
    "   Data",
    0
};

const char *climateTypesNames [] =
{
    "Tropical",
    "Hot",
    "Temperate",
    "Temperate Continental",
    "Desert",
    "Cold",
    0
};

const char *nativesCharacterTypesNames [] =
{
    "Friendly",
    "Medium",
    "Cold",
    "Isolationist",
    "Aggressive",
    0
};

District::District (Urho3D::Context *context) : Urho3D::Component (context),
    hash_ ("nothing"),
    isSea_ (true),
    isImpassable_ (false),
    name_ ("Unknown"),
    polygonPoints_ (),
    unitPosition_ (),
    colonyPosition_ (),
    neighbors_ (),

    farmingSquare_ (1.0f),
    forestsSquare_ (1.0f),
    landAverageFertility_ (1.0f),
    climate_ (CLIMATE_TEMPERATE),

    hasCoalDeposits_ (false),
    hasIronDeposits_ (false),
    hasSilverDeposits_ (false),
    hasGoldDeposits_ (false),

    nativesCount_ (0.0f),
    nativesFightingTechnologyLevel_ (1.0f),
    nativesAggressiveness_ (0.0f),
    nativesCharacter_ (NATIVES_CHARACTER_MEDIUM),

    hasColony_ (false),
    colonyOwnerName_ (),
    colonyActions_ (),
    menCount_ (0.0f),
    womenCount_ (0.0f),

    farmsEvolutionPoints_ (0.0f),
    minesEvolutionPoints_ (0.0f),
    industryEvolutionPoints_ (0.0f),
    logisticsEvolutionPoints_ (0.0f),
    defenseEvolutionPoints_ (0.0f),

    farmsBalance_ (0.0f),
    minesBalance_ (0.0f),
    industryBalance_ (0.0f),
    logisticsBalance_ (0.0f),
    defenseBalance_ (0.0f),
    averageLevelOfLifePoints_ (0.0f),

    lastTradeFarmsProduction_ (),
    lastTradeMinesProduction_ (),
    lastTradeIndustryProduction_ (),

    lastTradeFarmsBalanceAddition_ (0.0f),
    lastTradeMinesBalanceAddition_ (0.0f),
    lastTradeIndustryBalanceAddition_ (0.0f),
    lastTradeLogisticsBalanceAddition_ (0.0f),
    lastTradeDefenseBalanceAddition_ (0.0f)
{

}

District::~District ()
{

}

void District::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    if (!polygonPoints_.Empty ())
    {
        for (int index = 0; index < polygonPoints_.Size (); index++)
        {
            Urho3D::Vector3 firstPoint = polygonPoints_.At (index);
            Urho3D::Vector3 secondPoint;

            if (index + 1 < polygonPoints_.Size ())
            {
                secondPoint = polygonPoints_.At (index + 1);
            }
            else
            {
                secondPoint = polygonPoints_.At (0);
            }

            debug->AddLine (firstPoint, secondPoint, Urho3D::Color::RED, depthTest);
        }
    }

    Urho3D::Sphere colonySphere (colonyPosition_, 0.25f);
    debug->AddSphere (colonySphere, Urho3D::Color::YELLOW, depthTest);

    Urho3D::Sphere unitSphere (unitPosition_, 0.15f);
    debug->AddSphere (unitSphere, Urho3D::Color::GREEN, depthTest);
}

void District::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <District> (COLONIZATION_CORE_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Hash", GetHash, SetHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Sea", IsSea, SetIsSea, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Impassable", IsImpassable, SetIsImpassable, bool, false, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Name", GetName, SetName, Urho3D::String, Urho3D::String::EMPTY, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Polygon Points", GetPolygonPointsAttribute, SetPolygonPointsAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, polygonPointsStructureElementsNames, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Unit Position", GetUnitPosition, SetUnitPosition, Urho3D::Vector3, Urho3D::Vector3::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Colony Position", GetColonyPosition, SetColonyPosition, Urho3D::Vector3, Urho3D::Vector3::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Neighbors Hashes", GetNeighborsHashesAttribute, SetNeighborsHashesAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, neighborsHashesStructureElementsNames, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Farming Square", GetFarmingSquare, SetFarmingSquare, float, 1.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Forests Square", GetForestsSquare, SetForestsSquare, float, 1.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Land Average Fertility", GetLandAverageFertility, SetLandAverageFertility, float, 1.0f, Urho3D::AM_DEFAULT);
    URHO3D_ENUM_ACCESSOR_ATTRIBUTE ("Climate", GetClimate, SetClimate, ClimateType, climateTypesNames, CLIMATE_TEMPERATE, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Has Coal Deposits", HasCoalDeposits, SetCoalDeposits, bool, false, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Has Iron Deposits", HasIronDeposits, SetIronDeposits, bool, false, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Has Silver Deposits", HasSilverDeposits, SetSilverDeposits, bool, false, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Has Gold Deposits", HasGoldDeposits, SetGoldDeposits, bool, false, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Natives Count", GetNativesCount, SetNativesCount, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Natives Fighting Technology Level", GetNativesFightingTechnologyLevel, SetNativesFightingTechnologyLevel, float, 1.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Natives Aggressiveness", GetNativesAggressiveness, SetNativesAggressiveness, float, 1.0f, Urho3D::AM_DEFAULT);
    URHO3D_ENUM_ACCESSOR_ATTRIBUTE ("Natives Character", GetNativesCharacter, SetNativesCharacter, NativesCharacterType, nativesCharacterTypesNames, NATIVES_CHARACTER_MEDIUM, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Has Colony", HasColony, SetColony, bool, false, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Colony Owner Name", GetColonyOwnerName, SetColonyOwnerName, Urho3D::String, Urho3D::String::EMPTY, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Colony Actions", GetColonyActionsAttribute, SetColonyActionsAttribute,
                                                              Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector,
                                                              colonyActionsStructureElementsNames, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Men Count", GetMenCount, SetMenCount, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Women Count", GetWomenCount, SetWomenCount, float, 0.0f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Evolution Points", GetFarmsEvolutionPoints, SetFarmsEvolutionPoints, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Evolution Points", GetMinesEvolutionPoints, SetMinesEvolutionPoints, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Evolution Points", GetIndustryEvolutionPoints, SetIndustryEvolutionPoints, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Logistics Evolution Points", GetLogisticsEvolutionPoints, SetLogisticsEvolutionPoints, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Defense Evolution Points", GetDefenseEvolutionPoints, SetDefenseEvolutionPoints, float, 0.0f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Farms Balance", GetFarmsBalance, SetFarmsBalance, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Mines Balance", GetMinesBalance, SetMinesBalance, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Industry Balance", GetIndustryBalance, SetIndustryBalance, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Logistics Balance", GetLogisticsBalance, SetLogisticsBalance, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Defense Balance", GetDefenseBalance, SetDefenseBalance, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Average Level Of Life Points", GetAverageLevelOfLifePoints, SetAverageLevelOfLifePoints, float, 0.0f, Urho3D::AM_DEFAULT);

    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Last Trade Farms Production", GetLastTradeFarmsProduction, SetLastTradeFarmsProduction,
                                     Urho3D::VariantMap, Urho3D::Variant::emptyVariantMap, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Last Trade Mines Production", GetLastTradeMinesProduction, SetLastTradeMinesProduction,
                                     Urho3D::VariantMap, Urho3D::Variant::emptyVariantMap, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Last Trade Industry Production", GetLastTradeIndustryProduction, SetLastTradeIndustryProduction,
                                     Urho3D::VariantMap, Urho3D::Variant::emptyVariantMap, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Last Trade Farms Balance Addition", GetLastTradeFarmsBalanceAddition,
                               SetLastTradeFarmsBalanceAddition, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Last Trade Mines Balance Addition", GetLastTradeMinesBalanceAddition,
                               SetLastTradeMinesBalanceAddition, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Last Trade Industry Balance Addition", GetLastTradeIndustryBalanceAddition,
                               SetLastTradeIndustryBalanceAddition, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Last Trade Logistics Balance Addition", GetLastTradeLogisticsBalanceAddition,
                               SetLastTradeLogisticsBalanceAddition, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Last Trade Defense Balance Addition", GetLastTradeDefenseBalanceAddition,
                               SetLastTradeDefenseBalanceAddition, float, 0.0f, Urho3D::AM_DEFAULT);
}

void District::CalculateNeighbors (Urho3D::Vector<Urho3D::SharedPtr<District> > &allDistricts)
{
    assert (!allDistricts.Empty ());
    neighbors_.Clear ();
    for (int index = 0; index < allDistricts.Size (); index++)
    {
        Urho3D::SharedPtr <District> another = allDistricts.At (index);
        Urho3D::PODVector <Urho3D::Vector3> anotherPolygonPoints = another->GetPolygonPoints ();
        assert (another);

        if (another != this && !anotherPolygonPoints.Empty () && !polygonPoints_.Empty ())
        {
            int contactsCount = 0;
            for (int anotherPolygonPointIndex = 0;
                 anotherPolygonPointIndex < anotherPolygonPoints.Size (); anotherPolygonPointIndex++)
            {

                for (int thisPolygonPointIndex = 0;
                     thisPolygonPointIndex < polygonPoints_.Size (); thisPolygonPointIndex++)
                {

                    if (anotherPolygonPoints.At (anotherPolygonPointIndex) ==
                            polygonPoints_.At (thisPolygonPointIndex))
                    {
                        contactsCount += 1;
                    }
                }
            }

            if (contactsCount >= 2)
            {
                neighbors_.Push (another->GetHash ());
            }
        }
    }
}

void District::Invest (Urho3D::StringHash investitionType, float money)
{
    if (investitionType == Urho3D::StringHash ("farms"))
    {
        farmsBalance_ += money;
    }
    else if (investitionType == Urho3D::StringHash ("mines"))
    {
        minesBalance_ += money;
    }
    else if (investitionType == Urho3D::StringHash ("industry"))
    {
        industryBalance_ += money;
    }
    else if (investitionType == Urho3D::StringHash ("logistics"))
    {
        logisticsBalance_ += money;
    }
    else if (investitionType == Urho3D::StringHash ("defense"))
    {
        defenseBalance_ += money;
    }
}

void District::AddColonyAction (Urho3D::StringHash actionType, Urho3D::VariantMap &actionData)
{    
    Urho3D::StringHash id;
    bool found = false;
    do
    {
        id = Urho3D::StringHash (Urho3D::Random (0, 100000));
        GetColonyActionById (id, found);
    }
    while (found);
    actionData [COLONY_ACTION_ID] = Urho3D::Variant (id);
    actionData [COLONY_ACTION_PROGRESS] = Urho3D::Variant (0.0f);

    Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> action;
    action.first_ = actionType;
    action.second_ = actionData;
    colonyActions_.Push (action);
}

int District::GetColonyActionsCount () const
{
    return colonyActions_.Size ();
}

Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> District::GetColonyActionByIndex (int index) const
{
    assert (index < colonyActions_.Size ());
    return colonyActions_.At (index);
}

Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> District::GetColonyActionById (Urho3D::StringHash id, bool &found) const
{
    for (int index = 0; index < colonyActions_.Size (); index++)
    {
        Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> action = colonyActions_.At (index);
        if (action.second_ [COLONY_ACTION_ID].GetStringHash () == id)
        {
            found = true;
            return action;
        }
    }

    found = false;
    Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> empty;
    return empty;
}

bool District::SetColonyActionAtIndexData (int index, Urho3D::VariantMap &actionData)
{
    assert (index < colonyActions_.Size ());
    // Prevent actions from overriding.
    if (colonyActions_.At (index).second_ [COLONY_ACTION_ID].GetStringHash () ==
            actionData [COLONY_ACTION_ID].GetStringHash ())
    {
        colonyActions_.At (index).second_ = actionData;
        return true;
    }
    else
    {
        return false;
    }
}

bool District::SetColonyActionWithIdData (Urho3D::VariantMap &actionData)
{
    for (Urho3D::Vector <Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> >::Iterator iterator = colonyActions_.Begin ();
         iterator != colonyActions_.End (); iterator++)
    {
        if (iterator->second_ [COLONY_ACTION_ID].GetStringHash () == actionData [COLONY_ACTION_ID].GetStringHash ())
        {
            iterator->second_ = actionData;
            return true;
        }
    }
    return false;
}

void District::RemoveColonyActionByIndex (int index)
{
    assert (index < colonyActions_.Size ());
    colonyActions_.Erase (index);
}

bool District::RemoveColonyActionById (Urho3D::StringHash id)
{
    for (Urho3D::Vector <Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> >::Iterator iterator = colonyActions_.Begin ();
         iterator != colonyActions_.End (); iterator++)
    {
        if (iterator->second_ [COLONY_ACTION_ID].GetStringHash () == id)
        {
            iterator = colonyActions_.Erase (iterator);
            return true;
        }
    }
    return false;
}

void District::UpdateHash (Map *owner)
{
    do
    {
        hash_ = Urho3D::StringHash (name_ + Urho3D::String (Urho3D::Random (0, 100000)));
    }
    while (owner->GetDistrictByHash (hash_) != this && hash_ != Urho3D::StringHash::ZERO);
}

Urho3D::StringHash District::GetHash () const
{
    return hash_;
}

void District::SetHash (const Urho3D::StringHash &hash)
{
    hash_ = hash;
}

bool District::IsSea () const
{
    return isSea_;
}

void District::SetIsSea (bool isSea)
{
    isSea_ = isSea;
}

bool District::IsImpassable () const
{
    return isImpassable_;
}

void District::SetIsImpassable (bool isImpassable)
{
    isImpassable_ = isImpassable;
}

Urho3D::String District::GetName () const
{
    return name_;
}

void District::SetName (const Urho3D::String &name)
{
    assert (!name.Empty ());
    name_ = name;
}

Urho3D::PODVector <Urho3D::Vector3> District::GetPolygonPoints () const
{
    return polygonPoints_;
}

void District::SetPolygonPoints (const Urho3D::PODVector <Urho3D::Vector3> &polygonPoints)
{
    polygonPoints_ = polygonPoints;
}

Urho3D::VariantVector District::GetPolygonPointsAttribute () const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (polygonPoints_.Size ());
    for (int index = 0; index < polygonPoints_.Size (); index++)
    {
        variantVector.Push (Urho3D::Variant (polygonPoints_.At (index)));
    }
    return variantVector;
}

void District::SetPolygonPointsAttribute (const Urho3D::VariantVector &polygonPoints)
{
    polygonPoints_.Clear ();
    if (!polygonPoints.Empty ())
    {
        int requestedSize = polygonPoints.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < polygonPoints.Size ())
                {
                    polygonPoints_.Push (polygonPoints.At (index + 1).GetVector3 ());
                }
                else
                {
                    polygonPoints_.Push (Urho3D::Vector3 ());
                }
            }
        }
    }
}

Urho3D::Vector3 District::GetUnitPosition () const
{
    return unitPosition_;
}

void District::SetUnitPosition (const Urho3D::Vector3 &unitPosition)
{
    unitPosition_ = unitPosition;
}

Urho3D::Vector3 District::GetColonyPosition () const
{
    return colonyPosition_;
}

void District::SetColonyPosition (const Urho3D::Vector3 &colonyPosition)
{
    colonyPosition_ = colonyPosition;
}

bool District::IsNeighborsWith (Urho3D::StringHash otherDistrict) const
{
    return neighbors_.Contains (otherDistrict);
}

Urho3D::PODVector <Urho3D::StringHash> District::GetNeighborsHashes () const
{
    return neighbors_;
}

void District::SetNeighborsHashes (const Urho3D::PODVector <Urho3D::StringHash> &neighbors)
{
    neighbors_ = neighbors;
}

Urho3D::VariantVector District::GetNeighborsHashesAttribute() const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (Urho3D::Variant (neighbors_.Size ()));
    for (int index = 0; index < neighbors_.Size (); index++)
    {
        variantVector.Push (Urho3D::Variant (neighbors_.At (index)));
    }
    return variantVector;
}

void District::SetNeighborsHashesAttribute (const Urho3D::VariantVector &neighbors)
{
    neighbors_.Clear ();
    if (!neighbors.Empty ())
    {
        int requestedSize = neighbors.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < neighbors.Size ())
                {
                    neighbors_.Push (neighbors.At (index + 1).GetStringHash ());
                }
                else
                {
                    neighbors_.Push (Urho3D::StringHash ());
                }
            }
        }
    }
}

float District::GetFarmingSquare () const
{
    return farmingSquare_;
}

void District::SetFarmingSquare (float farmingSquare)
{
    assert (farmingSquare > 0.0f);
    farmingSquare_ = farmingSquare;
}

float District::GetForestsSquare () const
{
    return forestsSquare_;
}

void District::SetForestsSquare (float forestsSquare)
{
    assert (forestsSquare > 0.0f);
    forestsSquare_ = forestsSquare;
}

float District::GetLandAverageFertility () const
{
    return landAverageFertility_;
}

void District::SetLandAverageFertility (float landAverageFertility)
{
    assert (landAverageFertility > 0.0f);
    landAverageFertility_ = landAverageFertility;
}

ClimateType District::GetClimate () const
{
    return climate_;
}

void District::SetClimate (ClimateType climate)
{
    climate_ = climate;
}

bool District::HasCoalDeposits () const
{
    return hasCoalDeposits_;
}

void District::SetCoalDeposits (bool hasCoalDeposits)
{
    hasCoalDeposits_ = hasCoalDeposits;
}

bool District::HasIronDeposits () const
{
    return hasIronDeposits_;
}

void District::SetIronDeposits (bool hasIronDeposits)
{
    hasIronDeposits_ = hasIronDeposits;
}

bool District::HasSilverDeposits () const
{
    return hasSilverDeposits_;
}

void District::SetSilverDeposits (bool hasSilverDeposits)
{
    hasSilverDeposits_ = hasSilverDeposits;
}

bool District::HasGoldDeposits () const
{
    return hasGoldDeposits_;
}

void District::SetGoldDeposits (bool hasGoldDeposits)
{
    hasGoldDeposits_ = hasGoldDeposits;
}

float District::GetNativesCount () const
{
    return nativesCount_;
}

void District::SetNativesCount (float nativesCount)
{
    assert (nativesCount >= 0.0f);
    nativesCount_ = nativesCount;
}

float District::GetNativesFightingTechnologyLevel () const
{
    return nativesFightingTechnologyLevel_;
}

void District::SetNativesFightingTechnologyLevel (float nativesFightingTechnologyLevel)
{
    assert (nativesFightingTechnologyLevel > 0.0f);
    nativesFightingTechnologyLevel_ = nativesFightingTechnologyLevel;
}

float District::GetNativesAggressiveness () const
{
    return nativesAggressiveness_;
}

void District::SetNativesAggressiveness (float nativesAggressiveness)
{
    assert (nativesAggressiveness >= 0.0f);
    nativesAggressiveness_ = nativesAggressiveness;
}

NativesCharacterType District::GetNativesCharacter () const
{
    return nativesCharacter_;
}

void District::SetNativesCharacter (NativesCharacterType nativesCharacter)
{
    nativesCharacter_ = nativesCharacter;
}

bool District::HasColony () const
{
    return hasColony_;
}

void District::SetColony (bool hasColony)
{
    hasColony_ = hasColony;
}

Urho3D::String District::GetColonyOwnerName () const
{
    return colonyOwnerName_;
}

void District::SetColonyOwnerName (const Urho3D::String &colonyOwnerName)
{
    colonyOwnerName_ = colonyOwnerName;
}

Urho3D::VariantVector District::GetColonyActionsAttribute () const
{
    Urho3D::VariantVector variantVector;
    for (int index = 0; index < colonyActions_.Size (); index++)
    {
        variantVector.Push (colonyActions_.At (index).first_);
        variantVector.Push (colonyActions_.At (index).second_);
    }
    return variantVector;
}

void District::SetColonyActionsAttribute (const Urho3D::VariantVector &colonyActions)
{
    colonyActions_.Clear ();
    for (int index = 0; index + 1 < colonyActions.Size (); index++)
    {
        Urho3D::Pair <Urho3D::StringHash, Urho3D::VariantMap> action;
        action.first_ = colonyActions.At (index).GetStringHash ();
        action.second_ = colonyActions.At (index + 1).GetVariantMap ();
        colonyActions_.Push (action);
    }
}

float District::GetMenCount () const
{
    return menCount_;
}

void District::SetMenCount (float menCount)
{
    assert (menCount >= 0.0f);
    menCount_ = menCount;
}

float District::GetWomenCount () const
{
    return womenCount_;
}

void District::SetWomenCount (float womenCount)
{
    assert (womenCount >= 0.0f);
    womenCount_ = womenCount;
}

float District::GetFarmsEvolutionPoints () const
{
    return farmsEvolutionPoints_;
}

void District::SetFarmsEvolutionPoints (float farmsEvolutionPoints)
{
    assert (farmsEvolutionPoints >= 0.0f);
    farmsEvolutionPoints_ = farmsEvolutionPoints;
}

float District::GetMinesEvolutionPoints () const
{
    return minesEvolutionPoints_;
}

void District::SetMinesEvolutionPoints (float minesEvolutionPoints)
{
    assert (minesEvolutionPoints >= 0.0f);
    minesEvolutionPoints_ = minesEvolutionPoints;
}

float District::GetIndustryEvolutionPoints () const
{
    return industryEvolutionPoints_;
}

void District::SetIndustryEvolutionPoints (float industryEvolutionPoints)
{
    assert (industryEvolutionPoints >= 0.0f);
    industryEvolutionPoints_ = industryEvolutionPoints;
}

float District::GetLogisticsEvolutionPoints () const
{
    return logisticsEvolutionPoints_;
}

void District::SetLogisticsEvolutionPoints (float logisticsEvolutionPoints)
{
    assert (logisticsEvolutionPoints >= 0.0f);
    logisticsEvolutionPoints_ = logisticsEvolutionPoints;
}

float District::GetDefenseEvolutionPoints () const
{
    return defenseEvolutionPoints_;
}

void District::SetDefenseEvolutionPoints (float defenseEvolutionPoints)
{
    assert (defenseEvolutionPoints >= 0.0f);
    defenseEvolutionPoints_ = defenseEvolutionPoints;
}

float District::GetFarmsBalance () const
{
    return farmsBalance_;
}

void District::SetFarmsBalance (float farmsBalance)
{
    farmsBalance_ = farmsBalance;
}

float District::GetMinesBalance () const
{
    return minesBalance_;
}

void District::SetMinesBalance (float minesBalance)
{
    minesBalance_ = minesBalance;
}

float District::GetIndustryBalance () const
{
    return industryBalance_;
}

void District::SetIndustryBalance (float industryBalance)
{
    industryBalance_ = industryBalance;
}

float District::GetLogisticsBalance () const
{
    return logisticsBalance_;
}

void District::SetLogisticsBalance (float logisticsBalance)
{
    logisticsBalance_ = logisticsBalance;
}

float District::GetDefenseBalance () const
{
    return defenseBalance_;
}

void District::SetDefenseBalance (float defenseBalance)
{
    defenseBalance_ = defenseBalance;
}

float District::GetAverageLevelOfLifePoints () const
{
    return averageLevelOfLifePoints_;
}

void District::SetAverageLevelOfLifePoints (float averageLevelOfLifePoints)
{
    assert (averageLevelOfLifePoints >= 0.0f);
    averageLevelOfLifePoints_ = averageLevelOfLifePoints;
}

Urho3D::VariantMap District::GetLastTradeFarmsProduction () const
{
    return lastTradeFarmsProduction_;
}

void District::SetLastTradeFarmsProduction (const Urho3D::VariantMap &lastTradeFarmsProduction)
{
    lastTradeFarmsProduction_ = lastTradeFarmsProduction;
}

Urho3D::VariantMap District::GetLastTradeMinesProduction () const
{
    return lastTradeMinesProduction_;
}

void District::SetLastTradeMinesProduction (const Urho3D::VariantMap &lastTradeMinesProduction)
{
    lastTradeMinesProduction_ = lastTradeMinesProduction;
}

Urho3D::VariantMap District::GetLastTradeIndustryProduction () const
{
    return lastTradeIndustryProduction_;
}

void District::SetLastTradeIndustryProduction (const Urho3D::VariantMap &lastTradeIndustryProduction)
{
    lastTradeIndustryProduction_ = lastTradeIndustryProduction;
}

float District::GetLastTradeFarmsBalanceAddition () const
{
    return lastTradeFarmsBalanceAddition_;
}

void District::SetLastTradeFarmsBalanceAddition (float lastTradeFarmsBalanceAddition)
{
    lastTradeFarmsBalanceAddition_ = lastTradeFarmsBalanceAddition;
}

float District::GetLastTradeMinesBalanceAddition () const
{
    return lastTradeMinesBalanceAddition_;
}

void District::SetLastTradeMinesBalanceAddition (float lastTradeMinesBalanceAddition)
{
    lastTradeMinesBalanceAddition_ = lastTradeMinesBalanceAddition;
}

float District::GetLastTradeIndustryBalanceAddition () const
{
    return lastTradeIndustryBalanceAddition_;
}

void District::SetLastTradeIndustryBalanceAddition (float lastTradeIndustryBalanceAddition)
{
    lastTradeIndustryBalanceAddition_ = lastTradeIndustryBalanceAddition;
}

float District::GetLastTradeLogisticsBalanceAddition () const
{
    return lastTradeLogisticsBalanceAddition_;
}

void District::SetLastTradeLogisticsBalanceAddition (float lastTradeLogisticsBalanceAddition)
{
    lastTradeLogisticsBalanceAddition_ = lastTradeLogisticsBalanceAddition;
}

float District::GetLastTradeDefenseBalanceAddition () const
{
    return lastTradeDefenseBalanceAddition_;
}

void District::SetLastTradeDefenseBalanceAddition (float lastTradeDefenseBalanceAddition)
{
    lastTradeDefenseBalanceAddition_ = lastTradeDefenseBalanceAddition;
}
}
