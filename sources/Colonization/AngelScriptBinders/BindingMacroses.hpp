#pragma once
#include <Colonization/BuildConfiguration.hpp>
#ifdef NDEBUG
    #define CHECK_ANGELSCRIPT_RETURN(expression) expression
#else
    #define CHECK_ANGELSCRIPT_RETURN(expression) assert ((expression) >= 0)
#endif

// Macro for easy defining of constants getters for AngelScript.
#define GETTER(type, constant) \
    type Get ## constant () \
    {\
        return constant; \
    } \

// Macro for easy binding of constants to AngelScript.
#define BIND_CONSTANT(asEngine, asType, constant_namespace, constant) \
    CHECK_ANGELSCRIPT_RETURN ( \
                asEngine->RegisterGlobalFunction ( \
                        (#asType + Urho3D::String (" get_") + #constant_namespace + Urho3D::String ("__")\
                            + #constant + Urho3D::String (" ()")).CString (), \
                        asFUNCTION (constant_namespace::Get ## constant), asCALL_CDECL) \
                ) \

