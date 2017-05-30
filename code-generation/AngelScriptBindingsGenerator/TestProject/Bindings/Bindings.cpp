#include "Bindings.hpp"
#include <TestProject/Bindings/Object1.hpp>
#include <TestProject/Bindings/ModuleX/ObjectX.hpp>

namespace TestProject
{
namespace Bindings
{
void RegisterAnything (asIScriptEngine *engine)
{
    void RegisterClassesForwardDeclarations (engine);
    void RegisterEnums (engine);
    void RegisterConstants (engine);
    void RegisterFreeFunctions (engine);
    void RegisterUrho3DSubsystems (engine);
    void RegisterClasses (engine);
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
    RegisterSubsystemX (engine);
}

void RegisterClasses (asIScriptEngine *engine)
{
    RegisterObject1 <Object1> (engine, "MyBaseObject");
    RegisterObjectX <ObjectX> (engine, "ObjectX");
}

}
}
