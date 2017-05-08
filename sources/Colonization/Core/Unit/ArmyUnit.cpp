#include <Colonization/BuildConfiguration.hpp>
#include "ArmyUnit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Unit/UnitTags.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
ArmyUnit::ArmyUnit (Urho3D::Context *context) : Unit (context),
    soldiersCount_ (0)
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
    URHO3D_ACCESSOR_ATTRIBUTE ("Soldiers count", GetSoldiersCount, SetSoldiersCount, int, 0, Urho3D::AM_DEFAULT);
}

void ArmyUnit::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    assert (map);

    District *district = map->GetDistrictByHash (positionHash_);
    if (district)
    {
        Urho3D::Sphere sphere (district->GetUnitPosition (), 0.15f);
        debug->AddSphere (sphere, Urho3D::Color::RED, depthTest);
    }
}

int ArmyUnit::GetSoldiersCount () const
{
    return soldiersCount_;
}

void ArmyUnit::SetSoldiersCount (int soldiersCount)
{
    soldiersCount_ = soldiersCount;
}

float ArmyUnit::GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const
{
    return 0.0f;
}

bool ArmyUnit::ApplyDamage (GameConfiguration *configuration, float damage)
{
    return false;
}

Urho3D::String ArmyUnit::GetUnitTypeTag () const
{
    return TAG_ARMY;
}
}
