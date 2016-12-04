#pragma once
#include <Colonization/BuildConfiguration.hpp>
#include <Urho3D/AngelScript/APITemplates.h>

namespace Urho3D
{
// Modification of RegisterObject. Allows to register object which isn't directly inherits Urho3D object.
template <class T, class Base> void RegisterObjectWithInheritance (asIScriptEngine *engine, const char *className, const char *baseName)
{
    RegisterRefCounted <T> (engine, className);
    engine->RegisterObjectMethod (className, "StringHash get_type() const", asMETHODPR(T, GetType, () const, StringHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className, "const String& get_typeName() const", asMETHODPR(T, GetTypeName, () const, const String&), asCALL_THISCALL);
    engine->RegisterObjectMethod (className, "const String& get_category() const", asMETHODPR(T, GetCategory, () const, const String&), asCALL_THISCALL);
    engine->RegisterObjectMethod (className, "void SendEvent(const String&in, VariantMap& eventData = VariantMap())", asFUNCTION(ObjectSendEvent<T>), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod (className, "bool HasSubscribedToEvent(const String&in)", asFUNCTION(ObjectHasSubscribedToEvent<T>), asCALL_CDECL_OBJLAST);
    engine->RegisterObjectMethod (className, "bool HasSubscribedToEvent(Object@+, const String&in)", asFUNCTION(ObjectHasSubscribedToSenderEvent<T>), asCALL_CDECL_OBJLAST);
    RegisterSubclass <Base, T> (engine, baseName,  className);
}
}

#ifdef NDEBUG
    #define CHECK_ANGELSCRIPT_RETURN(expression) expression
#else
    #define CHECK_ANGELSCRIPT_RETURN(expression) assert ((expression) >= 0)
#endif

#define GLOBAL_CONSTANT_GETTER(type, constant) \
    type Get ## constant () \
    {\
        return constant; \
    } \

#define BIND_GLOBAL_CONSTANT(asEngine, asType, constant_namespace, constant) \
    CHECK_ANGELSCRIPT_RETURN ( \
                asEngine->RegisterGlobalFunction ( \
                        (#asType + Urho3D::String (" get_") + #constant_namespace + Urho3D::String ("__")\
                            + #constant + Urho3D::String (" ()")).CString (), \
                        asFUNCTION (constant_namespace::Get ## constant), asCALL_CDECL) \
                ) \

#define OBJECT_PROPERTY_GETTER(objectType, propertyType, propertyName) \
    propertyType objectType ## _get_ ## propertyName (objectType * object) \
    { \
    return object->propertyName; \
    } \

#define OBJECT_PROPERTY_SETTER(objectType, propertyType, propertyName) \
    void objectType ## _set_ ## propertyName (objectType * object, propertyType value) \
    { \
    object->propertyName = value; \
    } \

#define OBJECT_PROPERTY_GETTER_AND_SETTER(objectType, propertyType, propertyName) \
    OBJECT_PROPERTY_GETTER (objectType, propertyType, propertyName) \
    OBJECT_PROPERTY_SETTER (objectType, propertyType, propertyName) \

#define BIND_OBJECT_PROPERTY_GETTER(asEngine, cxxObjectType, asObjectType, asPropertyType, propertyName) \
    CHECK_ANGELSCRIPT_RETURN ( \
                asEngine->RegisterObjectMethod (#asObjectType, (#asPropertyType + Urho3D::String (" get_") \
                + #propertyName + Urho3D::String (" ()")).CString (), asFUNCTION (cxxObjectType ## _get_ ## propertyName ), \
                asCALL_CDECL_OBJFIRST) \
                ) \


#define BIND_OBJECT_PROPERTY_SETTER(asEngine, cxxObjectType, asObjectType, asPropertyType, propertyName) \
    CHECK_ANGELSCRIPT_RETURN ( \
                asEngine->RegisterObjectMethod (#asObjectType, (Urho3D::String ("void set_") \
                + #propertyName + Urho3D::String (" (") + #asPropertyType \
                + Urho3D::String (" value)")).CString (), asFUNCTION (cxxObjectType ## _set_ ## propertyName ), \
                asCALL_CDECL_OBJFIRST) \
                ) \

#define BIND_OBJECT_PROPERTY_GETTER_AND_SETTER(asEngine, cxxObjectType, asObjectType, asPropertyType, propertyName) \
    BIND_OBJECT_PROPERTY_GETTER (asEngine, cxxObjectType, asObjectType, asPropertyType, propertyName); \
    BIND_OBJECT_PROPERTY_SETTER (asEngine, cxxObjectType, asObjectType, asPropertyType, propertyName)

#define OBJECT_ARRAY_PROPERTY_ACESSOR(objectType, arrayType, arrayValuesType, propertyName, asArrayDecl) \
    Urho3D::CScriptArray *objectType ## _get_ ## propertyName (objectType * object) \
    { \
        return Urho3D::VectorToArray <arrayValuesType> (object->propertyName, asArrayDecl); \
    } \
    \
    void objectType ## _set_ ## propertyName (objectType * object, Urho3D::CScriptArray *array) \
    { \
        object->propertyName = Urho3D::ArrayTo ## arrayType <arrayValuesType> (array); \
    } \

#define BIND_OBJECT_ARRAY_PROPERTY(asEngine, cxxObjectType, asObjectType, asArrayValuesType, propertyName) \
    CHECK_ANGELSCRIPT_RETURN ( \
                asEngine->RegisterObjectMethod (#asObjectType, (Urho3D::String ("Array <") \
                + #asArrayValuesType + Urho3D::String (">@") + Urho3D::String (" get_") \
                + #propertyName + Urho3D::String (" ()")).CString (), \
                asFUNCTION (cxxObjectType ## _get_ ## propertyName), \
                asCALL_CDECL_OBJFIRST) \
                ); \
    CHECK_ANGELSCRIPT_RETURN ( \
                asEngine->RegisterObjectMethod (#asObjectType, (Urho3D::String ("void set_") \
                + #propertyName + Urho3D::String (" (Array <") + #asArrayValuesType \
                + Urho3D::String (">@ array)")).CString (), \
                asFUNCTION (cxxObjectType ## _set_ ## propertyName), \
                asCALL_CDECL_OBJFIRST) \
                );
