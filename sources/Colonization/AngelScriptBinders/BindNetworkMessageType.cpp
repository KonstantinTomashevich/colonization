#include <Colonization/BuildConfiguration.hpp>
#include "BindNetworkMessageType.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Backend/MessagesHandler.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
void BindNetworkMessageType(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterEnum ("NetworkMessageType");
    engine->RegisterEnumValue ("NetworkMessageType", "CTS_NETWORK_MESSAGE_SEND_CHAT_MESSAGE", CTS_NETWORK_MESSAGE_SEND_CHAT_MESSAGE);
    engine->RegisterEnumValue ("NetworkMessageType", "CTS_NETWORK_MESSAGE_SEND_PRIVATE_MESSAGE", CTS_NETWORK_MESSAGE_SEND_PRIVATE_MESSAGE);
    engine->RegisterEnumValue ("NetworkMessageType", "CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION", CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
    engine->RegisterEnumValue ("NetworkMessageType", "STC_NETWORK_MESSAGE_SEND_PLAYER_STATS", STC_NETWORK_MESSAGE_SEND_PLAYER_STATS);
    engine->RegisterEnumValue ("NetworkMessageType", "STC_NETWORK_MESSAGE_CHAT_MESSAGE", STC_NETWORK_MESSAGE_CHAT_MESSAGE);
    engine->RegisterEnumValue ("NetworkMessageType", "STC_NETWORK_MESSAGE_TEXT_INFO_FROM_SERVER", STC_NETWORK_MESSAGE_TEXT_INFO_FROM_SERVER);
}
}
