#pragma once
#include <Colonization/Utils/Activity.hpp>
#include <Colonization/Backend/GameStateManager.hpp>

namespace Colonization
{
class HostActivity : public Activity
{
URHO3D_OBJECT (HostActivity, Activity)
protected:
    GameStateManager *gameStateManager_;
    unsigned short serverPort_;
public:
    HostActivity (Urho3D::Context *context);
    virtual ~HostActivity ();

    unsigned short GetServerPort ();
    void SetServerPort (unsigned short serverPort);

    virtual void Start ();
    virtual void Update (float timeStep);
    virtual void Stop ();
};
}
