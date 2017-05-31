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
    return Urho3D::VectorToArray <Urho3D::String> (result, "Array <String>");
}

void wrapper_ObjectX_SetArray_array (ObjectX* objectPtr, Urho3D::CScriptArray * array)
{
    objectPtr->SetArray (Urho3D::ArrayToVector <Urho3D::String> (array));
}

void RegisterMyEnumType (asIScriptEngine *engine)
{

}

template <class T> void RegisterObjectX (asIScriptEngine *engine, char *className)
{

    Urho3D::RegisterSubclass <Object1, T> (engine, "MyBaseObject", className);
    RegisterMyBaseObject <T> (engine, className);

}

}
}
