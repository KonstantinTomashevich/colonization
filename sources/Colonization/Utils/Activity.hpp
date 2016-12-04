#pragma once
#include <Urho3D/Core/Object.h>

namespace Colonization
{
class Activity : public Urho3D::Object
{
URHO3D_OBJECT (Activity, Object)
protected:
    void *application_;
public:
    Activity (Urho3D::Context *context);
    virtual ~Activity ();

    void *GetApplication ();
    void SetApplication (void *application);

    virtual void Start () = 0;
    virtual void Update (float timeStep) = 0;
    virtual void Stop () = 0;
};
}
