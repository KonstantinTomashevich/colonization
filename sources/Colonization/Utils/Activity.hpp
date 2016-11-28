#pragma once
#include <Urho3D/Core/Object.h>

namespace Colonization
{
class Activity : public Urho3D::Object
{
URHO3D_OBJECT (Activity, Object)
public:
    Activity (Urho3D::Context *context);
    virtual ~Activity ();

    virtual void Init () = 0;
    virtual void Update (float timeStep) = 0;
    virtual void Stop () = 0;
};
}
