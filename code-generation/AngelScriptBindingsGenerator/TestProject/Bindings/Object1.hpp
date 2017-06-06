#pragma once
#include <TestProject/Object1.hpp>

namespace TestProject
{
namespace Bindings
{
const Urho3D::StringHash wrapper_MY_CONSTANT ()
{
    return MY_CONSTANT;
}

Urho3D::CScriptArray * wrapper_MyFreeFunction_argument (Urho3D::CScriptArray * argument)
{
    Urho3D::Vector <Object1 *> result = MyFreeFunction (Urho3D::ArrayToPODVector <float> (argument));
    return Urho3D::VectorToHandleArray <Object1 > (result, "Array <MyBaseObject @>");
}

const Urho3D::String & wrapper_MyBaseObject_StaticFunction ()
{
    const Urho3D::String & result = Object1::StaticFunction ();
    return result;
}

Object1 * wrapper_MyBaseObject_constructor ()
{
    return new Object1 (Urho3D::GetScriptContext ());
}

void RegisterMyFreeFunction (asIScriptEngine *engine)
{
    engine->RegisterGlobalFunction ("float MyFreeFunction ()", asFUNCTION (MyFreeFunction), asCALL_CDECL);
    engine->RegisterGlobalFunction ("Array <MyBaseObject @> @ MyFreeFunction (Array <float> @ argument)", asFUNCTION (wrapper_MyFreeFunction_argument), asCALL_CDECL);
}

void RegisterMyBaseObjectStaticFunction (asIScriptEngine *engine)
{
    engine->RegisterGlobalFunction (" String  MyBaseObjectStaticFunction ()", asFUNCTION (wrapper_MyBaseObject_StaticFunction), asCALL_CDECL);
}

void RegisterMY_CONSTANT (asIScriptEngine *engine)
{
    engine->RegisterGlobalFunction ("const StringHash get_MY_CONSTANT ()", asFUNCTION (wrapper_MY_CONSTANT), asCALL_CDECL);
}

template <class T> void RegisterMyBaseObject (asIScriptEngine *engine, const char *className, bool registerConstructors)
{
    Urho3D::RegisterObject <T> (engine, className);

    if (registerConstructors)
    {
        Urho3D::RegisterSubclass <Urho3D::Object, T> (engine, "Object", className);

        engine->RegisterObjectBehaviour (className, asBEHAVE_FACTORY, (Urho3D::String (className) + "@+ f ()").CString (), asFUNCTION (wrapper_MyBaseObject_constructor), asCALL_CDECL);
    }

    engine->RegisterObjectMethod (className, "MyBaseObject @+ DoSomething (float first = 3.14f, uint second = 10) ", asMETHOD (T, DoSomething), asCALL_THISCALL);
    engine->RegisterObjectMethod (className, "String get_string () const", asMETHOD (T, GetString), asCALL_THISCALL);
    engine->RegisterObjectMethod (className, "void set_string ( String  string) ", asMETHOD (T, SetString), asCALL_THISCALL);
    engine->RegisterObjectMethod (className, "float get_number () const", asMETHOD (T, GetNumber), asCALL_THISCALL);
    engine->RegisterObjectMethod (className, "void set_number (float number) ", asMETHOD (T, SetNumber), asCALL_THISCALL);
}

}
}
