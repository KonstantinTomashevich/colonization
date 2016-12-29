#include <Colonization/BuildConfiguration.hpp>
#include "PlayerInfo.hpp"
#include <Urho3D/Core/Context.h>
#include <Colonization/Utils/Categories.hpp>

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
    context_->RegisterFactory <PlayerInfo> (COLONIZATION_CORE_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Name", GetName, SetName, Urho3D::String, Urho3D::String ("NoName"), Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Points", GetPoints, SetPoints, float, 0.0f, Urho3D::AM_DEFAULT);
}

Urho3D::String PlayerInfo::GetName ()
{
    return name_;
}

void PlayerInfo::SetName (Urho3D::String name)
{
    name_ = name;
}

float PlayerInfo::GetPoints ()
{
    return points_;
}

void PlayerInfo::SetPoints (float points)
{
    points_ = points;
}
}
