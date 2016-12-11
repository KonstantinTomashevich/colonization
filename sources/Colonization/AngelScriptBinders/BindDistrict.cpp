#include <Colonization/BuildConfiguration.hpp>
#include "BindDistrict.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/District.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
OBJECT_PROPERTY_GETTER_AND_SETTER (District, bool, needDataUpdate_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, bool, isSea_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, bool, isImpassable_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, Urho3D::String, name_)
OBJECT_ARRAY_PROPERTY_ACESSOR (District, PODVector, Urho3D::Vector3, polygonPoints_, "Array<Vector3>")
OBJECT_PROPERTY_GETTER_AND_SETTER (District, Urho3D::Vector3, unitPosition_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, Urho3D::Vector3, colonyPosition_)
OBJECT_ARRAY_OF_POINTERS_PROPERTY_ACESSOR (District, PODVector, District*, neighbors_, "Array<District@>")

OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, farmingSquare_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, forestsSquare_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, landAverageFertility_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, ClimateType, climate_)

OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, forestsReproductivity_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, bool, hasCoalDeposits_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, bool, hasIronDeposits_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, bool, hasSilverDeposits_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, bool, hasGoldDeposits_)

OBJECT_PROPERTY_GETTER_AND_SETTER (District, int, nativesCount_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, int, nativesFightingTechnologyLevel_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, nativesAggressiveness_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, NativesCharacter, nativesCharacter_)

OBJECT_PROPERTY_GETTER_AND_SETTER (District, bool, hasColony_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, Urho3D::String, colonyOwnerName_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, int, mansCount_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, int, womenCount_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, int, localArmySize_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, farmsEvolutionPoints_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, minesEvolutionPoints_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, industryEvolutionPoints_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, logisticsEvolutionPoints_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, defenseEvolutionPoints_)
OBJECT_PROPERTY_GETTER_AND_SETTER (District, float, averageLevelOfLifePoints_)

District *CreateDistrict ()
{
    return new District (Urho3D::GetScriptContext ());
}

void BindDistrict (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <District> (engine, "District");

    engine->RegisterEnum ("ClimateType");
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_TROPICAL", CLIMATE_TROPICAL);
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_HOT", CLIMATE_HOT );
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_NORMAL", CLIMATE_NORMAL);
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_NORMAL_CONTINENTAL", CLIMATE_NORMAL_CONTINENTAL);
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_DESERT", CLIMATE_DESERT);
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_COLD", CLIMATE_COLD);

    engine->RegisterEnum ("NativesCharacter");
    engine->RegisterEnumValue ("NativesCharacter", "NATIVES_CHARACTER_FRIENDLY", NATIVES_CHARACTER_FRIENDLY);
    engine->RegisterEnumValue ("NativesCharacter", "NATIVES_CHARACTER_MEDIUM", NATIVES_CHARACTER_MEDIUM);
    engine->RegisterEnumValue ("NativesCharacter", "NATIVES_CHARACTER_COLD", NATIVES_CHARACTER_COLD);
    engine->RegisterEnumValue ("NativesCharacter", "NATIVES_CHARACTER_ISOLATIONIST", NATIVES_CHARACTER_ISOLATIONIST);
    engine->RegisterEnumValue ("NativesCharacter", "NATIVES_CHARATER_AGRESSIVE", NATIVES_CHARATER_AGRESSIVE);

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "District", "void UpdateDataNode (Node @dataNode, bool rewritePolygonPoints = false)",
                                  asMETHOD (District, UpdateDataNode), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "District", "void ReadDataFromNode (Node @dataNode)",
                                  asMETHOD (District, ReadDataFromNode), asCALL_THISCALL)
                );

    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", bool, needDataUpdate_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", bool, isSea_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", bool, isImpassable_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", String, name_);
    BIND_OBJECT_ARRAY_PROPERTY (engine, District, "District", Vector3, polygonPoints_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", Vector3, unitPosition_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", Vector3, colonyPosition_);
    BIND_OBJECT_ARRAY_PROPERTY (engine, District, "District", District@, neighbors_);

    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, farmingSquare_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, forestsSquare_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, landAverageFertility_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", ClimateType, climate_);

    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, forestsReproductivity_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", bool, hasCoalDeposits_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", bool, hasIronDeposits_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", bool, hasSilverDeposits_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", bool, hasGoldDeposits_);

    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", int, nativesCount_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", int, nativesFightingTechnologyLevel_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, nativesAggressiveness_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", NativesCharacter, nativesCharacter_);

    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", bool, hasColony_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", String, colonyOwnerName_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", int, mansCount_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", int, womenCount_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", int, localArmySize_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, farmsEvolutionPoints_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, minesEvolutionPoints_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, industryEvolutionPoints_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, logisticsEvolutionPoints_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, defenseEvolutionPoints_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, District, "District", float, averageLevelOfLifePoints_);

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterGlobalFunction (
                    "District @CreateDistrict ()", asFUNCTION (CreateDistrict), asCALL_CDECL)
                );
}
}
