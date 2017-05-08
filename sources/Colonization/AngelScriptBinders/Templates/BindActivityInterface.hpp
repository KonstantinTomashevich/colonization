#pragma once
#include <Colonization/BuildConfiguration.hpp>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/Activity.hpp>

namespace Colonization
{
template <class T> void BindActivityInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterSubclass <Activity, T> (engine, "Activity", className.CString ());
    engine->RegisterObjectMethod (className.CString (), "void Start ()", asMETHOD (T, Start), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void Update (float timeStep)", asMETHOD (T, Update), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void Stop ()", asMETHOD (T, Stop), asCALL_THISCALL);
}
}
