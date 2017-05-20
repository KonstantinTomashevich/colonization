#pragma once
#include <TestProject/Object1.hpp>

namespace TestProject
{
//@ASBindGen Enum
enum MyEnumType
{
    ME_FIRST_VALUE = 0,
    ME_SECOND_VALUE,
    ME_THIRD_VALUE,
    ME_VALUES_COUNT
};

//@ASBindGen Class ObjectType=Ref AddUrho3DDefaultConstructor
class ObjectX :
    public Object1
{
URHO3D_OBJECT (ObjectX, Object1)
protected:
    Urho3D::Vector <Urho3D::String> stringArray_;

public:
    ObjectX (Urho3D::Context *context);
    virtual ~ObjectX ();

    //@ASBindGen Function OverrideName=get_array
    Urho3D::Vector <Urho3D::String> GetArray () const;
    //@ASBindGen Function OverrideName=set_array arg_array_TypeReplace=&| arg_array_TypeReplace=const|
    void SetArray (const Urho3D::Vector <Urho3D::String> &array);
};
}
