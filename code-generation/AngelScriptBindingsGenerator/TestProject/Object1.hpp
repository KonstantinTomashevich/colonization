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

//@ASBindGen Class ObjectType=Ref AddUrho3DDefaultConstructor
class Object1 :
    public Urho3D::Object,
    private DontBindMe
{
URHO3D_OBJECT (Object1, Object)
protected:
    Urho3D::String string_;
    float number_;

public:
    Object1 (Urho3D::Context *context);
    virtual ~Object1 ();

    //@ASBindGen Function
    static void StaticFunction ();
    //@ASBindGen Function
    void DoSomething ();

    //@ASBindGen Function OverrideName=get_string
    Urho3D::String GetString () const;
    //@ASBindGen Function OverrideName=set_string arg_string_TypeReplace=&| arg_string_TypeReplace=const|
    void SetString (const Urho3D::String &string);

    //@ASBindGen Function OverrideName=get_number
    float GetNumber () const;
    //@ASBindGen Function OverrideName=get_number
    void SetNumber (float string);
};
}
