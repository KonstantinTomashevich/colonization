#pragma once
#include <Colonization/Utils/EditorLauncher.hpp>

class EditorLauncherImpl : public Colonization::EditorLauncher
{
URHO3D_OBJECT (EditorLauncher, Application)
public:
    EditorLauncherImpl (Urho3D::Context* context);
    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
