#include <Colonization/BuildConfiguration.hpp>
#include "BindMapMaskUpdater.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Frontend/MapMaskUpdater.hpp>

namespace Colonization
{
void BindMapMaskUpdater (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <MapMaskUpdater> (engine, "MapMaskUpdater");
    BindMapMaskUpdaterInterface (script, "MapMaskUpdater");
}

void BindMapMaskUpdaterInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "StringHash GetDistrictByPoint (Vector3 point)", asMETHOD (MapMaskUpdater, GetDistrictByPoint), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Image @+ get_maskImage ()", asMETHOD (MapMaskUpdater, GetMaskImage), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "Texture2D @+ get_maskTexture ()", asMETHOD (MapMaskUpdater, GetMaskTexture), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Vector3 get_mapMinPoint () const", asMETHOD (MapMaskUpdater, GetMapMinPoint), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "Vector3 set_mapMinPoint (Vector3 mapMinPoint) const", asMETHOD (MapMaskUpdater, SetMapMinPoint), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Vector3 get_mapMaxPoint () const", asMETHOD (MapMaskUpdater, GetMapMaxPoint), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "Vector3 set_mapMaxPoint (Vector3 mapMaxPoint) const", asMETHOD (MapMaskUpdater, SetMapMaxPoint), asCALL_THISCALL);
}
}
