#pragma once

namespace TestProject
{
namespace Bindings
{
void RegisterAnything (asIScriptEngine *engine);
void RegisterClassesForwardDeclarations (asIScriptEngine *engine);
void RegisterEnums (asIScriptEngine *engine);
void RegisterConstants (asIScriptEngine *engine);
void RegisterFreeFunctions (asIScriptEngine *engine);
void RegisterUrho3DSubsystems (asIScriptEngine *engine);
void RegisterClasses (asIScriptEngine *engine);
}
}
