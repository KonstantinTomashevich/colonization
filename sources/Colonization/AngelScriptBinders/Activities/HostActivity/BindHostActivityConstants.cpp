#include <Colonization/BuildConfiguration.hpp>
#include "BindHostActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Activities/HostActivity/HostActivityConstants.hpp>

namespace Colonization
{
Urho3D::StringHash get_VAR_REPLICATED_NODES_COUNT ()
{
    return VAR_REPLICATED_NODES_COUNT;
}

Urho3D::StringHash get_VAR_MAP_FOLDER ()
{
    return VAR_MAP_FOLDER;
}

Urho3D::StringHash get_VAR_MAP_INFO_PATH ()
{
    return VAR_MAP_INFO_PATH;
}

void BindHostActivityConstants (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("StringHash get_VAR_REPLICATED_NODES_COUNT ()", asFUNCTION (get_VAR_REPLICATED_NODES_COUNT), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_VAR_MAP_FOLDER ()", asFUNCTION (get_VAR_MAP_FOLDER), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_VAR_MAP_INFO_PATH ()", asFUNCTION (get_VAR_MAP_INFO_PATH), asCALL_CDECL);
}
}
