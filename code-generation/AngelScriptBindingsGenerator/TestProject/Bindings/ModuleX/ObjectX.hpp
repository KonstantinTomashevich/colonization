#pragma once
#include <TestProject/ModuleX/ObjectX.hpp>
#include <TestProject/Bindings/Object1.hpp>

namespace TestProject
{
namespace Bindings
{
ObjectX * wrapper_ObjectX_constructor ()
{
    return ObjectX (Urho3D::GetScriptContext ());
}

ObjectX * wrapper_ObjectX_constructor_myEnumType (MyEnumType myEnumType)
{
    return ObjectX (Urho3D::GetScriptContext (), myEnumType);
}

Urho3D::CScriptArray * wrapper_ObjectX_GetArray (ObjectX* objectPtr)
{
    Urho3D::Vector <Urho3D::String> result = objectPtr->GetArray ();
    return Urho3D::VectorToArray <Urho3D::String> (result, "Array <Array <String> @>");
}

void wrapper_ObjectX_SetArray_array (ObjectX* objectPtr, Urho3D::CScriptArray * array)
{
    objectPtr->SetArray (Urho3D::ArrayToVector <Urho3D::String> (array));
}

MyNameSpace::SubsystemX *wrapper_subsystemX ()
{
    return Urho3D::GetScriptContext ()->GetSubsystem <MyNameSpace::SubsystemX> ();
}

template <class T> void RegisterObjectX (asIScriptEngine *engine, char *className, bool registerConstructors)
{
    Urho3D::RegisterSubclass <Object1, T> (engine, "MyBaseObject", className);
    RegisterMyBaseObject <T> (engine, className, false);

    if (registerConstructors)
    {
        engine->RegisterObjectBehaviour (className, asBEHAVE_FACTORY, (Urho3D::String (className) + "@+ f ()").CString (), asFUNCTION (wrapper_ObjectX_constructor), asCALL_CDECL);
        engine->RegisterObjectBehaviour (className, asBEHAVE_FACTORY, (Urho3D::String (className) + "@+ f (MyEnumType myEnumType)").CString (), asFUNCTION (wrapper_ObjectX_constructor_myEnumType), asCALL_CDECL);
    }

    engine->RegisterObjectMethod (className, "Array <String> @ get_array () const", asFUNCTION (wrapper_ObjectX_GetArray), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod (className, "void set_array ( Array <String>  @ array) ", asFUNCTION (wrapper_ObjectX_SetArray_array), asCALL_CDECL_OBJFIRST);
}

void RegisterMyEnumType (asIScriptEngine *engine)
{
    engine->RegisterEnum ("MyEnumType");
    engine->RegisterEnumValue ("MyEnumType", "ME_THIRD_VALUE", ME_THIRD_VALUE);
    engine->RegisterEnumValue ("MyEnumType", "ME_VALUES_COUNT", ME_VALUES_COUNT);
    engine->RegisterEnumValue ("MyEnumType", "ME_SECOND_VALUE", ME_SECOND_VALUE);
    engine->RegisterEnumValue ("MyEnumType", "ME_FIRST_VALUE", ME_FIRST_VALUE);
}

void RegistersubsystemX (asIScriptEngine *engine)
{
    engine->RegisterGlobalFunction ("SubsystemX @+ get_subsystemX ()", asFUNCTION (wrapper_subsystemX), asCALL_CDECL);
}

}
}
