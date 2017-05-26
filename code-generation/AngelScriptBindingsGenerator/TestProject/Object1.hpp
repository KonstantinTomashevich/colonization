#pragma once
#include <Urho3D/Core/Object.h>

namespace TestProject
{
//@ASBindGen Function
float MyFreeFunction ();
//@ASBindGen Constant
const Urho3D::StringHash MY_CONSTANT;

class DontBindMe
{
public:
    DontBindMe ();
    virtual ~DontBindMe ();
    void DoAnything ();
};

//@ASBindGen Class ObjectType=Ref
class Object1 :
    public Urho3D::Object,
    private DontBindMe
{
URHO3D_OBJECT (Object1, Object)
protected:
    Urho3D::String string_;
    float number_;

public:
    //@ASBindGen Constructor UseUrho3DScriptContext_arg0
    Object1 (Urho3D::Context *context);
    virtual ~Object1 ();

    //@ASBindGen Function ReplaceInType_arg-1=&| ReplaceInType_arg-1=const|
    static const Urho3D::String &StaticFunction ();
    //@ASBindGen Function
    void DoSomething (float first = 3.14f, unsigned int second = 10);

    //@ASBindGen Function OverrideName=get_string
    Urho3D::String GetString () const;
    //@ASBindGen Function OverrideName=set_string ReplaceInType_arg0=&| ReplaceInType_arg0=const|
    void SetString (const Urho3D::String &string);

    //@ASBindGen Function OverrideName=get_number
    float GetNumber () const;
    //@ASBindGen Function OverrideName=set_number
    void SetNumber (float number);
};
}
