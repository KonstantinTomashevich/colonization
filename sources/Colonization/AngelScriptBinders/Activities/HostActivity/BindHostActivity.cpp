#include <Colonization/BuildConfiguration.hpp>
#include "BindHostActivity.hpp"
#include <Colonization/AngelScriptBinders/Templates/BindActivityInterface.hpp>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Colonization/Activities/HostActivity/HostActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindActivity.hpp>

namespace Colonization
{
void BindHostActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <HostActivity> (engine, "HostActivity");
    Urho3D::RegisterObjectConstructor <HostActivity> (engine, "HostActivity");
    BindActivityInterface <HostActivity> (script, "HostActivity");

    engine->RegisterObjectMethod ("HostActivity", "int get_serverPort ()", asMETHOD (HostActivity, GetServerPort), asCALL_THISCALL);
    engine->RegisterObjectMethod ("HostActivity", "void set_serverPort (int serverPort)", asMETHOD (HostActivity, SetServerPort), asCALL_THISCALL);

    engine->RegisterObjectMethod ("HostActivity", "String get_mapFolder () const", asMETHOD (HostActivity, GetMapFolder), asCALL_THISCALL);
    engine->RegisterObjectMethod ("HostActivity", "void set_mapFolder (String mapFolder)", asMETHOD (HostActivity, SetMapFolder), asCALL_THISCALL);

    engine->RegisterObjectMethod ("HostActivity", "String get_mapInfoPath () const", asMETHOD (HostActivity, GetMapInfoPath), asCALL_THISCALL);
    engine->RegisterObjectMethod ("HostActivity", "void set_mapInfoPath (String mapInfoPath)", asMETHOD (HostActivity, SetMapInfoPath), asCALL_THISCALL);
    engine->RegisterObjectMethod ("HostActivity", "bool IsStartRequested () const", asMETHOD (HostActivity, IsStartRequested), asCALL_THISCALL);
}
}
