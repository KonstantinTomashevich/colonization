#pragma once
#include <Urho3D/Engine/Application.h>
class CheckScripts : public Urho3D::Application
{
URHO3D_OBJECT (CheckScripts, Application)
public:
    explicit CheckScripts (Urho3D::Context *context);
    virtual ~CheckScripts ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
