#include <Colonization/BuildConfiguration.hpp>
#include "ColonizatorsUnit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Unit/UnitTags.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
ColonizatorsUnit::ColonizatorsUnit (Urho3D::Context *context) : Unit (context),
    colonizatorsCount_ (0)
{
    unitType_ = UNIT_COLONIZATORS;
}

ColonizatorsUnit::~ColonizatorsUnit ()
{

}

void ColonizatorsUnit::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <ColonizatorsUnit> (COLONIZATION_CORE_CATEGORY);
    URHO3D_COPY_BASE_ATTRIBUTES (Unit);
    URHO3D_ACCESSOR_ATTRIBUTE ("Colonizators Count", GetColonizatorsCount, SetColonizatorsCount, int, 0, Urho3D::AM_DEFAULT);
}

void ColonizatorsUnit::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    assert (map);

    District *district = map->GetDistrictByHash (positionHash_);
    if (district)
    {
        Urho3D::Sphere sphere (district->GetUnitPosition (), 0.15f);
        debug->AddSphere (sphere, Urho3D::Color::GREEN, depthTest);
    }
}

int ColonizatorsUnit::GetColonizatorsCount () const
{
    return colonizatorsCount_;
}

void ColonizatorsUnit::SetColonizatorsCount (int colonizatorsCount)
{
    colonizatorsCount_ = colonizatorsCount;
}

float ColonizatorsUnit::GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const
{
    return 0.0f;
}

bool ColonizatorsUnit::ApplyDamage (GameConfiguration *configuration, float damage)
{
    return false;
}

Urho3D::String ColonizatorsUnit::GetUnitTypeTag () const
{
    return TAG_COLONIZATORS;
}
}
