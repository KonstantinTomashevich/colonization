#pragma once
#include <Urho3D/Math/StringHash.h>

namespace Colonization
{
const Urho3D::String EVENT_HOST_REQUEST_GAME_START ("HostRequestGameStart");
const Urho3D::String EVENT_HOST_REQUEST_KICK_PLAYER ("HostRequestKickPlayer");
namespace HostRequestKickPlayer
{
const Urho3D::String PLAYER_NAME ("PlayerName");
}

const Urho3D::String EVENT_HOST_REQUEST_SELECT_MAP ("HostRequestSelectMap");
namespace HostRequestSelectMap
{
const Urho3D::String MAP_FOLDER ("MapFolder");
const Urho3D::String MAP_INFO_PATH ("MapInfoPath");
}
}
