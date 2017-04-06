#include <Colonization/BuildConfiguration.hpp>
#include "BindHostActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Colonization/Activities/HostActivity/GameStateType.hpp>

namespace Colonization
{
void BindGameStateType(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterEnum ("GameStateType");
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_UNITIALIZED", GAME_STATE_UNITIALIZED);
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_WAITING_FOR_START", GAME_STATE_WAITING_FOR_START);
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_PLAYING", GAME_STATE_PLAYING);
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_FINISHED", GAME_STATE_FINISHED);
}
}
