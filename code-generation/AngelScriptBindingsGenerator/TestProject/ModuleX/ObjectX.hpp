#pragma once
#include <TestProject/Object1.hpp>

namespace TestProject
{
//@ASBindGen Urho3DSubsystem BindingName=subsystemX Type=MyNameSpace::SubsystemX
//@ASBindGen Enum
enum MyEnumType
{
    ME_FIRST_VALUE = 0,
    ME_SECOND_VALUE,
    ME_THIRD_VALUE,
    ME_VALUES_COUNT
};

//@ASBindGen Class ObjectType=Ref
class ObjectX :
    public Object1
{
URHO3D_OBJECT (ObjectX, Object1)
protected:
    Urho3D::Vector <Urho3D::String> stringArray_;

public:
    //@ASBindGen Constructor UseUrho3DScriptContext_arg0
    ObjectX (Urho3D::Context *context);
    //@ASBindGen Constructor UseUrho3DScriptContext_arg0
    ObjectX (Urho3D::Context *context, MyEnumType myEnumType);
    virtual ~ObjectX ();

    //@ASBindGen Function OverrideName=get_array
    Urho3D::Vector <Urho3D::String> GetArray () const;
    //@ASBindGen Function OverrideName=set_array ReplaceInType_arg0=&| ReplaceInType_arg0=const|
    void SetArray (const Urho3D::Vector <Urho3D::String> &array);
};
}
