#pragma once
#include <Urho3D/Core/Object.h>

namespace Colonization
{
class ActivitiesApplication;
//@ASBindGen Class ObjectType=Ref
class Activity : public Urho3D::Object
{
URHO3D_OBJECT (Activity, Object)
public:
    explicit Activity (Urho3D::Context *context);
    virtual ~Activity ();

    //@ASBindGen Function AddRef_arg-1 OverrideName=get_application
    ActivitiesApplication *GetApplication ();
    //@ASBindGen Function AddRef_arg0 OverrideName=set_application
    void SetApplication (ActivitiesApplication *application);

    //@ASBindGen Function
    virtual void Start () = 0;
    //@ASBindGen Function
    virtual void Update (float timeStep) = 0;
    //@ASBindGen Function
    virtual void Stop () = 0;

private:
    ActivitiesApplication *application_;
};
}
