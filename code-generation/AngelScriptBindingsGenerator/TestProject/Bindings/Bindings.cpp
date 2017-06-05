#include "Bindings.hpp"
#include <TestProject/Bindings/Object1.hpp>
#include <TestProject/Bindings/ModuleX/ObjectX.hpp>

namespace TestProject
{
namespace Bindings
{
void RegisterAnything (asIScriptEngine *engine)
{
    RegisterClassesForwardDeclarations (engine);
    RegisterEnums (engine);
    RegisterConstants (engine);
    RegisterFreeFunctions (engine);
    RegisterUrho3DSubsystems (engine);
    RegisterClasses (engine);
}

void RegisterClassesForwardDeclarations (asIScriptEngine *engine)
{
    engine->RegisterObjectType ("MyBaseObject", 0, asOBJ_REF);
    engine->RegisterObjectType ("ObjectX", 0, asOBJ_REF);
}

void RegisterEnums (asIScriptEngine *engine)
{
    RegisterMyEnumType (engine);
}

void RegisterConstants (asIScriptEngine *engine)
{
    RegisterMY_CONSTANT (engine);
}

void RegisterFreeFunctions (asIScriptEngine *engine)
{
    RegisterMyFreeFunction (engine);
    RegisterMyBaseObjectStaticFunction (engine);
}

void RegisterUrho3DSubsystems (asIScriptEngine *engine)
{
    RegistersubsystemX (engine);
}

void RegisterClasses (asIScriptEngine *engine)
{
    RegisterMyBaseObject <Object1> (engine, "MyBaseObject", true);
    RegisterObjectX <ObjectX> (engine, "ObjectX", true);
}

}
}
