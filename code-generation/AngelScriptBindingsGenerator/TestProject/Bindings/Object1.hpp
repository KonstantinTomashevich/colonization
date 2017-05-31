#pragma once
#include <TestProject/Object1.hpp>

namespace TestProject
{
namespace Bindings
{
Urho3D::CScriptArray * wrapper_MyFreeFunction_argument (Urho3D::CScriptArray * argument)
{
    Urho3D::Vector <Object1 *> result = MyFreeFunction (Urho3D::ArrayToPODVector <float> (argument));
    return Urho3D::VectorToHandleArray <Object1 *> (result, "Array <MyBaseObject @>");
}

Object1 * wrapper_Object1_constructor ()
{
    return Object1 (Urho3D::GetScriptContext ());
}

void RegisterMyFreeFunction (asIScriptEngine *engine)
{


}

void RegisterMyBaseObjectStaticFunction (asIScriptEngine *engine)
{

}

void RegisterMY_CONSTANT (asIScriptEngine *engine)
{

}

template <class T> void RegisterMyBaseObject (asIScriptEngine *engine, char *className)
{

}

}
}
