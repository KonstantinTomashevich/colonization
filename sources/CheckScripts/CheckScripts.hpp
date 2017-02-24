#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/AngelScript/ScriptFile.h>
#include <AngelScript/angelscript.h>

class CheckScripts : public Urho3D::Application
{
URHO3D_OBJECT (CheckScripts, Application)
public:
    CheckScripts (Urho3D::Context *context);
    virtual ~CheckScripts ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
