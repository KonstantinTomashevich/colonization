#pragma once
#include <Urho3D/Scene/Serializable.h>

// These macros fixed after 1.6 release. Apply this fix because we using 1.6 release on Travis CI.
/// Define an attribute that points to a memory offset in the object.
#define URHO3D_ATTRIBUTE(name, typeName, variable, defaultValue, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(Urho3D::GetVariantType<typeName >(), name, offsetof(ClassName, variable), defaultValue, mode))
/// Define an attribute that points to a memory offset in the object, and uses zero-based enum values, which are mapped to names through an array of C string pointers.
#define URHO3D_ENUM_ATTRIBUTE(name, variable, enumNames, defaultValue, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(name, offsetof(ClassName, variable), enumNames, defaultValue, mode))
/// Define an attribute that uses get and set functions.
#define URHO3D_ACCESSOR_ATTRIBUTE(name, getFunction, setFunction, typeName, defaultValue, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(Urho3D::GetVariantType<typeName >(), name, new Urho3D::AttributeAccessorImpl<ClassName, typeName, Urho3D::AttributeTrait<typeName > >(&ClassName::getFunction, &ClassName::setFunction), defaultValue, mode))
/// Define an attribute that uses get and set free functions.
#define URHO3D_ACCESSOR_ATTRIBUTE_FREE(name, getFunction, setFunction, typeName, defaultValue, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(Urho3D::GetVariantType<typeName >(), name, new Urho3D::AttributeAccessorFreeImpl<ClassName, typeName, Urho3D::AttributeTrait<typeName > >(getFunction, setFunction), defaultValue, mode))
/// Define an attribute that uses get and set functions, and uses zero-based enum values, which are mapped to names through an array of C string pointers.
#define URHO3D_ENUM_ACCESSOR_ATTRIBUTE(name, getFunction, setFunction, typeName, enumNames, defaultValue, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(name, new Urho3D::EnumAttributeAccessorImpl<ClassName, typeName >(&ClassName::getFunction, &ClassName::setFunction), enumNames, defaultValue, mode))
/// Define an attribute that uses get and set free functions, and uses zero-based enum values, which are mapped to names through an array of C string pointers.
#define URHO3D_ENUM_ACCESSOR_ATTRIBUTE_FREE(name, getFunction, setFunction, typeName, enumNames, defaultValue, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(name, new Urho3D::EnumAttributeAccessorFreeImpl<ClassName, typeName >(getFunction, setFunction), enumNames, defaultValue, mode))
/// Define an attribute that uses get and set functions, where the get function returns by value, but the set function uses a reference.
#define URHO3D_MIXED_ACCESSOR_ATTRIBUTE(name, getFunction, setFunction, typeName, defaultValue, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(Urho3D::GetVariantType<typeName >(), name, new Urho3D::AttributeAccessorImpl<ClassName, typeName, Urho3D::MixedAttributeTrait<typeName > >(&ClassName::getFunction, &ClassName::setFunction), defaultValue, mode))
/// Define an attribute that uses get and set free functions, where the get function returns by value, but the set function uses a reference.
#define URHO3D_MIXED_ACCESSOR_ATTRIBUTE_FREE(name, getFunction, setFunction, typeName, defaultValue, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(Urho3D::GetVariantType<typeName >(), name, new Urho3D::AttributeAccessorFreeImpl<ClassName, typeName, Urho3D::MixedAttributeTrait<typeName > >(getFunction, setFunction), defaultValue, mode))
/// Update the default value of an already registered attribute.
#define URHO3D_UPDATE_ATTRIBUTE_DEFAULT_VALUE(name, defaultValue) context->UpdateAttributeDefaultValue<ClassName>(name, defaultValue)

// These macroses uses features which developed after 1.6 release. If Urho3D version is 1.6, they will not be defined and we can define safe implementation of them.
#ifndef URHO3D_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE
/// Define an attribute that uses get and set functions.
#define URHO3D_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE(name, getFunction, setFunction, typeName, defaultValue, variantStructureElementsNames, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(Urho3D::GetVariantType<typeName >(), name, new Urho3D::AttributeAccessorImpl<ClassName, typeName, Urho3D::AttributeTrait<typeName > >(&ClassName::getFunction, &ClassName::setFunction), defaultValue, mode))
#endif

#ifndef URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE
/// Define an variant structure attribute that uses get and set functions, where the get function returns by value, but the set function uses a reference.
#define URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE(name, getFunction, setFunction, typeName, defaultValue, variantStructureElementsNames, mode) context->RegisterAttribute<ClassName>(Urho3D::AttributeInfo(Urho3D::GetVariantType<typeName >(), name, new Urho3D::AttributeAccessorImpl<ClassName, typeName, Urho3D::MixedAttributeTrait<typeName > >(&ClassName::getFunction, &ClassName::setFunction), defaultValue, mode))
#endif
