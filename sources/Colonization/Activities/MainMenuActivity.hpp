#pragma once
#include <Colonization/Utils/Activity.hpp>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/AngelScript/ScriptInstance.h>

namespace Colonization
{
class MainMenuActivity : public Activity
{
URHO3D_OBJECT (MainMenuActivity, Activity)
protected:
    Urho3D::Scene *backgroundScene_;
    Urho3D::ScriptInstance *angelScriptGUI_;
public:
    MainMenuActivity (Urho3D::Context *context);
    virtual ~MainMenuActivity ();

    virtual void Start ();
    virtual void Update (float timeStep);
    virtual void Stop ();
};
}
