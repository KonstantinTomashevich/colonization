#include <Colonization/BuildConfiguration.hpp>
#include "PlayerInfo.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
PlayerInfo::PlayerInfo (Urho3D::Context *context) : Urho3D::Component (context),
    name_ ("NoName"),
    points_ (0.0f)
{

}

PlayerInfo::~PlayerInfo ()
{

}

void PlayerInfo::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <PlayerInfo> (COLONIZATION_CORE_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Name", GetName, SetName, Urho3D::String, Urho3D::String ("NoName"), Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Points", GetPoints, SetPoints, float, 0.0f, Urho3D::AM_DEFAULT);
}

const Urho3D::String &PlayerInfo::GetName () const
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
}
