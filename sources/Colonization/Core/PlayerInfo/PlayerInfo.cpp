#include <Colonization/BuildConfiguration.hpp>
#include "PlayerInfo.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Colonization/Core/PlayerInfo/PlayerInfoConstants.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
PlayerInfo::PlayerInfo (Urho3D::Context *context) : Urho3D::Component (context),
    name_ ("NoName"),
    points_ (0.0f),
    color_ (Urho3D::Color::GRAY),
    isReadyForStart_ (false),
    progressToVictory_ ()
{

}

PlayerInfo::~PlayerInfo ()
{

}

void PlayerInfo::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <PlayerInfo> (COLONIZATION_CORE_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Name", GetName, SetName, Urho3D::String, Urho3D::String ("NoName"), Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Points", GetPoints, SetPoints, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Color", GetColor, SetColor, Urho3D::Color, Urho3D::Color::GRAY, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Ready For Start", IsReadyForStart, SetIsReadyForStart, bool, false, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Progress To Victory", GetProgressToVictory, SetProgressToVictory,
                                     Urho3D::VariantMap, Urho3D::Variant::emptyVariantVector, Urho3D::AM_DEFAULT);
}

Urho3D::String PlayerInfo::GetName () const
{
    return name_;
}

void PlayerInfo::SetName (const Urho3D::String &name)
{
    name_ = name;
}

float PlayerInfo::GetPoints () const
{
    return points_;
}

void PlayerInfo::SetPoints (float points)
{
    points_ = points;
}

Urho3D::Color PlayerInfo::GetColor () const
{
    return color_;
}

void PlayerInfo::SetColor (const Urho3D::Color &color)
{
    color_ = color;
}

bool PlayerInfo::IsReadyForStart () const
{
    return isReadyForStart_;
}

void PlayerInfo::SetIsReadyForStart (bool isReadyForStart)
{
    isReadyForStart_ = isReadyForStart;
}

Urho3D::VariantMap PlayerInfo::GetProgressToVictory () const
{
    return progressToVictory_;
}

void PlayerInfo::SetProgressToVictory (const Urho3D::VariantMap &progressToVictory)
{
    progressToVictory_ = progressToVictory;
}

float PlayerInfo::GetProgressToVictoryOfType (Urho3D::StringHash victoryType) const
{
    Urho3D::Variant *value = progressToVictory_ [victoryType];
    if (value && value->GetType () == Urho3D::VAR_VARIANTMAP)
    {
        return value->GetVariantMap () [PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY]->GetFloat ();
    }
    else
    {
        return 0.0f;
    }
}

Urho3D::VariantMap PlayerInfo::GetProgressToVictoryOfTypeInfo (Urho3D::StringHash victoryType) const
{
    Urho3D::Variant *value = progressToVictory_ [victoryType];
    if (value && value->GetType () == Urho3D::VAR_VARIANTMAP)
    {
        Urho3D::VariantMap progressToVictoryInfo = value->GetVariantMap ();
        return progressToVictoryInfo;
    }
    else
    {
        return Urho3D::Variant::emptyVariantMap;
    }
}

void PlayerInfo::SetProgressToVictoryOfTypeInfo (Urho3D::StringHash victoryType, const Urho3D::VariantMap &progressToVictoryInfo)
{
    progressToVictory_ [victoryType] = progressToVictoryInfo;
}
}
