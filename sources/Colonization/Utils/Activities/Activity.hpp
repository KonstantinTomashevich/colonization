#pragma once
#include <Urho3D/Core/Object.h>

namespace Colonization
{
class ActivitiesApplication;
class Activity : public Urho3D::Object
{
URHO3D_OBJECT (Activity, Object)
protected:
    ActivitiesApplication *application_;
public:
    Activity (Urho3D::Context *context);
    virtual ~Activity ();

    ActivitiesApplication *GetApplication ();
    void SetApplication (ActivitiesApplication *application);

    virtual void Start () = 0;
    virtual void Update (float timeStep) = 0;
    virtual void Stop () = 0;
};
}
