#include <Colonization/BuildConfiguration.hpp>
#include "BindPlayerInfo.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/PlayerInfo.hpp>

namespace Colonization
{
void BindPlayerInfo (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <PlayerInfo> (engine, "PlayerInfo");
    BindPlayerInfoInterface (script, "PlayerInfo");
}

void BindPlayerInfoInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "String get_name ()", asMETHOD (PlayerInfo, GetName), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_name (String name)", asMETHOD (PlayerInfo, SetName), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_points ()", asMETHOD (PlayerInfo, GetPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_points (float Points)", asMETHOD (PlayerInfo, SetPoints), asCALL_THISCALL);
}
}
