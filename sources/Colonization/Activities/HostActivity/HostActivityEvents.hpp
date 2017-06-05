#pragma once
#include <Urho3D/Math/StringHash.h>

namespace Colonization
{
//@ASBindGen Constant
const Urho3D::String EVENT_HOST_REQUEST_GAME_START ("HostRequestGameStart");
//@ASBindGen Constant
const Urho3D::String EVENT_HOST_REQUEST_KICK_PLAYER ("HostRequestKickPlayer");
namespace HostRequestKickPlayer
{
//@ASBindGen Constant OverrideName=HostRequestKickPlayer_PLAYER_NAME AddNamespaceToName=HostRequestKickPlayer
const Urho3D::String PLAYER_NAME ("PlayerName");
}

//@ASBindGen Constant
const Urho3D::String EVENT_HOST_REQUEST_SELECT_MAP ("HostRequestSelectMap");
namespace HostRequestSelectMap
{
//@ASBindGen Constant OverrideName=HostRequestSelectMap_MAP_FOLDER AddNamespaceToName=HostRequestSelectMap
const Urho3D::String MAP_FOLDER ("MapFolder");
//@ASBindGen Constant OverrideName=HostRequestSelectMap_MAP_INFO_PATH AddNamespaceToName=HostRequestSelectMap
const Urho3D::String MAP_INFO_PATH ("MapInfoPath");
}
}
