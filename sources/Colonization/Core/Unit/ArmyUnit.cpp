#include <Colonization/BuildConfiguration.hpp>
#include "ArmyUnit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Unit/UnitTags.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
ArmyUnit::ArmyUnit (Urho3D::Context *context) : Unit (context),
    soldiersCount_ (0.0f)
{
    unitType_ = UNIT_ARMY;
}

ArmyUnit::~ArmyUnit ()
{

}

void ArmyUnit::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <ArmyUnit> (COLONIZATION_CORE_CATEGORY);
    URHO3D_COPY_BASE_ATTRIBUTES (Unit);
    URHO3D_ACCESSOR_ATTRIBUTE ("Soldiers count", GetSoldiersCount, SetSoldiersCount, float, 0.0f, Urho3D::AM_DEFAULT);
}

void ArmyUnit::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    assert (map);

    District *district = map->GetDistrictByHash (GetPositionHash ());
    if (district)
    {
        Urho3D::Sphere sphere (district->GetUnitPosition (), 0.15f);
        debug->AddSphere (sphere, Urho3D::Color::RED, depthTest);
    }
}

float ArmyUnit::GetSoldiersCount () const
{
    return soldiersCount_;
}

void ArmyUnit::SetSoldiersCount (float soldiersCount)
{
    soldiersCount_ = soldiersCount;
}

bool ArmyUnit::IsCanGoTo (const District *district, const Map *map, Urho3D::StringHash imaginePosition) const
{
    if (Unit::IsCanGoTo (district, map, imaginePosition))
    {
        if (district->GetIsSea () || (district->GetHasColony () && district->GetColonyOwnerName () == GetOwnerPlayerName ()))
        {
            return true;
        }
        else if (district->GetHasColony ())
        {
            PlayerInfo *myPlayerInfo = GetPlayerInfoByNameHash (node_->GetScene (), Urho3D::StringHash (GetOwnerPlayerName ()));
            return myPlayerInfo->IsAtWarWith (district->GetColonyOwnerName ());
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

float ArmyUnit::GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const
{
   if (isNaval)
   {
       return configuration->GetSoldierBasicNavalAttackForce () * soldiersCount_;
   }
   else
   {
       return configuration->GetSoldierBasicLandAttackForce () * soldiersCount_;
   }
}

bool ArmyUnit::ApplyDamage (GameConfiguration *configuration, float damage)
{
    float losses = damage / configuration->GetSoldierBasicHealth ();
    soldiersCount_ -= losses;
    return IsAlive ();
}

bool ArmyUnit::IsAlive () const
{
    return soldiersCount_ > 0.0f;
}

Urho3D::String ArmyUnit::GetUnitTypeTag () const
{
    return TAG_ARMY;
}
}
