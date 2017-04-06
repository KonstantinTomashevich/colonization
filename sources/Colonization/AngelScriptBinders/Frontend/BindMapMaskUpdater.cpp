#include <Colonization/BuildConfiguration.hpp>
#include "BindMapMaskUpdater.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Colonization/Frontend/MapMaskUpdater.hpp>

namespace Colonization
{
Urho3D::CScriptArray *script_GetAllCachedResources (Urho3D::StringHash resourceType)
{
    Urho3D::ResourceCache *cache = Urho3D::GetScriptContext ()->GetSubsystem <Urho3D::ResourceCache> ();
    Urho3D::PODVector <Urho3D::Resource *> resources;
    cache->GetResources (resources, resourceType);
    return Urho3D::VectorToHandleArray <Urho3D::Resource> (resources, "Array <Resource@>");
}

void BindMapMaskUpdater (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <MapMaskUpdater> (engine, "MapMaskUpdater");
    BindMapMaskUpdaterInterface (script, "MapMaskUpdater");
    engine->RegisterGlobalFunction ("Array <Resource@> @GetAllCachedResources (StringHash resourceType)", asFUNCTION (script_GetAllCachedResources), asCALL_CDECL);
}

void BindMapMaskUpdaterInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "void RecalculateMaskImage ()", asMETHOD (MapMaskUpdater, RecalculateMaskImage), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "StringHash GetDistrictByPoint (Vector3 point)", asMETHOD (MapMaskUpdater, GetDistrictByPoint), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "StringHash GetDistrictByColor (Color color)", asMETHOD (MapMaskUpdater, GetDistrictByColor), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "StringHash GetDistrictByColorInt (uint color)", asMETHOD (MapMaskUpdater, GetDistrictByColorInt), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "uint GetDistrictColorInt (StringHash districtHash)", asMETHOD (MapMaskUpdater, GetDistrictColorInt), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "IntVector2 get_mapMaskSize () const", asMETHOD (MapMaskUpdater, GetMapMaskSize), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_mapMaskSize (IntVector2 mapMaskSize)", asMETHOD (MapMaskUpdater, SetMapMaskSize), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "int get_mapMaskComponents () const", asMETHOD (MapMaskUpdater, GetMapMaskComponents), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_mapMaskComponents (int mapMaskComponents)", asMETHOD (MapMaskUpdater, SetMapMaskComponents), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "int get_districtBorderLineWidth () const", asMETHOD (MapMaskUpdater, GetDistrictBorderLineWidth), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_districtBorderLineWidth (int districtBorderLineWidth)", asMETHOD (MapMaskUpdater, SetDistrictBorderLineWidth), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_selectedDistrictColorModifer () const", asMETHOD (MapMaskUpdater, GetSelectedDistrictColorModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_selectedDistrictColorModifer (float selectedDistrictColorModifer)", asMETHOD (MapMaskUpdater, SetSelectedDistrictColorModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Color get_districtBorderLineColor () const", asMETHOD (MapMaskUpdater, GetDistrictBorderLineColor), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_districtBorderLineColor (Color districtBorderLineColor)", asMETHOD (MapMaskUpdater, SetDistrictBorderLineColor), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Color get_visibleDistrictColor () const", asMETHOD (MapMaskUpdater, GetVisibleDistrictColor), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_visibleDistrictColor (Color visibleDistrictColor)", asMETHOD (MapMaskUpdater, SetVisibleDistrictColor), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Color get_districtUnderFogColor () const", asMETHOD (MapMaskUpdater, GetDistrictUnderFogColor), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_districtUnderFogColor (Color districtUnderFogColor)", asMETHOD (MapMaskUpdater, SetDistrictUnderFogColor), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Image @+ get_fogOfWarMaskImage () const", asMETHOD (MapMaskUpdater, GetFogOfWarMaskImage), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "Texture2D @+ get_fogOfWarMaskTexture () const", asMETHOD (MapMaskUpdater, GetFogOfWarMaskTexture), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Image @+ get_maskImage () const", asMETHOD (MapMaskUpdater, GetMaskImage), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "Texture2D @+ get_maskTexture () const", asMETHOD (MapMaskUpdater, GetMaskTexture), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Vector3 get_mapMinPoint () const", asMETHOD (MapMaskUpdater, GetMapMinPoint), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_mapMinPoint (Vector3 mapMinPoint) const", asMETHOD (MapMaskUpdater, SetMapMinPoint), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Vector3 get_mapMaxPoint () const", asMETHOD (MapMaskUpdater, GetMapMaxPoint), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_mapMaxPoint (Vector3 mapMaxPoint)", asMETHOD (MapMaskUpdater, SetMapMaxPoint), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "StringHash get_selectedDistrictHash () const", asMETHOD (MapMaskUpdater, GetSelectedDistrictHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_selectedDistrictHash (StringHash selectedDistrictHash)", asMETHOD (MapMaskUpdater, SetSelectedDistrictHash), asCALL_THISCALL);
}
}
