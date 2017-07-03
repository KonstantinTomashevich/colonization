#include <Colonization/BuildConfiguration.hpp>
#include "FleetUnit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Unit/UnitTags.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
const char *warShipsHealthPointsElementsNames [] =
{
    "War Ships Count:",
    "   War Ship HP",
    0
};

FleetUnit::FleetUnit (Urho3D::Context *context) : Unit (context),
    warShipsCount_ (0),
    warShipsHealthPoints_ ()
{
    unitType_ = UNIT_FLEET;
}

FleetUnit::~FleetUnit ()
{

}

void FleetUnit::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <FleetUnit> (COLONIZATION_CORE_CATEGORY);
    URHO3D_COPY_BASE_ATTRIBUTES (Unit);
    URHO3D_ACCESSOR_ATTRIBUTE ("War Ships Count", GetWarShipsCount, SetWarShipsCount, int, 0, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE  ("War Ships Health Points",
                               GetWarShipsHealthPointsAttribute,
                               SetWarShipsHealthPointsAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector,
                               warShipsHealthPointsElementsNames, Urho3D::AM_DEFAULT);
}

void FleetUnit::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    assert (map);

    District *district = map->GetDistrictByHash (positionHash_);
    if (district)
    {
        Urho3D::Sphere sphere (district->GetUnitPosition (), 0.15f);
        debug->AddSphere (sphere, Urho3D::Color::BLUE, depthTest);
    }
}

int FleetUnit::GetWarShipsCount () const
{
    return warShipsCount_;
}

void FleetUnit::SetWarShipsCount (int warShipsCount)
{
    warShipsCount_ = warShipsCount;
    if (warShipsHealthPoints_.Size () > warShipsCount_)
    {
        warShipsHealthPoints_.Erase (warShipsCount_, warShipsHealthPoints_.Size () - warShipsCount_);
    }

    while (warShipsHealthPoints_.Size () < warShipsCount_)
    {
        warShipsHealthPoints_.Push (100.0f);
    }
}

Urho3D::PODVector <float> FleetUnit::GetWarShipsHealthPoints () const
{
    return warShipsHealthPoints_;
}

void FleetUnit::SetWarShipsHealthPoints (const Urho3D::PODVector <float> &warShipsHealthPoints)
{
    warShipsHealthPoints_ = warShipsHealthPoints;
    if (warShipsHealthPoints_.Size () > warShipsCount_)
    {
        warShipsHealthPoints_.Erase (warShipsCount_, warShipsHealthPoints_.Size () - warShipsCount_);
    }

    while (warShipsHealthPoints_.Size () < warShipsCount_)
    {
        warShipsHealthPoints_.Push (100.0f);
    }
}

Urho3D::VariantVector FleetUnit::GetWarShipsHealthPointsAttribute () const
{
    Urho3D::VariantVector vector;
    vector.Push (warShipsCount_);
    for (int index = 0; index < warShipsCount_; index++)
    {
        if (index < warShipsHealthPoints_.Size ())
        {
            vector.Push (warShipsHealthPoints_.At (index));
        }
        else
        {
            vector.Push (100.0f);
        }
    }
    return vector;
}

void FleetUnit::SetWarShipsHealthPointsAttribute (const Urho3D::VariantVector &warShipsHealthPoints)
{
    warShipsHealthPoints_.Clear ();
    if (warShipsHealthPoints.Empty ())
    {
        return;
    }

    for (int index = 0; index < warShipsCount_; index++)
    {
        if (index + 1 < warShipsHealthPoints.Size ())
        {
            warShipsHealthPoints_.Push (warShipsHealthPoints.At (index + 1).GetFloat ());
        }
        else
        {
            warShipsHealthPoints_.Push (100.0f);
        }
    }
}

float FleetUnit::GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const
{
    if (isNaval)
    {
        return configuration->GetWarShipBasicNavalAttackForce () * warShipsCount_;
    }
    else
    {
        return 0.0f;
    }
}

bool FleetUnit::ApplyDamage (GameConfiguration *configuration, float damage)
{
    float toLoss = damage * 100.0f / configuration->GetWarShipBasicHealth ();
    if (warShipsCount_ > 0)
    {
        int shipToDamageIndex = Urho3D::Random (0, warShipsCount_);
        while (toLoss > 0.0f && warShipsCount_ > 0)
        {
            float thisShipDamage = configuration->GetWarShipBasicHealth () * Urho3D::Random (0.2f, 1.0f);
            if (thisShipDamage > toLoss)
            {
                thisShipDamage = toLoss;
            }

            if (thisShipDamage > warShipsHealthPoints_.At (shipToDamageIndex))
            {
                thisShipDamage = warShipsHealthPoints_.At (shipToDamageIndex);
            }

            warShipsHealthPoints_.At (shipToDamageIndex) -= thisShipDamage;
            toLoss -= thisShipDamage;

            if (warShipsHealthPoints_.At (shipToDamageIndex) <= 0.0f)
            {
                warShipsHealthPoints_.Erase (shipToDamageIndex);
                warShipsCount_ -= 1;
            }

            shipToDamageIndex = Urho3D::Random (0, warShipsCount_);
        }
    }
    return IsAlive ();
}

bool FleetUnit::IsAlive () const
{
    return warShipsCount_ > 0;
}

Urho3D::String FleetUnit::GetUnitTypeTag () const
{
    return TAG_FLEET;
}
}
