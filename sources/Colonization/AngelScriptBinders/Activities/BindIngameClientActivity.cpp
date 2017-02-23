#include <Colonization/BuildConfiguration.hpp>
#include "BindIngameClientActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Colonization/Activities/IngameClientActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindActivity.hpp>

namespace Colonization
{
void BindIngameClientActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <IngameClientActivity> (engine, "IngameClientActivity");
    Urho3D::RegisterObjectConstructor <IngameClientActivity> (engine, "IngameClientActivity");
    Urho3D::RegisterSubclass <Activity, IngameClientActivity> (engine, "Activity", "IngameClientActivity");
    BindActivityInterface (script, "IngameClientActivity");

    engine->RegisterObjectMethod ("IngameClientActivity", "String get_serverAdress ()", asMETHOD (IngameClientActivity, GetServerAdress), asCALL_THISCALL);
    engine->RegisterObjectMethod ("IngameClientActivity", "void set_serverAdress (String serverAdress)", asMETHOD (IngameClientActivity, SetServerAdress), asCALL_THISCALL);

    engine->RegisterObjectMethod ("IngameClientActivity", "int get_serverPort ()", asMETHOD (IngameClientActivity, GetServerPort), asCALL_THISCALL);
    engine->RegisterObjectMethod ("IngameClientActivity", "void set_serverPort (int serverPort)", asMETHOD (IngameClientActivity, SetServerPort), asCALL_THISCALL);

    engine->RegisterObjectMethod ("IngameClientActivity", "String get_playerName ()", asMETHOD (IngameClientActivity, GetPlayerName), asCALL_THISCALL);
    engine->RegisterObjectMethod ("IngameClientActivity", "void set_playerName (String playerName)", asMETHOD (IngameClientActivity, SetPlayerName), asCALL_THISCALL);

    engine->RegisterObjectMethod ("IngameClientActivity", "Color get_playerColor ()", asMETHOD (IngameClientActivity, GetPlayerColor), asCALL_THISCALL);
    engine->RegisterObjectMethod ("IngameClientActivity", "void set_playerColor (Color playerColor)", asMETHOD (IngameClientActivity, SetPlayerColor), asCALL_THISCALL);
}
}
